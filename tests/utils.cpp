#ifdef _WIN32
#include "windows.h"
#include "psapi.h"
#endif

#include <future>
#include <iostream>
#include <thread>

#include "utils.h"

namespace
{
    std::uint64_t GetMemoryUsed()
    {
#ifdef _WIN32
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        return pmc.PrivateUsage;
#else
        return 0;
#endif
    }

    std::uint64_t MemoryCheck(const std::atomic<bool>& stop)
    {
        const auto startMemory = GetMemoryUsed();
        auto maxMemory = startMemory;
        while(!stop)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            const auto currentMemory = GetMemoryUsed();
            if (currentMemory > maxMemory)
                maxMemory = currentMemory;
        }

        return maxMemory - startMemory;
    }

    void PartNumber(std::string& text, std::int64_t& num, unsigned base, unsigned size)
    {
        if (!num)
            return;

        int value = num % base;
        num /= base;

        std::string valueString = std::to_string(value);
        if (size > valueString.size())
            valueString = std::string(size - valueString.size(), '0') + valueString + ':';
        text = valueString + text;
    }

    std::string ToString(const std::chrono::milliseconds& duration)
    {
        std::int64_t num = duration.count();
        std::string result;
        PartNumber(result, num, 1000, 3);
        PartNumber(result, num, 60, 2);
        PartNumber(result, num, 60, 2);
        PartNumber(result, num, 24, 2);
        if (num)
            result = std::to_string(num) + ':' + result;

        return result;
    }

    class Printer
    {
    public:
        explicit Printer(const std::string& methodName)
        {
            Print("Testing method: " + methodName + ", Is Prime, Time, Memory\n");
        }

        void Print(const std::string& text)
        {
            std::cout << text;
        }
    };
}

namespace Prime
{
    bool TestResources(const std::function<bool()>& function, ResourcesInfo& info)
    {
        std::atomic<bool> memCheckStop{false};
        std::future<std::uint64_t> memUsed = std::async(std::launch::async, [&memCheckStop](){return MemoryCheck(memCheckStop);});

        const auto& startTime = std::chrono::high_resolution_clock::now();
        const bool result = function();
        const auto& endTime = std::chrono::high_resolution_clock::now();
        memCheckStop = true;

        info.time = std::chrono::duration_cast<decltype(info.time)>(endTime - startTime);
        info.memory = memUsed.get();

        return result;
    }

    void Test(const std::function<bool (const BigNum &)> &function, const std::string &methodName, const std::atomic<bool> &stop)
    {
        Printer printer(methodName);
        BigNum number(1);
        while(!stop)
        {
            printer.Print(number.ToString() + ", ");

            Prime::ResourcesInfo info;
            const bool result = Prime::TestResources([&function, &number](){return function(number);}, info);
            printer.Print(std::to_string(result) + ", " + ToString(info.time) + ", " + std::to_string(info.memory));

            ++number;
        }
    }
}
