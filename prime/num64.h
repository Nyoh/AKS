#ifndef TESTNUM
#define TESTNUM

#include "num.h"
#include "utils.h"

namespace Prime
{
    typedef Prime::Num<std::uint64_t> Num64;

    template <>
    inline std::uint32_t Num64::BitsNum() const
    {
        return HighestBit(m_num);
    }

    template <>
    inline bool Num64::Bit(std::uint32_t index) const
    {
        return (m_num & (std::uint64_t(1) << index)) != 0;
    }

    template <>
    inline bool Num64::IsOdd() const
    {
        return m_num % 2 == 1;
    }

    template <>
    inline Num64 Num64::SquareRoot() const
    {
        return Num64(std::sqrt(m_num));
    }
}

#endif // TESTNUM

