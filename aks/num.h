#ifndef NUM
#define NUM

#include <cstdint>

namespace AKSN
{
  // An interface class for big number implementations
  template <typename T> class Num
  {
  public:
    // Return the number of bits in the number
    // It is used in algorithm as a log2
    std::uint64_t BitsNum() const
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

    bool operator >(const Num<T>& rhs) const
    {
      return m_num > rhs.m_num;
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

