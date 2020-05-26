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

bool CHCache::lookup(SimpleRequest *req)
{
    //CacheObject obj(req);
    auto cache_index = chash.look_up(std::to_string(req->getId()));
    //auto cache_iter = mapper.find(obj); // redirect to small cache
    //return caches_list[cache_iter->second].lookup(req);
    return caches_list[cache_index].lookup(req);
}

void CHCache::admit(SimpleRequest *req)
{
    //CacheObject obj(req);
    //auto cache_iter = mapper.find(obj);
    //caches_list[cache_iter->second].admit(req);
    auto cache_index = chash.look_up(std::to_string(req->getId()));
    caches_list[cache_index].admit(req);
}

void CHCache::setPar(std::string parName, std::string parValue)
{
    if (parName.compare("n") == 0)
    {
        const int n = stoull(parValue);
        cache_number = n;
        caches_list = new LRUCache[cache_number];
        assert(n > 1);
        for (int i = 0; i < n; ++i)
        {
            caches_list[i].setSize(_cacheSize);
        }
    }
    else
    {
        std::cerr << "unrecognized parameter: " << parName << std::endl;
    }
}

void CHCache::init_mapper(std::string path)
{
    /*
    std::ifstream infile;
    infile.open(path);
    long long t, id, size;
    uint8_t cache_idx;
    std::hash<long long> hasher;
    SimpleRequest *req = new SimpleRequest(0, 0);

    while (infile >> t >> id >> size)
    {
        req->reinit(id, size);
        CacheObject obj(req);
        if (mapper.find(obj) == mapper.end())
        {
            cache_idx = uint8_t(hasher(id) % cache_number);
            mapper.insert(std::pair<CacheObject, uint8_t>(CacheObject(req), cache_idx));
        }
    }
    infile.close();
    */
    chash.add_real_node("192.168.0.136", 40);
    chash.add_real_node("192.168.1.137", 40);
    chash.add_real_node("192.168.2.138", 40);
    chash.add_real_node("192.168.3.139", 40);
}

bool CHCache::request(SimpleRequest *req)
{
    auto cache_index = chash.look_up(std::to_string(req->getId()));
    //auto cache_index = mapper.find(obj)->second; // redirect to small cache
    bool flag = caches_list[cache_index].lookup(req);
    if (!flag)
    {
        caches_list[cache_index].admit(req);
    }
    return flag;
}

/*
    Shuffler
*/

bool Shuffler::lookup(SimpleRequest *req)
{

    //CacheObject obj(req);
    //auto cache_iter = mapper.find(obj); // redirect to small cache
    //return caches_list[cache_iter->second].lookup(req);
    auto cache_index = chash.look_up(std::to_string(req->getId()));
    //auto cache_iter = mapper.find(obj); // redirect to small cache
    //return caches_list[cache_iter->second].lookup(req);
    return caches_list[cache_index].lookup(req);
}

void Shuffler::admit(SimpleRequest *req)
{
    //CacheObject obj(req);
    //auto cache_iter = mapper.find(obj);
    //caches_list[cache_iter->second].admit(req);
    auto cache_index = chash.look_up(std::to_string(req->getId()));
    caches_list[cache_index].admit(req);
}

void Shuffler::setPar(std::string parName, std::string parValue)
{
    if (parName.compare("n") == 0)
    { // set the number of servers
        const int n = stoull(parValue);
        cache_number = n;
        caches_list = new LRUCache[cache_number];
        assert(n > 1);
        for (int i = 0; i < n; ++i)
        {
            caches_list[i].setSize(_cacheSize);
        }
    }
    else if (parName.compare("W") == 0)
    { // set the window size
        const int w = stoull(parValue);
        window_size = w;
    }
    else if (parName.compare("K") == 0)
    { // set the top k
        const int param = stoull(parValue);
        k = param;
    }
    else if (parName.compare("alpha") == 0)
    {
        double param = stoull(parValue);
        while (param > 1.0)
            param /= 10;
        alpha = param;
    }
    else
    {
        std::cerr << "unrecognized parameter: " << parName << std::endl;
    }
}

void Shuffler::print_hash_space()
{
    for (auto i : chash.sorted_node_hash_list)
    {
        std::cout << i << ',';
    }
    std::cout << std::endl;
}

void Shuffler::init_mapper(std::string path)
{
    // using mapper
    /*
    std::ifstream infile;
    infile.open(path); 
    long long t, id, size;
    uint8_t cache_idx;
    std::hash<long long> hasher;
    SimpleRequest *req = new SimpleRequest(0, 0);

    while (infile >> t >> id >> size)
    {
        req->reinit(id, size);
        CacheObject obj(req);
        if (mapper.find(obj) == mapper.end())
        {
            cache_idx = uint8_t(hasher(id) % cache_number);
            mapper.insert(std::pair<CacheObject, uint8_t>(CacheObject(req), cache_idx));
            obj2row.insert(std::pair<CacheObject, int>(CacheObject(req), m));
            last_access.insert(std::pair<CacheObject, int>(CacheObject(req), 0));
            m++;
        }
    }
    infile.close();
    */

    // using consistent hash

    chash.add_real_node("192.168.0.136", 40);
    chash.add_real_node("192.168.1.137", 40);
    chash.add_real_node("192.168.2.138", 40);
    chash.add_real_node("192.168.3.139", 40);

    print_hash_space();

    matrix = new uint32_t *[window_size]; // only record last window_size
    blocks = int(window_size / 32);
    occur_count = 0;
    for (int i = 0; i < window_size; i++)
    {
        matrix[i] = new uint32_t[blocks];
        for (int j = 0; j < blocks; ++j)
        {
            matrix[i][j] = 0;
        }
    }
    request_count = new uint64_t[cache_number];
    hit_count = new uint64_t[cache_number];
    miss_count = new uint64_t[cache_number];
    miss_rate = new double[cache_number];
    usage_ratio = new double[cache_number];
    rank = new double[cache_number];
    for (int i = 0; i < cache_number; ++i)
    {
        request_count[i] = 0;
        hit_count[i] = 0;
        miss_count[i] = 0;
        miss_rate[i] = 0.0;
        usage_ratio[i] = 0.0;
    }

    time = std::chrono::steady_clock::now();
    std::cerr << "init done" << std::endl;
    for (auto i : chash.sorted_node_hash_list)
    {
        cache_index_each_node.push_back(chash.virtual_node_map[i].cache_index);
        virtual_node_number++;
    }
    std::cout << virtual_node_number << std::endl;
    for (auto i : cache_index_each_node)
    {
        std::cout << i << '\t';
    }
    std::cout << std::endl;
}

void Shuffler::reset()
{
    position = 0;
    //auto size = sizeof(matrix);
    //memset(matrix, 0, size);
    /*
    for (int i = 0; i < window_size; ++i)
    {
        for (int j = 0; j < blocks; j++)
        {
            matrix[i][j] = 0;
        }
    }
    */
    for (int i = 0; i < cache_number; ++i)
    {
        request_count[i] = 0;
        hit_count[i] = 0;
        miss_count[i] = 0;
        usage_ratio[i] = 0;
        rank[i] = 0.0;
    }
    min2max.clear();
    /*
    _rank_list_map.clear();
    _rank_list.clear();
    sd.clear();
    last_access.clear();
    occur_count = 0;
    obj2row.clear();
    */
}

bool Shuffler::requested(int row, int prev)
{
    for (int i = prev >> 5; i<position>> 5; ++i)
        if (matrix[row][i])
            return true;
    return false;
}

bool cmp(const std::pair<CacheObject, double> &x, const std::pair<CacheObject, double> &y)
{
    return x.second > y.second;
}

bool cmp2(const std::pair<int, unsigned int> &x, const std::pair<int, unsigned int> &y)
{
    return x.second > y.second;
}

bool Shuffler::request(SimpleRequest *req)
{
    /*
    CacheObject obj(req);
    auto rank_list_map_it = _rank_list_map.find(obj);
    // get stack distance for this request
    if (rank_list_map_it != _rank_list_map.end())
    { // not first request for this content
        int stack_distance = 0;
        for (auto it = _rank_list.begin(); it != rank_list_map_it->second; ++it)
            stack_distance++;
        sd.find(obj)->second.request(stack_distance);
        _rank_list.splice(_rank_list.begin(), _rank_list, rank_list_map_it->second);
        matrix[obj2row[obj]][position >> 5] |= (1 << (position & 0b11111));
        last_access[obj] = position;
    }
    else
    { // first occur
        //sd.insert(std::pair<CacheObject, sd_block>(obj, sd_block()));
        //obj2row.insert(std::pair<CacheObject, int>(obj, occur_count));
        matrix[occur_count][position >> 5] |= (1 << (position & 0b11111));
        last_access.insert(std::pair<CacheObject, int>(obj, position));
        _rank_list.push_front(obj);
        _rank_list_map[obj] = _rank_list.begin();
        occur_count++;
    }
    */

    auto cache_index = chash.look_up(std::to_string(req->getId()));
    //auto cache_index = mapper.find(obj)->second; // redirect to small cache
    request_count[cache_index]++;
    bool flag = caches_list[cache_index].lookup(req);
    if (!flag)
    {
        caches_list[cache_index].admit(req);
        miss_count[cache_index]++;
    }
    else
    {
        hit_count[cache_index]++;
    }
    position++;
    if (position == window_size)
    {

        unsigned int max_i = 0, min_i = 0;
        //uint64_t max = request_count[0], min = request_count[0];
        unsigned int max_requests = request_count[0];
        for (int i = 1; i < cache_number; ++i)
        {
            if (max_requests < request_count[i])
            {
                max_requests = request_count[i];
            }
        }
        for (int i = 0; i < cache_number; ++i)
        {
            miss_rate[i] = miss_count[i] / request_count[i];
            usage_ratio[i] = request_count[i] / max_requests;
            rank[i] = miss_rate[i] + usage_ratio[i];
        }
        double max_rank = rank[0], min_rank = rank[0];
        for (int i = 1; i < cache_number; ++i)
        {
            if (rank[i] > max_rank)
            {
                max_rank = rank[i];
                max_i = i;
            }
            if (rank[i] < min_rank)
            {
                min_rank = rank[i];
                min_i = i;
            }
        }

        for (int i = 0; i < 159; ++i)
        { /*
            if (cache_index_each_node[i] == min_i)
            {
                if (cache_index_each_node[i + 1] == max_i)
                {
                    //min2max.push_back(std::pair<int, unsigned int>(i, chash.sorted_node_hash_list[i + 1] - chash.sorted_node_hash_list[i]));
                    std::cout<< "d";
                }
            }
            */
            std::cout << min_i << max_i;
        }

        //std::cout << "Shift " << min_i << " to " << max_i << std::endl;
        /*
        if (min2max.size() != 0)
        {
            sort(min2max.begin(), min2max.end(), cmp2); // find the bigest gap of min node to max node, shift this min node to max node
            int moving_min_index = min2max[0].first;
            //unsigned int mid = min2max[0].second * (1 - min / max) / 10;
            //mid += chash.sorted_node_hash_list[moving_min_index];
            //unsigned int mid = chash.sorted_node_hash_list[moving_min_index];
            auto iter = chash.virtual_node_map.find(chash.sorted_node_hash_list[moving_min_index]);
            if (iter == chash.virtual_node_map.end())
                std::cout << "fuck" << std::endl;
            //(unsigned int &)iter->first = mid;
            //iter->second.hash_value = mid;
            //chash.sorted_node_hash_list[moving_min_index] = mid;
            //std::cout << "Shift " << min_i << " to " << max_i << " success!" << std::endl;
        }
        else
        {
            std::cout << "Shift " << min_i << " to " << max_i << " fail!" << std::endl;
        }
        


        reset();
        //const long timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time).count();
        //std::cerr << "*********** update *********** time elapsed: " << timeElapsed << std::endl;
        //time = std::chrono::steady_clock::now();
        */
        reset();
    }

    /*
    position++;
    if (position % window_size == 0)
    {
        std::vector<std::pair<CacheObject, double>> vtMap;
        for (auto it = sd.begin(); it != sd.end(); it++)
            vtMap.push_back(std::pair<CacheObject, double>(it->first,
                                                           it->second.sum_sd * alpha + it->second.times * (1 - alpha)));
        std::sort(vtMap.begin(), vtMap.end(), cmp);
        for (int i = 0; i < k; i++)
        {
            mapper[vtMap[i].first] = uint8_t(i % cache_number);
        }
        reset();
        const long timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time).count();
        std::cerr << "*********** update *********** time elapsed: " << timeElapsed << std::endl;
        time = std::chrono::steady_clock::now();
    }
    */

    return flag;
}
