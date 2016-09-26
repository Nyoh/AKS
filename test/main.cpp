#include <iostream>

#include <aks.h>
#include <bignum.h>
#include <num64.h>

#include "tests.h"

int main(int argc, char *argv[])
{
    TestBigNum();

    for (int i = 5; i < 30; i++)
        std::cout << i << " " << IsPrimeAKS(Prime::BigNum(i)) << std::endl;

    return 0;
}
