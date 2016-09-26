#include <aks_utils.h>
#include <bignum.h>

#include "tests.h"

namespace
{
    bool TestShift()
    {
        for (std::size_t i = 1; i <= 1024; i *= 2)
        {
            Prime::BigNum a = Prime::BigNum(1);
            a <<= i;
            if (a.BitsNum() != i)
                return false;

            Prime::BigNum b = a;
            b >>= i;
            if (b != Prime::BigNum(1))
                return false;
        }

        return true;
    }

    bool TestSum()
    {
        for (std::size_t i = 32; i <= 1024; i *= 2)
        {
            Prime::BigNum a = Prime::BigNum(i);
            a <<= i;
            const auto sum = a + a;
            const auto prod = a * Prime::BigNum(2);
            if (sum != prod)
                return false;
        }

        return true;
    }

    bool TestDivision()
    {
        for (std::size_t i = 32; i <= 1024; i *= 2)
        {
            Prime::BigNumData<std::uint32_t> a = Prime::BigNumData<std::uint32_t>(i);
            a <<= i;
            const auto prod = a * a;
            const auto div = prod / a;
            if (div != a)
                return false;
        }

        return true;
    }

    bool TestMod()
    {
        for (std::size_t i = 17; i <= 100; i++)
        {
            Prime::BigNum a = Prime::Pow(Prime::BigNum(i), i);
            a += Prime::BigNum(i - 1);

            const auto mod = a % Prime::BigNum(i);
            if (mod != Prime::BigNum(i - 1))
                return false;
        }

        return true;
    }
}

bool TestBigNum()
{
    if (!TestShift())
        return false;

    if (!TestSum())
        return false;

    if (!TestDivision())
        return false;

    if (!TestMod())
        return false;

    return true;
}
