#ifndef MILLER_RABIN_H
#define MILLER_RABIN_H

#include "num.h"
#include "utils.h"

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

        std::int64_t S = 0;
        while (value.Bit(S) == false)
            S++;

        Num<T> t = (value >> S);

        for (std::uint32_t round = 0; round != rounds; round++)
        {
            Num<T> a = Num<T>::Random(Num<T>(2), value - 2);
            Num<T> X = PowerMod(a, t, value);
            if (X == 1 || X == value - 1)
                continue;

            bool continueRoundLoop = false;
            for (std::int64_t sRound = 1; sRound < S; sRound++)
            {
                X = PowerMod(X, Num<T>(2), value);
                if (X == 1)
                    return true;
                else if (X == value - 1)
                {
                    continueRoundLoop = true;
                    break;
                }
            }
            if (!continueRoundLoop)
                return false;
        }

        return true;
    }
}

#endif // MILLER_RABIN_H
