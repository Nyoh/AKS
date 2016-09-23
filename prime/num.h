#ifndef NUM
#define NUM

#include <algorithm>
#include <assert.h>
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

    Num<T>& operator <<=(int /*shift*/)
    {
      static_assert(sizeof(T) == -1, "Implement in specialized class");
      return *this;
    }

    Num<T>& operator >>=(int /*shift*/)
    {
      static_assert(sizeof(T) == -1, "Implement in specialized class");
      return *this;
    }

    bool IsNull() const
    {
      return m_num == 0;
    }

    void Swap(Num<T>& other)
    {
      std::swap(*this, other);
    }

    bool IsOdd() const
    {
      return m_num % 2 == 1;
    }

    Num ()
      : m_num()
    {}

    explicit Num(std::uint32_t value)
      : m_num(value)
    {
    }

    explicit Num(std::uint64_t value)
      : m_num(value)
    {
    }

    explicit Num(int value)
      : m_num(static_cast<std::uint32_t>(value))
    {
      assert(value >= 0);
    }

    bool operator ==(int rhs) const
    {
      assert(rhs >= 0);
      return m_num == static_cast<unsigned int>(rhs);
    }

    bool operator >(int rhs) const
    {
      assert(rhs >= 0);
      return m_num > static_cast<unsigned int>(rhs);
    }

    bool operator <(const std::uint64_t rhs) const
    {
      return m_num < rhs;
    }

    Num<T> operator%(const std::uint64_t rhs) const
    {
        return Num<T>(m_num % rhs);
    }

    bool operator ==(const Num<T>& rhs) const
    {
      return m_num == rhs.m_num;
    }

    bool operator !=(const Num<T>& rhs) const
    {
      return m_num != rhs.m_num;
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

    Num<T> operator+(const std::uint64_t rhs) const
    {
      return Num<T>(m_num + rhs);
    }

    Num<T>& operator +=(const Num<T>& rhs)
    {
      m_num += rhs.m_num;
      return *this;
    }

    Num<T> operator-(const std::uint64_t rhs) const
    {
      assert(m_num > rhs);
      return Num<T>(m_num - rhs);
    }

    Num<T> operator-(const Num<T>& rhs) const
    {
      assert(m_num > rhs.m_num);
      return Num<T>(m_num - rhs.m_num);
    }

    Num<T>& operator -=(const Num<T>& rhs)
    {
      assert(m_num > rhs.m_num);
      m_num -= rhs.m_num;
      return *this;
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

    Num<T>& operator %=(const Num<T>& rhs)
    {
      m_num %= rhs.m_num;
      return *this;
    }

    Num<T> operator >>(int shift) const
    {
      return Num<T>(*this) >>= shift;
    }

    Num<T> operator <<(int shift) const
    {
      return Num<T>(*this) <<= shift;
    }

  private:
    T m_num;
  };
}

#endif // NUM

