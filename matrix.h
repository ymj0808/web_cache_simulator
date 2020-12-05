#ifndef MATRIX_H
#define MATRIX_H

#include <./caches/cache_object.h>
#include <cstdint>

struct matrix_block {
  matrix_block *prev;
  matrix_block *next;
  uint32_t meta_data;
  bool flag;

  matrix_block(matrix_block *p, matrix_block *n) {
    prev = p;
    next = n;
    meta_data = 0;
    flag = false;
  }

  void request(const uint8_t pos) {
    meta_data = meta_data & (1 << pos);
    flag = true;
  }
};

struct sd_block {
  uint64_t sum_sd = 1;
  int times = 1;

  void request(int sd) {
    sum_sd += sd;
    times++;
  }
};

#endif