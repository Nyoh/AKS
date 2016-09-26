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
            : m_sieve(CreateSieve(size >= 3 ? size : 3))
        {
        }

        bool IsPrime(const std::uint64_t value)
        {
            return !m_sieve[valie];
        }

    private:
        static std::vector<bool> CreateSieve(const size_t size)
        {
            std::vector<bool> sieve;
            sieve.resize(size);
            sieve[0] = true;
            sieve[1] = true;
        }

        const std::vector<bool> m_sieve; // false - prime, true - not
    };
}

#endif // SOE_H
