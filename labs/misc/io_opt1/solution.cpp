#include "solution.hpp"

#include <fstream>
#include <stdexcept>
// 由于计算 32 位数据块的 CRC 非常廉价[1]，因此该问题的性能瓶颈主要取决于我们向 CPU 提供这些 32 位数据块的速度。
uint32_t solution(const char *file_name) {
  std::fstream file_stream{file_name};
  if (!file_stream.is_open())
    throw std::runtime_error{"The file could not be opened"};

  // Initial value has all bits set to 1
  uint32_t crc = 0xff'ff'ff'ff;

  // Update the CRC32 value character by character
  char c;
  while (true) {
    file_stream.read(&c, 1);
    if (file_stream.eof())
      break;
    update_crc32(crc, static_cast<uint8_t>(c));
  }

  // Invert the bits
  crc ^= 0xff'ff'ff'ff;

  return crc;
}
