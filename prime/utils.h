#ifndef UTILS_H
#define UTILS_H

#include <cstdint>

namespace Prime
{
  std::uint32_t HighestBit(std::uint64_t value)
  {
    std::uint32_t result = 0;
    while (value >>= 1)
      result++;

    return result;
  }

  std::uint64_t NextPowerOf2(std::uint64_t value)
  {
    std::uint64_t result = 1;
    result <<= HighestBit(value);
    if (value > result)
      result <<= 1;
    return result;
  }
}

#endif // UTILS_H

