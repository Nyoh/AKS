#include <bignum.h>

#include "tests.h"

namespace
{
  bool TestShift()
  {
    for (int i = 0; i <= 256; i++)
    {
      Prime::BigNum a = Prime::BigNum(1);
      a <<= i;
      if (a.BitsNum() != i)
        return false;
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
