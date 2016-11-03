#ifndef RESOURCES_H
#define RESOURCES_H

#include <chrono>
#include <functional>

namespace Prime
{
    struct ResourcesInfo
    {
        std::chrono::milliseconds time;
        size_t memory = 0;
    };

    bool TestResources(const std::function<bool()>& function, ResourcesInfo& info);
}

#endif // RESOURCES_H
