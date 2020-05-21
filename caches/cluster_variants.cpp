#include <fstream>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <cassert>
#include <cmath>
#include <cassert>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <vector>
#include "cluster_variants.h"
#include "../random_helper.h"

/*
    Consistent hash
*/

bool CHCache::lookup(SimpleRequest* req) {
    CacheObject obj(req);
    auto cache_iter = mapper.find(obj);  // redirect to small cache
    return caches_list[cache_iter->second].lookup(req);
}

void CHCache::admit(SimpleRequest* req) {
    CacheObject obj(req);
    auto cache_iter = mapper.find(obj);
    caches_list[cache_iter->second].admit(req);
}

void CHCache::setPar(std::string parName, std::string parValue) {
    if(parName.compare("n") == 0) {     
        const int n = stoull(parValue);
        cache_number = n;
        caches_list = new LRUCache[cache_number];
        assert(n > 1);
        for (int i = 0; i < n; ++i) {
            caches_list[i].setSize(_cacheSize);
        }
    } 
    else {
        std::cerr << "unrecognized parameter: " << parName << std::endl;
    }
}

void CHCache::init_mapper(std::string path) {
    std::ifstream infile;
    infile.open(path);
    long long t, id, size;
    uint8_t cache_idx;
    std::hash<long long> hasher;
    SimpleRequest* req = new SimpleRequest(0, 0);

    while (infile >> t >> id >> size) {
        req->reinit(id, size);
        CacheObject obj(req);
        if (mapper.find(obj) == mapper.end()) {
            cache_idx = uint8_t(hasher(id) % cache_number);
            mapper.insert( std::pair<CacheObject, uint8_t>(CacheObject(req), cache_idx) );
        }
    }
    infile.close();
}

/*
    Shuffler
*/

bool Shuffler::lookup(SimpleRequest* req) {
    CacheObject obj(req);
    auto cache_iter = mapper.find(obj);  // redirect to small cache
    return caches_list[cache_iter->second].lookup(req);
}

void Shuffler::admit(SimpleRequest* req) {
    CacheObject obj(req);
    auto cache_iter = mapper.find(obj);
    caches_list[cache_iter->second].admit(req);
}

void Shuffler::setPar(std::string parName, std::string parValue) {
    if(parName.compare("n") == 0) {     // set the number of servers
        const int n = stoull(parValue);
        cache_number = n;
        caches_list = new LRUCache[cache_number];
        assert(n > 1);
        for (int i = 0; i < n; ++i) {
            caches_list[i].setSize(_cacheSize);
        }
    } 
    else if (parName.compare("W") == 0){    // set the window size
        const int w = stoull(parValue);
        window_size = w;
    }
    else if (parName.compare("K") == 0){                               // set the top k
        const int param = stoull(parValue);
        k = param;
    }
    else if (parName.compare("alpha") == 0) {
        double param = stoull(parValue);
        while (param > 1.0) 
            param /= 10;
        alpha = param;
    }
    else {
        std::cerr << "unrecognized parameter: " << parName << std::endl;
    }
}

void Shuffler::init_mapper(std::string path) {
    std::ifstream infile;
    infile.open(path);
    long long t, id, size;
    uint8_t cache_idx;
    std::hash<long long> hasher;
    SimpleRequest* req = new SimpleRequest(0, 0);

    while (infile >> t >> id >> size) {
        req->reinit(id, size);
        CacheObject obj(req);
        if (mapper.find(obj) == mapper.end()) {
            cache_idx = uint8_t(hasher(id) % cache_number);
            mapper.insert( std::pair<CacheObject, uint8_t>(CacheObject(req), cache_idx) );
            obj2row.insert( std::pair<CacheObject, int>(CacheObject(req), m));
            last_access.insert( std::pair<CacheObject, int>(CacheObject(req), 0));
            m++;
            //matrix_block* head = new matrix_block(head, nullptr);
            //matrix_block* cur = head;
            //for (int i = 0; i < window_size; i++, cur = cur->next) {
            //    cur->next = new matrix_block(cur, nullptr);
            //}
            //matrix.insert( (std::pair<CacheObject, matrix_block*>(CacheObject(req), head)) );
            //cur = head;
            //pointer.insert( (std::pair<CacheObject, matrix_block*>(CacheObject(req), cur)) );
        }
    }
    matrix = new uint32_t*[m];
    blocks = int(window_size / 32);
    for (uint64_t i = 0; i < m; i++) {
        matrix[i] = new uint32_t[blocks];
        for (int j = 0; j < blocks; ++j) {
            matrix[i][j] = 0;
        }
    }
    infile.close();
    std::cerr << "init done" << std::endl;
}

void Shuffler::reset() {
    position = 0;
    //auto size = sizeof(matrix);
    //memset(matrix, 0, size);
    for (uint64_t i = 0; i < m; ++i) {
        for (int j = 0; j < blocks; j++) {
            matrix[i][j] = 0;
        }
    }
    _rank_list_map.clear();
    sd.clear();
}

bool Shuffler::requested(int row, int prev) {
    for (int i = prev >> 5; i < position>>5 ; ++i)
        if (matrix[row][i])
            return true;
    return false;
}

bool cmp(const std::pair<CacheObject, double> &x, const std::pair<CacheObject, double> &y) {
    return x.second > y.second;
}

bool Shuffler::request(SimpleRequest* req) {
    CacheObject obj(req);
    int row = obj2row.find(obj)->second;
    auto cache_index = mapper.find(obj)->second;  // redirect to small cache
    matrix[row][position >> 5] |= (1 << (position & 0b11111));
    last_access.find(obj)->second = position;
    //cacheNumber2requestedObj.find(cache_index)->second.insert(obj);

    auto rank_list_map_it = _rank_list_map.find(obj);
    // get stack distance for this request
    if (rank_list_map_it != _rank_list_map.end()) {     // not first request for this content
        int stack_distance = 0;
        for (auto it = _rank_list.begin(); it != rank_list_map_it->second; ++it)
            stack_distance++;
        sd.find(obj)->second.request(stack_distance);
        _rank_list.splice(_rank_list.begin(), _rank_list, rank_list_map_it->second);
    }
    else {
        sd.insert(std::pair<CacheObject, sd_block>(obj, sd_block()));
        _rank_list.push_front(obj);
        _rank_list_map[obj] = _rank_list.begin();
    }

    bool flag = caches_list[cache_index].lookup(req);
    if (!flag) {
        caches_list[cache_index].admit(req);
    }

    position++;
    if (position % window_size == 0) {
        std::vector<std::pair<CacheObject, double> > vtMap;
        for (auto it = sd.begin(); it != sd.end(); it++)
            vtMap.push_back(std::pair<CacheObject, double>(it->first, 
                            it->second.sum_sd * alpha + it->second.times * (1 - alpha)) );
        std::sort(vtMap.begin(), vtMap.end(), cmp);
        for (int i = 0; i < k; i++) {
            mapper[vtMap[i].first] = uint8_t();
        }
        reset();
        std::cerr << "*********** update ***********" << std::endl;
    }
    
    return flag;
}
