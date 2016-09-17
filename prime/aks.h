#ifndef AKS_H
#define AKS_H

#include "polynomial.h"
#include "aks_utils.h"

namespace Prime
{
    template <typename T>
    bool IsPrimeAKS(const Num<T>& value)
    {
        if (value < 2) return false;
        if (value < 4) return true;
        if (value.Bit(0) == false) return false; // Even number

        if (IsPerfectPower(value))
            return false;

        const auto r = FindR(value);
        if (value < r)
            return true;

        for (Num<T> i = Num<T>(2); i < r; ++i) {
            if (HaveGCD(i, value))
                return false;
        }



        return true;
    }
}

#endif // AKS_H
