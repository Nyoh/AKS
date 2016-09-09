#ifndef UTILS
#define UTILS

#include <num.h>

namespace Prime
{
    template <typename T>
    Num<T> Pow(const Num<T>& base, std::uint64_t power)
    {
        Num<T> result = 1;
        while (power)
        {
            if (power % 2)
                result *= base;

            result *= result;
            power /= 2; // divide by two
        }

        return result;
    }

    template <typename T>
    bool IsPerfectPower(const Num<T>& value)
    {
        Num<T> upperBound = value;
        Num<T> lowerBound = 1;
        const auto log2 = value.BitsNum();
        for (std::uint64_t i = 1; i < log2; i++)
        {
            while (upperBound > lowerBound)
            {
                const Num<T> candidateBase = (lowerBound + upperBound) >> 1; // Shifting is to divide by two
                const Num<T> candidate = Pow(candidateBase, i + 1);
                if (value == candidate)
                    return true;

                if (candidate > value)
                    upperBound = candidateBase;
                else
                    lowerBound = candidateBase;
            }
        }

        return false;
    }

    // Calculates n^j mod q
    template <typename T>
    Num<T> PowerMod(const Num<T>& n, const Num<T>& j, const Num<T>& q)
    {
        if (j == 0)
            return 1;

        Num<T> result = 1;
        for (auto i = j.BitsNum() - 1; i >= 0; i--) {
            result = (result * result) % q;
            if (j.Bit(i) == true)
                result = (result * n) % q;
        }

        return result;
    }

    template <typename T>
    Num<T> FindR(const Num<T>& value)
    {
        std::uint64_t log2Pow2 = value.BitsNum();
        log2Pow2 *= log2Pow2;

        Num<T> r = log2Pow2 + 1;
        while(true)
        {
            bool foundR = true;
            for (std::uint64_t j = 1; j <= log2Pow2; j++)
            {
                if (PowerMod(value, Num<T>(j), r) == 1)
                {
                    foundR = false;
                    break;
                }
            }

            if (foundR)
                return r;

            ++r;
        }
    }
}

#endif // UTILS

