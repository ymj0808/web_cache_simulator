#ifndef CLUSTER_VARIENTS_H
#define CLUSTER_VARIENTS_H

#include <unordered_map>
#include <list>
#include <random>
#include "cache.h"
#include "cache_object.h"
#include "lru_variants.h"

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
    LRUCache* caches_list;             // LRUCaches cluster
    std::unordered_map<CacheObject, uint8_t> mapper;   // map CacheObjec to LRUCache

public:
    CHCache() : Cache() {}

    virtual ~CHCache() {}

    virtual void setPar(std::string parName, std::string parValue);
    void init_mapper(std::string filepath);
    virtual bool lookup(SimpleRequest* req);
    virtual void admit(SimpleRequest* req);
    virtual void evict(SimpleRequest* req) {};
    virtual void evict() {};
};

static Factory<CHCache> factoryCH("CH");

#endif