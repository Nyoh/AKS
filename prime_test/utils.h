#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <atomic>
#include <chrono>
#include <functional>

#include "../prime/bignum.h"

namespace Prime
{
    struct ResourcesInfo
    {
        std::chrono::milliseconds time;
        size_t memory = 0;
    };

    bool TestResources(const std::function<bool()>& function, ResourcesInfo& info);
    void Test(const std::function<bool(const BigNum&)>& function, const std::string& methodName, const std::atomic<bool>& stop, const std::function<BigNum()>& feeder);

    std::function<BigNum()> CreateIntrementalFeeder(const BigNum& startPoint = BigNum(1));
}

#endif // TEST_UTILS_H
