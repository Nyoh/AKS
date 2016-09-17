#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <algorithm>
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
      const auto degree = NextPowerOf2(std::max(GetDegree(), rhs.GetDegree()));
      SetDegree(degree);
      rhs.SetDegree(degree);
      Polynomial<T> result{degree * 2};
      Mult(m_coefficients.begin(), m_coefficients.end(),
           rhs.m_coefficients.begin(), rhs.m_coefficients.end(),
           result.m_coefficients.begin());

      result.m_coefficients.swap(m_coefficients);
      Shrink();
      return *this;
    }

    // Mod the polynomial by x^r - 1, n
    void Mod(const std::uint64_t r, const T& n)
    {
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
    }

 // private:
    std::vector<T> m_coefficients;

    void Shrink()
    {
      for (auto i = m_coefficients.end() - 1; i > m_coefficients.begin(); --i)
        if (!i->IsNull())
        {
          m_coefficients.erase(i + 1, m_coefficients.end());
          break;
        }
      SetDegree(m_coefficients.size() - 1);
    }

    void Mult(const typename std::vector<T>::iterator& aBegin, const typename std::vector<T>::iterator& aEnd,
              const typename std::vector<T>::iterator& bBegin, const  typename std::vector<T>::iterator& bEnd,
              const typename std::vector<T>::iterator& rBegin)
    {
        assert(aEnd - aBegin == bEnd - bBegin);
        if (aEnd - aBegin == 1)
        {
          *rBegin += *aBegin * *bBegin;
          return;
        }
        else if (aEnd - aBegin == 2)
        {
          *rBegin += *aBegin * *bBegin;
          *(rBegin + 1) += *(aBegin + 1) * *bBegin + *aBegin * *(bBegin + 1);
          *(rBegin + 2) += *(aBegin + 1) * *(bBegin + 1);
        }
        else
        {
          Mult(aBegin, aBegin + (aEnd - aBegin) / 2, bBegin, bBegin + (bEnd - bBegin) / 2, rBegin); //A0 + B0
          Mult(aBegin, aBegin + (aEnd - aBegin) / 2, bBegin + (bEnd - bBegin) / 2, bEnd, rBegin  + (bEnd - bBegin) / 2); //A0 + B1
          Mult(aBegin + (aEnd - aBegin) / 2, aEnd, bBegin, bBegin + (bEnd - bBegin) / 2, rBegin  + (bEnd - bBegin) / 2); //A1 + B0
          Mult(aBegin + (aEnd - aBegin) / 2, aEnd, bBegin + (bEnd - bBegin) / 2, bEnd, rBegin  + (bEnd - bBegin)); //A1 + B1
        }
    }
  };
}

#endif // POLYNOMIAL_H

