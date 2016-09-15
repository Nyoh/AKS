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
      static_assert(sizeof(T) == -1, "Implement in specialized class");
      return 0;
    }

    // Returns bit with the given index
    bool Bit(std::uint32_t /*index*/) const
    {
      static_assert(sizeof(T) == -1, "Implement in specialized class");
      return false;
    }

    Num () {}

    explicit Num(std::uint64_t value)
      : m_num(value)
    {
    }

    operator bool() const
    {
      return m_num != 0;
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
      return Num<T>(m_num + rhs.m_num);
    }

    Num<T> operator-(const Num<T>& rhs) const
    {
      return Num<T>(m_num - rhs.m_num);
    }

    Num<T> operator*(const Num<T>& rhs) const
    {
      return Num<T>(m_num * rhs.m_num);
    }

    Num<T>& operator *=(const Num<T>& rhs)
    {
      m_num *= rhs.m_num;
      return *this;
    }

    Num<T> operator%(const Num<T>& rhs) const
    {
        return Num<T>(m_num % rhs.m_num);
    }

    Num<T>& operator >>=(int shift)
    {
      m_num >>= shift;
      return *this;
    }

    Num<T> operator >>(int shift) const
    {
      return Num<T>(*this) >>= shift;
    }

  private:
    T m_num;
  };
}

#endif // NUM

