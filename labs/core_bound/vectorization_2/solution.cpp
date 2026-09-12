#include "solution.hpp"
/*
uint16_t checksum(const Blob &blob) {
  uint16_t acc = 0;
  for (auto value : blob) {
    acc += value;
    acc += acc < value; // add carry
  }
  return acc;
}
*/
uint16_t checksum(const Blob &blob) {
    uint32_t acc = 0;
    uint32_t pre = 0;
    uint32_t two_pow_16 = (1 << 16);
    for (int i = 0; i < blob.size(); i += two_pow_16) {
        for (int j = i; j < i+ two_pow_16; j++) {
            acc += blob[j];
        }
        if (acc < pre) {
            acc += 1;
        }
    }
    pre = acc;
    uint16_t top = acc >> 16;
    uint16_t bottom = acc & (two_pow_16 - 1);
    acc = top + bottom;
    if (acc < pre) {
        acc += 1;
    } 
    return acc;
}
