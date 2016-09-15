#ifndef TESTNUM
#define TESTNUM



#include <num.h>

namespace Prime
{
  typedef Prime::Num<std::uint64_t> Num64;

  template <>
  std::uint32_t Num64::BitsNum() const
  {
    std::uint64_t value = m_num;
    std::uint32_t result = 0;
    while (value >>= 1)
      result++;

    return result;
  }

  template <>
  bool Num64::Bit(std::uint32_t index) const
  {
    return (m_num & (std::uint64_t(1) << index));
  }
}

#endif // TESTNUM

