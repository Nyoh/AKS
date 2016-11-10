#ifndef MILLER_RABIN_H
#define MILLER_RABIN_H

#include "num.h"

namespace Prime
{
    template <typename T>
    bool IsPrimeMillerRabin(const Num<T>& value, std::uint32_t rounds = 0)
    {
        if (value < 2) return false;
        if (value < 4) return true;
        if (!value.IsOdd()) return false;

        if (!rounds)
            rounds = value.BitsNum();

        std::uint64_t S = 0;
        while (value.Bit(S) == false)
            S++;

        Num<T> t = (value >> S);

        for (std::uint32_t round = 0; round != rounds; round++)
        {

        }


        return true;
    }
}

#endif // MILLER_RABIN_H
