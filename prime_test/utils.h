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
    void TestSync(const std::function<bool(const BigNum&)>& function, const std::string& methodName, const std::atomic<bool>& stop, const std::function<BigNum()>& feeder);
    void TestAsync(const std::function<bool(const BigNum&)>& function, const std::string& methodName);
    void TestFromFileAsync(const std::function<bool(const BigNum&)>& function, const std::string& methodName);

    void CreatePrimesFile();

    std::function<BigNum()> CreateIntrementalFeeder(const BigNum& startPoint = BigNum(1));
    std::function<BigNum()> CreateFileFeeder(const std::string& fileName, std::atomic<bool>& stop);
}

#endif // TEST_UTILS_H
