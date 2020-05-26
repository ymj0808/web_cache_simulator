#include "node.h"

virtual_node::virtual_node(std::string ip, unsigned int hash_value, unsigned int cache_index)
{
    this->ip = ip;
    this->hash_value = hash_value;
    this->cache_index = cache_index;
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