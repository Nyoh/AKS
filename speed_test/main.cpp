#include <iostream>

#include <aks.h>
#include <bignum.h>
#include <soe.h>

#include "tests.h"

int main(int argc, char *argv[])
{
    TestBigNum();

    const unsigned num = 100;

    Prime::SoE soe = Prime::SoE(num);
    for (int i = 1; i < num; i++)
    {
        bool aksR = IsPrimeAKS(Prime::BigNum(i));
        bool soeR = soe.IsPrime(i);

        std::cout << i << " " << aksR << ' ' << soeR << ' ' << (aksR == soeR ? 'V' : 'X') << std::endl;

    }

    return 0;
}
