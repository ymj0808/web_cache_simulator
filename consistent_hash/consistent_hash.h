#ifndef CONSISTENT_HASH_H
#define CONSISTENT_HASH_H

#include <map>
#include <algorithm>
#include <iostream>
#include <vector>
#include "node.h"

class consistent_hash
{
public:
    unsigned int real_node_sum;
    unsigned int virtual_node_sum;
    std::map<std::string, real_node> real_node_map;
    std::map<unsigned int, virtual_node> virtual_node_map;  // hash position to virtual node
    std::vector<unsigned int> sorted_node_hash_list; // the position of each virtual node on hash ring


    consistent_hash();

    ~consistent_hash();

    unsigned int look_up(const std::string& content);

    unsigned int find_nearest_node(unsigned int hash_value);

    // unsigned int put(std::string data_id);

    void add_real_node(std::string ip, unsigned int virtual_node_num);

    // void drop_real_node(std::string ip);

    // void print_real_node(std::string ip);

    // void print();
};

#endif //LEETCODE_MY_HASH_H
