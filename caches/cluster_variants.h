#ifndef CLUSTER_VARIENTS_H
#define CLUSTER_VARIENTS_H

#include <unordered_map>
#include <list>
#include <set>
#include <random>
#include "cache.h"
#include "cache_object.h"
#include "lru_variants.h"
#include "../matrix.h"
#include <chrono>

/*
    this file defines cluster-based cache policy
*/

/*
    Consistent hash
*/

class CHCache : public Cache
{
protected:
    int cache_number;
    LRUCache *caches_list;                           // LRUCaches cluster
    std::unordered_map<CacheObject, uint8_t> mapper; // map CacheObjec to LRUCache

public:
    CHCache() : Cache() {}

    virtual ~CHCache() {}

    virtual void setPar(std::string parName, std::string parValue);
    void init_mapper(std::string filepath);
    virtual bool lookup(SimpleRequest *req);
    virtual void admit(SimpleRequest *req);
    virtual void evict(SimpleRequest *req){};
    virtual void evict(){};
};

static Factory<CHCache> factoryCH("CH");

/*
    Shuffler
*/

//bool cmp(std::pair<CacheObject, double> &x, std::pair<CacheObject, double> &y);

class Shuffler : public Cache
{
protected:
    int cache_number; // cache number in cluster
    int window_size;  // for each window_size gap, run optimizer
    int blocks;       // how many matrix blocks for each row
    int k;            // top k contents to redistribute
    double alpha;
    int occur_count;
    std::chrono::steady_clock::time_point time;
    uint64_t m = 0;                                  // unique content number
    LRUCache *caches_list;                           // LRUCaches cluster
    std::unordered_map<CacheObject, uint8_t> mapper; // map CacheObjec to LRUCache
    std::list<CacheObject> _rank_list;               // LRU stack order for requested content
    lruCacheMapType _rank_list_map;                  //
    std::unordered_map<CacheObject, sd_block> sd;    // obj to sd_block

    uint32_t **matrix;                            // request matrix, reset
    std::unordered_map<CacheObject, int> obj2row; // reset
    std::unordered_map<CacheObject, int> last_access;

    int position = 0;

public:
    Shuffler() : Cache() {}

    virtual void setPar(std::string parName, std::string parValue);
    void init_mapper(std::string filepath);
    virtual bool lookup(SimpleRequest *req);
    virtual void admit(SimpleRequest *req);
    virtual void evict(SimpleRequest *req){};
    virtual void evict(){};

    bool request(SimpleRequest *req);
    void reset();
    bool requested(int row, int prev);
};

static Factory<Shuffler> factorySF("SF");

#endif
