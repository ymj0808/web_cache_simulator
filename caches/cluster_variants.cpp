#include <fstream>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <cassert>
#include <cmath>
#include <cassert>
#include "cluster_variants.h"
#include "../random_helper.h"

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