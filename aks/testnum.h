#ifndef TESTNUM
#define TESTNUM



#include <num.h>

namespace Prime
{
  typedef Prime::Num<std::uint64_t> TestNum;

  template <>
  std::uint32_t TestNum::BitsNum() const
  {
    std::uint64_t value = m_num;
    std::uint32_t result = 0;
    while (value >>= 1)
      result++;

    return result;
  }

  template <>
  bool TestNum::Bit(std::uint32_t index) const
  {
    return (m_num & (std::uint64_t(1) << index));
  }
}

#endif // TESTNUM

