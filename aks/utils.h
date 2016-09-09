#ifndef UTILS
#define UTILS

#include <num.h>

namespace AKSN
{
  template <typename T>
  Num<T> Pow(const Num<T> base, std::uint64_t power)
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
  bool IsPerfectPower(const Num<T> value)
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
}

#endif // UTILS

