#ifndef NUM
#define NUM

#include <cstdint>

namespace Prime
{
  // An interface class for big number implementations
  template <typename T> class Num
  {
  public:
    // Returns the number of bits in the number
    // It is used in algorithm as a log2
    std::uint32_t BitsNum() const
    {
      static_assert(false, "Implement in specialized class");
    }

    // Returns bit with the given index
    bool Bit(std::uint32_t index) const
    {
      static_assert(false, "Implement in specialized class");
    }

    Num () {}

    Num(std::uint64_t value)
      : m_num(value)
    {
    }

    bool operator ==(const Num<T>& rhs) const
    {
      return m_num == rhs.m_num;
    }

    bool operator <(const Num<T>& rhs) const
    {
      return m_num < rhs.m_num;
    }

    bool operator <=(const Num<T>& rhs) const
    {
      return m_num <= rhs.m_num;
    }

    bool operator >(const Num<T>& rhs) const
    {
      return rhs.m_num <= m_num;
    }

    bool operator >=(const Num<T>& rhs) const
    {
      return rhs.m_num < m_num;
    }

    Num<T>& operator++()
    {
        ++m_num;
        return *this;
    }

    Num<T> operator+(const Num<T>& rhs) const
    {
      return m_num + rhs.m_num;
    }

    Num<T> operator*(const Num<T>& rhs) const
    {
      return m_num * rhs.m_num;
    }

    Num<T>& operator *=(const Num<T>& rhs)
    {
      m_num *= rhs.m_num;
      return *this;
    }

    Num<T> operator%(const Num<T>& rhs) const
    {
        return m_num % rhs.m_num;
    }

    Num<T>& operator >>=(unsigned int shift)
    {
      m_num >>= shift;
      return *this;
    }

    Num<T> operator >>(unsigned int shift) const
    {
      return Num<T>(*this) >>= shift;
    }

  private:
    T m_num;
  };
}

#endif // NUM

