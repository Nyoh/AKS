#ifdef _WIN32
#include "windows.h"
#include "psapi.h"
#endif

#include <atomic>
#include <future>
#include <thread>

#include "resources.h"

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
}
