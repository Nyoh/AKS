#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <algorithm>
#include <array>
#include <deque>
#include <vector>

#include "num.h"
#include "utils.h"

namespace Prime
{
    template <typename T>
    class Polynomial
    {
    public:
        Polynomial()
            : m_coefficients{0}
        {}

        explicit Polynomial(const std::uint64_t degree)
        {
            SetDegree(degree);
        }

        std::uint64_t GetDegree() const
        {
            return m_coefficients.size() - 1;
        }

        void SetDegree(const std::uint64_t degree)
        {
            m_coefficients.resize(degree + 1);
        }

        Polynomial<T>& operator *=(Polynomial<T>& rhs)
        {
            const auto alignedSize = NextPowerOf2(std::max(m_coefficients.size(), rhs.m_coefficients.size()));
            m_coefficients.resize(alignedSize);
            rhs.m_coefficients.resize(alignedSize);
            Polynomial<T> result{alignedSize * 2};
            Mult(m_coefficients.begin(), m_coefficients.end(),
                 rhs.m_coefficients.begin(), rhs.m_coefficients.end(),
                 result.m_coefficients.begin());

            result.m_coefficients.swap(m_coefficients);
            Shrink();
            rhs.Shrink();
            return *this;
        }

        bool operator ==(Polynomial<T>& rhs)
        {
            return m_coefficients == rhs.m_coefficients;
        }

        // Mod the polynomial by x^r - 1, n
        void Mod(const std::uint64_t r, const T& n)
        {
            if (m_coefficients.empty())
                return;

            if (r <= GetDegree())
            {
                for (std::uint64_t i = GetDegree(); i >= r; i--)
                {
                    m_coefficients[i - r] += m_coefficients[i];
                }
                SetDegree(r - 1);
            }
            for (auto& coeff : m_coefficients)
                coeff %= n;

            Shrink();
        }

        // Pows and mods by x^r - 1, n
        void PowMod(T power, const std::uint64_t r, const T& n)
        {
            Polynomial<T> result{1};
            result[0] = T(1);

            while (!power.IsNull())
            {
                if (power.IsOdd())
                {
                    result *= *this;
                    result.Mod(r, n);
                }

                if (power > 1)
                    *this *= *this;

                power >>= 1; // divide by two
            }

            result.m_coefficients.swap(m_coefficients);
        }

        T& operator[](const std::uint64_t index)
        {
            return m_coefficients[index];
        }

    private:
        std::vector<T> m_coefficients;

        void Shrink()
        {
            if (m_coefficients.empty())
                return;

            for (auto i = m_coefficients.end() - 1; i > m_coefficients.begin(); --i)
                if (!i->IsNull())
                {
                    m_coefficients.erase(i + 1, m_coefficients.end());
                    return;
                }

            if (m_coefficients.begin()->IsNull())
                m_coefficients.clear();
            else
                m_coefficients.resize(1);
        }

        void Mult(const typename std::vector<T>::iterator& aBegin, const typename std::vector<T>::iterator& aEnd,
                  const typename std::vector<T>::iterator& bBegin, const typename std::vector<T>::iterator& bEnd,
                  const typename std::vector<T>::iterator& rBegin)
        {
            std::deque<std::array<typename std::vector<T>::iterator, 5>> queue;
            queue.push_back({aBegin, aEnd, bBegin, bEnd, rBegin});

            while (!queue.empty())
            {
                const auto& params = queue.front();
                const auto& alphaBegin = params[0];
                const auto& alphaEnd = params[1];
                const auto& betaBegin = params[2];
                const auto& betaEnd = params[3];
                const auto& resultBegin = params[4];

                assert(alphaEnd - alphaBegin == betaEnd - betaBegin);
                if (alphaEnd - alphaBegin == 1)
                {
                    *resultBegin += *alphaBegin * *betaBegin;
                }
                else if (alphaEnd - alphaBegin == 2)
                {
                    *resultBegin += *alphaBegin * *betaBegin;
                    *(resultBegin + 1) += *(alphaBegin + 1) * *betaBegin + *alphaBegin * *(betaBegin + 1);
                    *(resultBegin + 2) += *(alphaBegin + 1) * *(betaBegin + 1);
                }
                else
                {
                    const auto half = (alphaEnd - alphaBegin) / 2; // equal to (betaEnd - betaBegin) / 2

                    queue.push_back({alphaBegin, alphaBegin + half, betaBegin, betaBegin + half, resultBegin}); //A0 + B0
                    queue.push_back({alphaBegin, alphaBegin + half, betaBegin + half, betaEnd, resultBegin + half}); //A0 + B1
                    queue.push_back({alphaBegin + half, alphaEnd, betaBegin, betaBegin + half, resultBegin + half}); //A1 + B0
                    queue.push_back({alphaBegin + half, alphaEnd, betaBegin + half, betaEnd, resultBegin + half * 2}); //A1 + B1
                }

                queue.pop_front();
            }
        }
    };
}

#endif // POLYNOMIAL_H

