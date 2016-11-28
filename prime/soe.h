#ifndef SOE_H
#define SOE_H

#include <cstdint>
#include <vector>

namespace Prime
{
    // Sieve of Eratosthenes
    class SoE
    {
    public:
        explicit SoE(const size_t size)
            : m_sieve(CreateSieve(size >= 3 ? size : 3))
        {
        }

        bool IsPrime(const std::uint64_t value)
        {
            return !m_sieve[value];
        }

    private:
        static std::vector<bool> CreateSieve(const size_t size)
        {
            std::vector<bool> sieve;
            sieve.resize(size + 1);
            sieve[0] = true;
            sieve[1] = true;
            const auto limit = std::sqrt(size);
            for (size_t i = 2; i < limit; i++)
            {
                if (sieve[i])
                    continue;

                for (size_t j = i * 2; j < sieve.size(); j += i)
                    sieve[j] = true;
            }

            return sieve;
        }

        const std::vector<bool> m_sieve; // false - prime, true - not
    };
}

#endif // SOE_H
