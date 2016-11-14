#ifndef NUM
#define NUM

#include <algorithm>
#include <assert.h>
#include <cstdint>
#include <string>

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

        bool IsOdd() const
        {
            static_assert(sizeof(T) == -1, "Implement in specialized class");
            return false;
        }

        Num<T> SquareRoot() const
        {
            static_assert(sizeof(T) == -1, "Implement in specialized class");
            return Num<T>();
        }

        std::string ToString() const
        {
            static_assert(sizeof(T) == -1, "Implement in specialized class");
            return std::string();
        }

        static Num<T> Random(const Num<T>& low, const Num<T>& high)
        {
            static_assert(sizeof(T) == -1, "Implement in specialized class");
            return Num<T>();
        }

        bool FromString(const std::string& string)
        {
            static_assert(sizeof(T) == -1, "Implement in specialized class");
            return std::string();
        }

        bool IsNull() const
        {
            return m_num == 0;
        }

        void Swap(Num<T>& other)
        {
            std::swap(*this, other);
        }

        static Num<T> Pow(Num<T> base, std::uint64_t power)
        {
            Num<T> result = Num<T>(1);
            while (power)
            {
                if (power % 2)
                    result *= base;

                base *= base;
                power /= 2;
            }

            return result;
        }

        Num ()
            : m_num()
        {}

        template <typename M>
        explicit Num (const M& rhs)
            : m_num(rhs)
        {}

        explicit Num(int value)
            : m_num(static_cast<std::uint32_t>(value))
        {
            assert(value >= 0);
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
            return m_num > rhs.m_num;
        }

        bool operator >=(const Num<T>& rhs) const
        {
            return m_num >= rhs.m_num;
        }

        bool operator ==(const std::uint64_t& rhs) const
        {
            return m_num == rhs;
        }

        bool operator !=(const std::uint64_t& rhs) const
        {
            return m_num != rhs;
        }

        bool operator <(const std::uint64_t& rhs) const
        {
            return m_num < rhs;
        }

        bool operator <=(const std::uint64_t& rhs) const
        {
            return m_num <= rhs;
        }

        bool operator >(const std::uint64_t& rhs) const
        {
            return m_num > rhs;
        }

        bool operator >=(const std::uint64_t& rhs) const
        {
            return m_num >= rhs;
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

        Num<T> operator-(const Num<T>& rhs) const
        {
            assert(m_num >= rhs.m_num);
            return Num<T>(m_num - rhs.m_num);
        }

        Num<T> operator-(const std::uint64_t rhs) const
        {
            assert(m_num >= rhs);
            return Num<T>(m_num - rhs);
        }

        Num<T>& operator -=(const Num<T>& rhs)
        {
            assert(m_num >= rhs.m_num);
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

        Num<T> operator/(const Num<T>& rhs) const
        {
            return Num<T>(m_num / rhs.m_num);
        }

        Num<T>& operator /=(const Num<T>& rhs)
        {
            m_num /= rhs.m_num;
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

        Num<T>& operator <<=(int shift)
        {
            m_num <<= shift;
            return *this;
        }

        Num<T>& operator >>=(int shift)
        {
            m_num >>= shift;
            return *this;
        }

    private:
        T m_num;
    };
}

#endif // NUM

