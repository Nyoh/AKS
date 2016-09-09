#ifndef TESTNUM
#define TESTNUM



#include <num.h>

namespace AKSN
{
  typedef AKSN::Num<std::uint64_t> TestNum;

  template <>
  std::uint64_t TestNum::BitsNum() const
  {
    std::uint64_t value = m_num;
    std::uint64_t result = 0;
    while (value >>= 1)
      result++;

    return result;
  }
}

#endif // TESTNUM

