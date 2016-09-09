#ifndef AKS_H
#define AKS_H

#include <utils.h>

namespace Prime
{
  template <typename T>
  bool IsPrimeAKS(const Num<T>& value)
  {
    if (IsPerfectPower(value))
      return false;

    Num<T> r = FindR(value);

    return true;
  }
}

#endif // AKS_H
