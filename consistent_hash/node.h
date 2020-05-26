#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

class virtual_node
{
public:
    std::string ip; // the ip address of this virtual node, format: real_node_ip:port
    unsigned int hash_value;
    //map<unsigned int, string> data;
    unsigned int cache_index;         // count how many request on this node

    virtual_node();

    virtual_node(std::string ip, unsigned int hash_value, unsigned int cache_index);

    ~virtual_node();
};

class real_node
{
public:
    std::string ip; // the ip address of this real node
    unsigned int virtual_node_num;  // the virtual node number
    std::vector<unsigned int> virtual_node_hash_list;
    unsigned int cur_max_port;   

    real_node();

    real_node(std::string ip);

    ~real_node();
};






#endif
