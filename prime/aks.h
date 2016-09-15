#ifndef AKS_H
#define AKS_H

#include <utils.h>

namespace Prime
{
    template <typename T>
    bool IsPrimeAKS(const Num<T>& value)
    {
        if (value < 2) return false;
        if (value < 4) return true;
        if (value.Bit(0) == 0) return false; // Even number

        if (IsPerfectPower(value))
            return false;

        Num<T> r = FindR(value);
        if (r >= value)
            return true;

        for (Num<T> i = 2; i < r; ++i) {
            if (HaveGCD(i, value))
                return false;
        }

        return true;
    }
}

#endif // AKS_H
