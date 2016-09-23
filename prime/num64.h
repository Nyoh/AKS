#ifndef TESTNUM
#define TESTNUM

#include "num.h"
#include "utils.h"

namespace Prime
{
    typedef Prime::Num<std::uint64_t> Num64;

    template <>
    std::uint32_t Num64::BitsNum() const
    {
        return HighestBit(m_num);
    }

    template <>
    bool Num64::Bit(std::uint32_t index) const
    {
        return (m_num & (std::uint64_t(1) << index)) != 0;
    }
}

#endif // TESTNUM

