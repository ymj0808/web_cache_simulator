#include "double_queue_node.h"

dequeue_node::dequeue_node(dequeue_node *previous) : prev(previous) {}

void dequeue_node::copy_arr_from_vnode(
    std::map<uint32_t, uint32_t> &frag_arr_vnode,
    std::pair<unsigned int, unsigned int> &look_up_res) {
  virtual_node = look_up_res.first;
  real_node = look_up_res.second;
  size = frag_arr_vnode.size();
  arr.resize(size);
  std::copy(frag_arr_vnode.begin(), frag_arr_vnode.end(), arr.begin());
}

uint32_t dequeue_node::c_value(uint32_t pos) {
  uint32_t lo = 0, hi = size - 1;
  if (pos >= arr[hi].first)
    return 1;
  uint32_t mid = 0;
  while (lo < hi) {
    mid = (lo + hi) / 2;
    if (arr[mid].first > pos) {
      hi = mid - 1;
    } else if (arr[mid].first < pos) {
      lo = mid;
    } else {
      break;
    }
  }
  return arr[size - mid - 1].second;
}