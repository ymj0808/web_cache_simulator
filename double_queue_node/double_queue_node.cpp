#include "double_queue_node.h"

dequeue_node::dequeue_node(dequeue_node* previous) {
    prev = previous;
}

uint32_t dequeue_node::c_value(uint32_t pos) {
    uint32_t lo = 0, hi = size - 1;
    if (pos >= arr[hi])
        return 1;
    uint32_t mid = 0;
    while (lo < hi) {
        mid = (lo + hi) / 2;
        if (arr[mid] > pos) {
            hi = mid - 1;
            continue;
        }
        else if (arr[mid] < pos) {
            lo = mid;
            continue;
        }
        else {
            break;
        }
    }
    return size - mid - 1;
}