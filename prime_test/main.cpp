#include <iostream>

#include <aks.h>
#include <bignum.h>
#include <miller_rabin.h>
#include <soe.h>

#include "utils.h"
#include "tests.h"

int main(int argc, char *argv[])
{
    TestBigNum();

    std::atomic<bool> stop{false};
    std::thread thread([&stop](){
        Prime::Test([](const Prime::BigNum& num){return Prime::IsPrimeMillerRabin(num);}, "AKS", stop);}
    );

    std::cin.ignore();
    stop = true;
    if (thread.joinable())
        thread.join();

    return 0;
}
