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
        for (std::size_t i = 1; i <= 1024; i *= 2)
        {

        }

        return true;
    }
}

bool TestBigNum()
{
    if (!TestShift())
        return false;

    return true;
}
