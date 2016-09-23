#ifndef AKS_H
#define AKS_H

#include "aks_utils.h"
#include "polynomial.h"

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

        const auto polyLimit = std::sqrt(r) * value.BitsNum();
        for (std::uint64_t i = 1; i < polyLimit; i++)
        {
          Polynomial<Num<T>> lhs;
          lhs.SetDegree(1);
          lhs[1] = Num<T>(1);
          lhs[0] = Num<T>(i);
          lhs.PowMod(value, r, value);  // lhs = (x + i)^r

          Polynomial<Num<T>> rhs;
          rhs.SetDegree(1);
          rhs[1] = Num<T>(1);
          rhs.PowMod(value, r, value);  // lhs = x^r

          rhs[0] += Num<T>(i);
          rhs.Mod(r, value);

          if (rhs != lhs)
            return false;
        }

        return true;
    }
}

#endif // AKS_H
