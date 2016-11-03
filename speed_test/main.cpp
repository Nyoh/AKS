#include <iostream>

#include <aks.h>
#include <bignum.h>
#include <soe.h>

#include "resources.h"
#include "tests.h"

int main(int argc, char *argv[])
{
    TestBigNum();

    Prime::ResourcesInfo info;
    //std::function<bool()> checkMemFun(Prime::IsPrimeAKS<std::uint32_t>, Prime::BigNum(100));
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
