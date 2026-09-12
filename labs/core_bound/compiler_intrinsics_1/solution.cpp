
#include "solution.h"
#include <memory>
#include <xmmintrin.h>
// #include <emmintrin.h>
#include <smmintrin.h>
void imageSmoothing(const InputVector &input, uint8_t radius,
                    OutputVector &output) {

/*
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 2. main loop.
  limit = size - radius;
  for (; pos < limit; ++pos) {
    currentSum -= input[pos - radius - 1];
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }

  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }
*/
  int pos = 0;
  int currentSum = 0;
  int size = static_cast<int>(input.size());

  // 1. left border - time spend in this loop can be ignored, no need to
  // optimize it
  for (int i = 0; i < std::min<int>(size, radius); ++i) {
    currentSum += input[i];
  }

  int limit = std::min(radius + 1, size - radius);
  for (pos = 0; pos < limit; ++pos) {
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }
  
  // 2. main loop.
  limit = size - radius;
  // __m256i _mm256_set1_epi32 (currentSum);
  __m128i current =  _mm_set1_epi16(currentSum);
  uint8_t * subtractPtr = const_cast<uint8_t*>(&input[pos - radius - 1]);   // const uint8_t *subtractptr = input.data() + pos - radius - 1;
  uint8_t *addPtr = const_cast<uint8_t*>(&input[pos + radius]);             // const uint8_t *addPtr = input.data() + pos + radius;
  uint16_t *outputPtr = const_cast<uint16_t*>(&output[pos]);                   // const uint16_t* outputPtr = output.data() + pos;
  constexpr size_t UNROLL = 8;
  size_t i = 0;
  for (; pos + i + UNROLL - 1 < limit; i += UNROLL) {
      __m128i sub_u8 = _mm_loadu_si64(subtractPtr + i);  // 加载操作不区分类型，虽然subtractPtr是uint8_t*, 也能使用si64进行加载
      __m128i add_u8 = _mm_loadu_si64(addPtr + i);
      __m128i sub = _mm_cvtepu8_epi16(sub_u8);
      __m128i add = _mm_cvtepu8_epi16(add_u8);
      __m128i diff = _mm_sub_epi16(add, sub);
      // 逻辑移位不考虑符号，纯粹把数据看作一串二进制位. 算数移位操作，逻辑移位，循环移位都有各自的应用哦个场景

      __m128i s =  _mm_adds_epi16(diff, _mm_slli_si128 (diff, 2));
      s = _mm_adds_epi16(s, _mm_slli_si128(s, 4));
      s = _mm_adds_epi16(s, _mm_slli_si128(s, 8));
      __m128i result = _mm_adds_epi16(s, current);
      _mm_storeu_si128((__m128i*)(outputPtr + i), result);
      currentSum = _mm_extract_epi16(result, 7); 
      current = _mm_set1_epi16(currentSum);    
  }
  pos += i;
  for(; pos < limit; pos++) {                     // 不够8个的情况，按照标量处理
    currentSum -= input[pos - radius -1];
    currentSum += input[pos + radius];
    output[pos] = currentSum;
  }



  // 3. special case, executed only if size <= 2*radius + 1
  limit = std::min(radius + 1, size);
  for (; pos < limit; pos++) {
    output[pos] = currentSum;
  }

  // 4. right border - time spend in this loop can be ignored, no need to
  // optimize it
  for (; pos < size; ++pos) {
    currentSum -= input[pos - radius - 1];
    output[pos] = currentSum;
  }

}
