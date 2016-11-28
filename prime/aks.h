#ifndef AKS_H
#define AKS_H

#include <deque>
#include <future>

#include "aks_utils.h"
#include "polynomial.h"

namespace Prime
{
    template <typename T>
    bool IsPrimeAKS(const Num<T>& value)
    {
        if (value < 2) return false;
        if (value < 4) return true;
        if (!value.IsOdd()) return false;

        if (IsPerfectPower(value))
            return false;

        const auto r = FindR(value);
        if (value < r)
            return true;

        for (Num<T> i = Num<T>(2); i < r; ++i) {
            if (HaveGCD(i, value))
                return false;
        }

        std::deque<std::future<bool>> futures;
        const size_t threadsNum = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 4;
        const auto polyLimit = std::sqrt(r) * value.BitsNum();
        for (std::uint64_t i = 1; i < polyLimit; i++)
        {
            if (futures.size() == threadsNum)
            {
                if (!futures.front().get())
                    return false;
                futures.pop_front();
            }

            futures.push_back(std::async(std::launch::async, [i, &r, &value](){
                Polynomial<Num<T>> lhs;
                lhs.SetDegree(1);
                lhs[1] = Num<T>(1);
                lhs[0] = Num<T>(i);
                lhs.PowMod(value, r, value);  // lhs = (x + i)^r

                Polynomial<Num<T>> rhs;
                rhs.SetDegree(1);
                rhs[1] = Num<T>(1);
                rhs.PowMod(value, r, value);  // rhs = x^r

                rhs[0] += Num<T>(i);
                rhs.Mod(r, value);

                return (rhs == lhs);
            }));
        }

        for (auto& future : futures)
            if (!future.get())
                return false;

        return true;
    }
}

#endif // AKS_H
