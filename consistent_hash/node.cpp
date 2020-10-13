#include "node.h"

virtual_node::virtual_node(std::string ip, unsigned int hash_value, int cache_index)
{
    this->ip = ip;
    this->hash_value = hash_value;
    this->cache_index = cache_index;
}

/* 
virtual_node::virtual_node(std::string ip, unsigned int hash_value, int cache_index)
{
    virtual_node(ip, hash_value, cache_index, 0);
}
*/              //ymj 2020 10 12

virtual_node::virtual_node(std::string ip, unsigned int hash_value, int cache_index, int uid)
{
    this->ip = ip;
    this->hash_value = hash_value;
    this->cache_index = cache_index;
    this->uid = uid;
}

virtual_node::~virtual_node()
{
    //data.clear();
}

virtual_node::virtual_node()
{
    this->ip = "";
    this->hash_value = 0;
    this->cache_index = 0;
}

void virtual_node::SetIP(std::string ip)
{
    this->ip = ip;
    std::cout << "[SetIP = ]]\t" << this->ip << std::endl; // 10122020 Peixuan vnode debug 
}

void virtual_node::SetHashValue(unsigned int hash_value)
{
    this->hash_value = hash_value;
    std::cout << "[SetHashValue = ]]\t" << this->hash_value << std::endl; // 10122020 Peixuan vnode debug
}


void virtual_node::SetCacheIndex(int cache_index)
{
    this->cache_index = cache_index;
    std::cout << "[SetCacheIndex = ]]\t" << this->cache_index << std::endl; // 10122020 Peixuan vnode debug 
}

void virtual_node::SetUid(int uid)
{
    this->uid = uid;
    std::cout << "[SetUID = ]]\t" << this->uid << std::endl; // 10122020 Peixuan vnode debug 
}

real_node::real_node(std::string ip)
{
    this->ip = ip;
    this->cur_max_port = 0;
    this->virtual_node_num = 0;
}

real_node::~real_node()
{
    this->virtual_node_hash_list.clear();
}

real_node::real_node()
{
    this->ip = "";
    this->cur_max_port = 0;
    this->virtual_node_num = 0;
}