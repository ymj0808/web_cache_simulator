#ifndef DOUBLE_QUEUE_NODE_H
#define DOUBLE_QUEUE_NODE_H

#include <stdint.h>
#include <map>
#include <vector>

class dequeue_node
{
public:
    std::vector<uint32_t> arr;
    //uint32_t* arr;
    uint32_t size;
    uint32_t last_access;
    uint32_t c;
    dequeue_node *prev = nullptr;
    dequeue_node *next = nullptr;
    uint32_t virtual_node;
    uint8_t real_node;
    dequeue_node(dequeue_node *previous);
    uint32_t c_value(uint32_t pos);
};


#endif
