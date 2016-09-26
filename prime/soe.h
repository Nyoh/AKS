#ifndef SOE_H
#define SOE_H

#include <cstdint>
#include <vector>

namespace Prime
{
    // Sieve of Eratosthenes
    class SoE
    {
        explicit SoE(const size_t size)
            : m_sieve(CreateSieve(size))
        {
        }

        bool IsPrime(const std::uint64_t value)
        {

            return true;
        }

    private:
        static std::vector<bool> CreateSieve(const size_t size)
        {
            std::vector<bool> sieve;
            sieve.resize(size);

        }

        const std::vector<bool> m_sieve;
    };
}

#endif // SOE_H
