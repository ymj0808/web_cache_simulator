#ifndef DOUBLE_QUEUE_NODE_H
#define DOUBLE_QUEUE_NODE_H

#include <map>
#include <stdint.h>
#include <vector>

class dequeue_node {
public:
  dequeue_node(dequeue_node *previous);
  void copy_arr_from_vnode(std::map<uint32_t, uint32_t> &frag_arr_vnode,
                           std::pair<unsigned int, unsigned int> &look_up_res);
  /*
    for arr, only record the start index of each increment fragment (see the
    paper)
    example : [2, 1, 1, 1, 1, 0 ... 0] -> [0, 1, 5]
    now consider real size of content, should record the true value of the arr fragment
    example : [0, 1, 5] -> [<0, 7>, <1, 2>, <5, 0>]
  */
  std::vector<std::pair<uint32_t, uint32_t>> arr;
  uint32_t size;
  uint32_t last_access;
  uint32_t c;
  dequeue_node *prev = nullptr;
  dequeue_node *next = nullptr;
  uint32_t virtual_node;
  uint8_t real_node;
  uint32_t c_value(uint32_t pos);
};

#endif
