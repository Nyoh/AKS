#ifndef UTILS_H
#define UTILS_H

#include "num.h"

#include <cstdint>

namespace Prime
{
    // Calculates n^j mod q
    template <typename T>
    Num<T> PowerMod(const Num<T>& n, const Num<T>& j, const Num<T>& q)
    {
        if (j == Num<T>(0))
            return Num<T>(1);
        else if (j == Num<T>(1))
            return n;

        Num<T> result = n;

        for (auto i = j.BitsNum() - 1; i > 0; i--) {
            result = (result * result) % q;
            if (j.Bit(i) == true)
                result = (result * n) % q;
        }

        return result;
    }

    inline std::uint32_t HighestBit(std::uint64_t value)
    {
        std::uint32_t result = 0;
        while (value)
        {
            value >>= 1;
            result++;
        }

        return result;
    }

    inline std::uint64_t NextPowerOf2(std::uint64_t value)
    {
        std::uint64_t result = 1;
        result <<= HighestBit(value);
        if (value > result)
            result <<= 1;
        return result;
    }
}

#endif // UTILS_H

