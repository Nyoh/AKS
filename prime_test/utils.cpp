#ifdef _WIN32
#define NOMINMAX
#include "windows.h"
#include "psapi.h"
#endif

#include <future>
#include <iostream>
#include <fstream>
#include <string>
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

    std::string GetFreeFileName(const std::string& name, const std::string& extention)
    {
        for(unsigned num = 1;; num++)
        {
            const std::string candidate = name + std::to_string(num) + extention;
            std::ifstream f(candidate.c_str());
            if (!f.good())
                return candidate;
        }
    }

    class Printer
    {
    public:
        explicit Printer(const std::string& methodName)
            : file(GetFreeFileName(methodName + '_', ".csv"), std::ofstream::out)
        {
            Print("Testing method: " + methodName + ", Is Prime, Time, Memory\n");
        }

        void Print(const std::string& text)
        {
            std::cout << text;
            file << text;
            file.flush();
        }

    private:
        std::ofstream file;
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

    void Test(const std::function<bool(const BigNum&)>& function, const std::string &methodName, const std::atomic<bool> &stop, const std::function<BigNum()>& feeder)
    {
        Printer printer(methodName);
        while(!stop)
        {
            const BigNum& number = feeder();
            printer.Print(number.ToString() + ", ");

            Prime::ResourcesInfo info;
            const bool result = Prime::TestResources([&function, &number](){return function(number);}, info);
            printer.Print(std::to_string(result) + ", " + std::to_string(info.time.count()) + ", " + std::to_string(info.memory) + "\n");
        }
    }

    std::function<BigNum()> CreateIntrementalFeeder(const BigNum& startPoint)
    {
        std::shared_ptr<BigNum> num = std::make_shared<BigNum>(startPoint);
        return std::function<BigNum()>([num](){
            BigNum result = *num;
            ++(*num);
            return result;
        });
    }
}
