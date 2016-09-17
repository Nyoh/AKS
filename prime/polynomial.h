#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

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

    Polynomial<T>& operator +=(Polynomial<T>& rhs)
    {
      const auto degree = NextPowerOf2(std::max(GetDegree(), rhs.GetDegree()));
      SetDegree(degree);
      rhs.SetDegree(degree);
      Polynomial<T> result{degree};


    }

  private:
    std::vector<Num<T>> m_coefficients;

    void Shrink()
    {
      for (auto i = m_coefficients.end() - 1; i > m_coefficients.begin(); --i)
        if (i->IsNull())
        {
          m_coefficients.erase(i, m_coefficients.end());
          break;
        }
    }
  };
}

#endif // POLYNOMIAL_H

