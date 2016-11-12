#ifndef TRIAL_DIVISION_H
#define TRIAL_DIVISION_H

#include <deque>
#include <future>

namespace Prime
{
    template <typename T>
    bool IsPrimeTrialDivision(const Num<T>& value)
    {
        if (value < 2) return false;
        if (value < 4) return true;
        if (!value.IsOdd()) return false;

        std::deque<std::future<bool>> futures;
        const size_t threadsNum = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 4;
        const auto limit = value.SquareRoot();
        for (Num<T> i = Num<T>(3); i <= limit; i = i + 2)
        {
            if (futures.size() == threadsNum)
            {
                if (futures.front().get())
                    return false;
                futures.pop_front();
            }

            futures.push_back(std::async(std::launch::async, [i, &value](){
                return (value % i == 0);
            }));
        }

        for (auto& future : futures)
            if (future.get())
                return false;

        return true;
    }
}

#endif // TRIAL_DIVISION_H
