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

    class FileFeeder
    {
    public:
        FileFeeder(const std::string& fileName, std::atomic<bool>& stop)
            : m_stop(stop)
        {
            std::ifstream file(fileName);
            if (!file.good())
            {
                std::cout << fileName << " not found." << std::endl;
                m_stop = true;
                return;
            }

            while (!file.eof())
            {
                std::string stringNum;
                Prime::BigNum num;
                file >> stringNum;
                if (!num.FromString(stringNum))
                {
                    std::cout << "Failed to parse number '" << stringNum << "'." << std::endl;
                    continue;
                }
                m_nums.push_back(num);
            }

            if (!m_nums.empty())
                m_current = m_nums.begin();
            else
                m_stop = true;
        }

        Prime::BigNum GetNum()
        {
            const auto& result = *m_current;

            if (++m_current == m_nums.end())
                m_stop = true;

            return result;
        }

    private:
        std::vector<Prime::BigNum> m_nums;
        std::vector<Prime::BigNum>::const_iterator m_current;
        std::atomic<bool>& m_stop;
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
        if (!num->IsOdd())
            ++(*num);

        return std::function<BigNum()>([num](){
            BigNum result = *num;
            *num = *num + 2;
            return result;
        });
    }

    std::function<BigNum()> CreateFileFeeder(const std::string& fileName, std::atomic<bool>& stop)
    {
        std::shared_ptr<FileFeeder> feeder = std::make_shared<FileFeeder>(fileName, stop);
        return std::function<BigNum()>([feeder](){
            return feeder->GetNum();
        });
    }
}
