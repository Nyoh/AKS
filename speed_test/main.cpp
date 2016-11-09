#include <iostream>

#include <aks.h>
#include <bignum.h>
#include <soe.h>

#include "../tests/utils.h"
#include "tests.h"

int main(int argc, char *argv[])
{
    std::atomic<bool> stop{false};
    Prime::Test([](const Prime::BigNum& num){return Prime::IsPrimeAKS(num);}, "AKS", stop);

    TestBigNum();

    Prime::ResourcesInfo info;
    bool a = Prime::TestResources([](){return Prime::IsPrimeAKS(Prime::BigNum(17));}, info);

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
