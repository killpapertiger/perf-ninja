#include "solution.hpp"
#include <iostream>

// Find the longest line in a file.
// Implementation uses ternary operator with a hope that compiler will
// turn it into a CMOV instruction.
// The code inside the inner loop is equivalent to:
/*
if (s == '\n') {
  longestLine = std::max(curLineLength, longestLine);
  curLineLength = 0;
} else {
  curLineLength++;
}*/
unsigned solution(const std::string &inputContents) {

/*
  unsigned longestLine = 0;
  unsigned curLineLength = 0;

  for (auto s : inputContents) {
    curLineLength = (s == '\n') ? 0 : curLineLength + 1;
    longestLine = std::max(curLineLength, longestLine);
  }

  return longestLine;
*/
  unsigned longestLine = 0;
  unsigned curLineLength = 0;
  // char end = `a`;
  __m256i eol = _mm256_set1_epi8 (`\n`);
  
  __m256i const * buffer = inputContents.data();
  for (int i = 0; i + 32 - 1 < inputContents.size(); i += 32) {
    __m256i vect = _mm256_loadu_si256 ((__m256i const*)(buffer));
    __m256i vectMask = _mm256_cmpeq_epi8 (vect, eol)；
    // vectMask 确实不能直接放入 while 条件中作为布尔值判断，因为这涉及到 SIMD 数据类型和 C++ 类型系统的一个关键差异。
    // __m256i 是一个聚合类型（本质上是一个包含 256 位数据的结构体），
    // 它不是 C++ 的内置类型，编译器不知道如何将它隐式转换为布尔值。 所以你无法写出 while (vectMask) { ... } 这样的代码。
    // int _mm256_movemask_epi8 (__m256i a)的返回值是int,但是perf-ninja将他用作uint32_t
    // 结论是：这样做是完全正确且安全的，因为在 x86-64 平台上，int 和 uint32_t 在底层是相同的 32 位数据，
    // 将 int 当作 uint32_t 使用只是一种“类型重解释”，不影响实际的二进制位模式和性能。
    // 关键是：后续代码只关心 mask 的二进制位模式（用 _tzcnt_u32 和 mask >>= 操作），而不关心它的“数值大小”或“正负号”。
    uint32_t mask = _mm256_movemask_epi8 (vectMask);
    while (mask) {
        
    }
  }
}
