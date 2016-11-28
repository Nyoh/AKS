#include <future>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include <miller_rabin.h>
#include <trial_division.h>

#include "utils.h"

namespace
{
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
            Print("Testing method: " + methodName + "\nN, log(N), Is Prime, Duration\n");
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
    bool TestResources(const std::function<bool()>& function, std::chrono::milliseconds& duration)
    {
        const auto& startTime = std::chrono::high_resolution_clock::now();
        const bool result = function();
        const auto& endTime = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        return result;
    }

    void TestAsync(const std::function<bool(const BigNum&)>& function, const std::string &methodName)
    {
        std::atomic<bool> stop{false};
        std::thread thread([&function, &methodName, &stop](){
            Prime::TestSync(function, methodName, stop, CreateIntrementalFeeder());}
        );

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        stop = true;
        if (thread.joinable())
            thread.join();
    }

    void TestFromFileAsync(const std::function<bool(const BigNum&)>& function, const std::string& methodName)
    {
        std::string fileName;
        std::cout << "Enter file's name: ";
        std::cin >> fileName;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::atomic<bool> stop{false};
        std::thread thread([&function, &methodName, &stop, &fileName](){
            Prime::TestSync(function, methodName, stop, CreateFileFeeder(fileName, stop));}
        );

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        stop = true;
        if (thread.joinable())
            thread.join();
    }

    void TestSync(const std::function<bool(const BigNum&)>& function, const std::string &methodName, const std::atomic<bool> &stop, const std::function<BigNum()>& feeder)
    {
        Printer printer(methodName);
        while(!stop)
        {
            const BigNum& number = feeder();
            printer.Print(number.ToString() + ", " + std::to_string(number.BitsNum()) + ", ");

            std::chrono::milliseconds duration;
            const bool result = Prime::TestResources([&function, &number](){return function(number);}, duration);
            printer.Print(std::to_string(result) + ", " + std::to_string(duration.count()) + "\n");
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

    void CreatePrimesFile()
    {
        std::ofstream file("primes.txt");
        std::atomic<bool> stop{false};
        std::thread thread([&stop, &file](){
            BigNum num(7);
            while(!stop)
            {
                if (Prime::IsPrimeMillerRabin(num))// && Prime::IsPrimeTrialDivision(num))
                {
                    std::cout << num.ToString() << std::endl;
                    file << num.ToString() << std::endl;

                        num <<= 1;
                    }
                    else
                ++num;
            }
        });

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        stop = true;
        if (thread.joinable())
            thread.join();
    }
}
