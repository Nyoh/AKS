#ifndef BIGNUM
#define BIGNUM

#include <array>
#include <cstring>
#include <deque>
#include <vector>
#include <utility>

#include "num.h"
#include "utils.h"

namespace Prime
{
    template <typename T>
    struct BigNumData
    {
        static_assert(sizeof(T) * 2 <= sizeof(std::uint64_t), "");

        inline constexpr static decltype(sizeof(T)) ChunkSizeBits() {return sizeof(T) * 8;}
        inline constexpr static decltype(sizeof(T)) ChunkSizeBytes() {return sizeof(T);}
        typedef std::vector<T> DataType;
        DataType data;

        BigNumData()
            : data{0}
        {}

        explicit BigNumData(std::uint64_t value)
        {
            data.reserve(sizeof(std::uint64_t) / sizeof(T));
            do
            {
                data.push_back(static_cast<T>(value));
                value >>= ChunkSizeBits();
            } while (value);
        }

        bool operator ==(const BigNumData& rhs) const
        {
            return data == rhs.data;
        }

        bool operator !=(const BigNumData& rhs) const
        {
            return data != rhs.data;
        }

        BigNumData& operator <<=(int shift)
        {
            assert(shift >= 0);
            const std::size_t bigShift = shift / ChunkSizeBits();
            data.reserve(data.size() + bigShift + 1);
            data.resize(data.size() + bigShift);

            memmove(data.data() + bigShift, data.data(), (data.size() - bigShift) * ChunkSizeBytes());
            std::memset(data.data(), 0, bigShift * ChunkSizeBytes());

            shift %= ChunkSizeBits();
            if (!shift)
                return *this;

            data.push_back(0);
            std::uint64_t temp = 0;
            for (auto& chunk : data)
            {
                temp = (static_cast<std::uint64_t>(chunk) << shift) | (temp >> ChunkSizeBits());
                chunk = static_cast<T>(temp);
            }

            Shrink();
            return *this;
        }

        BigNumData& operator >>=(int shift)
        {
            assert(shift >= 0);
            const std::size_t bigShift = shift / ChunkSizeBits();
            if (bigShift >= data.size())
            {
                data.resize(1);
                data[0] = 0;
                return *this;
            }

            memmove(data.data(), data.data() + bigShift, (data.size() - bigShift) * ChunkSizeBytes());
            data.resize(data.size() - bigShift);

            shift %= ChunkSizeBits();
            if (!shift)
                return *this;

            T tail = 0;
            for (auto chunk = data.rbegin(); chunk != data.rend(); ++chunk)
            {
                T temp = tail << (ChunkSizeBits() - shift);
                temp |= (*chunk >> shift);
                tail = *chunk;
                *chunk = temp;
            }

            Shrink();
            return *this;
        }

        BigNumData& operator+=(const BigNumData& rhs)
        {
            Add(*this, rhs, *this);
            return *this;
        }


        BigNumData operator+(const BigNumData& rhs) const
        {
            BigNumData result;
            Add(*this, rhs, result);
            return result;
        }

        BigNumData operator+(const std::uint64_t& rhs) const
        {
            BigNumData result;
            Add(*this, BigNumData(rhs), result);
            return result;
        }

        BigNumData& operator-=(const BigNumData& rhs)
        {
            Sub(rhs);
            return *this;
        }


        BigNumData operator-(const BigNumData& rhs) const
        {
            BigNumData result = *this;
            result.Sub(rhs);
            return result;
        }

        BigNumData operator-(const std::uint64_t rhs) const
        {
            BigNumData result = *this;
            result.Sub(BigNumData(rhs));
            return result;
        }

        BigNumData& operator *=(const BigNumData& value)
        {
            const size_t alignedSize = NextPowerOf2(std::max(data.size(), value.data.size()));
            DataType rhs;
            rhs.reserve(alignedSize);
            rhs.assign(value.data.begin(), value.data.end());
            rhs.resize(alignedSize);

            data.resize(alignedSize);

            DataType result;
            result.resize(alignedSize * 2);

            Mult(data.begin(), data.end(), rhs.begin(), rhs.end(), result.begin());

            result.swap(data);
            Shrink();
            return *this;
        }

        BigNumData operator *(const BigNumData& value) const
        {
            const auto alignedSize = NextPowerOf2(std::max(data.size(), value.data.size()));

            DataType rhs;
            rhs.reserve(alignedSize);
            rhs.assign(value.data.begin(), value.data.end());
            rhs.resize(alignedSize);

            DataType lhs;
            lhs.reserve(alignedSize);
            lhs.assign(data.begin(), data.end());
            lhs.resize(alignedSize);

            BigNumData result;
            result.data.resize(alignedSize * 2);
            Mult(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), result.data.begin());

            result.Shrink();
            return result;
        }

        BigNumData& operator /=(const BigNumData& value)
        {
            Div(value);
            return *this;
        }

        BigNumData operator /(const BigNumData& value) const
        {
            BigNumData result = *this;
            result /= value;
            return result;
        }

        BigNumData& operator %=(const BigNumData& value)
        {
            *this = Div(value);
            return *this;
        }

        BigNumData operator %(const BigNumData& value) const
        {
            BigNumData result = *this;
            return result.Div(value);
        }

        bool operator <(const BigNumData& rhs) const
        {
            if (data.size() < rhs.data.size())
                return true;
            else if (data.size() > rhs.data.size())
                return false;
            else
            {
                for (std::int64_t i = data.size() - 1; i >= 0; i--)
                {
                    if (data[i] < rhs.data[i])
                        return true;
                    else if (data[i] > rhs.data[i])
                        return false;
                }
            }
            return false;
        }

        bool operator <=(const BigNumData& rhs) const
        {
            if (data.size() < rhs.data.size())
                return true;
            else if (data.size() > rhs.data.size())
                return false;
            else
            {
                for (auto i = data.size() - 1; i >= 0; i++)
                {
                    if (data[i] < rhs.data[i])
                        return true;
                    else if (data[i] > rhs.data[i])
                        return false;
                }
            }
            return true;
        }

        bool operator >(const BigNumData& rhs) const
        {
            return !(*this <= rhs);
        }

        bool operator >=(const BigNumData& rhs) const
        {
            return !(*this < rhs);
        }

        bool operator ==(const std::uint64_t& rhs) const
        {
            std::uint64_t value;
            if (!GetUint64(value))
                return false;

            return value == rhs;
        }

        bool operator !=(const std::uint64_t& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator <(const std::uint64_t& rhs) const
        {
            std::uint64_t value;
            if (!GetUint64(value))
                return false;

            return value < rhs;
        }

        bool operator <=(const std::uint64_t& rhs) const
        {
            std::uint64_t value;
            if (!GetUint64(value))
                return false;

            return value <= rhs;
        }

        bool operator >(const std::uint64_t& rhs) const
        {
            return !(*this <= rhs);
        }

        bool operator >=(const std::uint64_t& rhs) const
        {
            return !(*this < rhs);
        }

        BigNumData& operator++()
        {
            data[0]++;
            for (size_t i = 0; data[i] == 0; i++)
            {
                if (data.size() == i + 1)
                    data.push_back(0);
                data[i + 1] += 1;
            }

            return *this;
        }

        std::uint32_t BitsNum() const
        {
            return (data.size() - 1) * ChunkSizeBits() + HighestBit(data.back());
        }

    private:
        bool GetUint64(std::uint64_t& value) const
        {
            if (data.size() > sizeof(value) / ChunkSizeBytes())
                return false;

            value = 0;
            for (auto it = data.rbegin(); it != data.rend(); ++it)
            {
                value |= *it;
                value <<= ChunkSizeBits();
            }

            return true;
        }

        // Divides and returns the remainder
        BigNumData Div(const BigNumData& value)
        {
            if (value.data.size() == 1 && value.data[0] == 0)
                throw std::overflow_error("Division by zero.");

            if (*this < value)
            {
                BigNumData result = *this;
                data.resize(1);
                data[0] = 0;
                return result;
            }

            BigNumData result;
            BigNumData divisor = value;

            std::int64_t power = BitsNum() - value.BitsNum();
            divisor <<= power;
            while (power >= 0)
            {
                result <<= 1;
                if (*this >= divisor)
                {
                    if (power == 61)
                    {
                        assert(power == 61);
                    }

                    Sub(divisor);
                    result.data[0] |= 0x1;
                }
                divisor >>= 1;
                power--;
            }

            result.data.swap(data);

            return result;
        }

        void Sub(const BigNumData& value)
        {
            assert(*this >= value);
            bool over = false;
            for (size_t i = 0; i != value.data.size(); i++)
            {
                if (over)
                {
                    if (data[i] > 0)
                        over = false;
                    data[i]--;
                }
                if (data[i] < value.data[i])
                    over = true;
                data[i] -= value.data[i];
            }
            if (over)
                data[value.data.size()]--;
            Shrink();
        }

        static void Mult(const typename DataType::iterator& aBegin, const typename DataType::iterator& aEnd,
                  const typename DataType::iterator& bBegin, const typename DataType::iterator& bEnd,
                  const typename DataType::iterator& rBegin)
        {
            std::deque<std::array<typename DataType::iterator, 5>> queue;
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
                    Add(static_cast<std::uint64_t>(*alphaBegin) * static_cast<std::uint64_t>(*betaBegin), resultBegin);
                }
                else if (alphaEnd - alphaBegin == 2)
                {
                    Add(static_cast<std::uint64_t>(*alphaBegin) * static_cast<std::uint64_t>(*betaBegin), resultBegin);

                    Add(static_cast<std::uint64_t>(*(alphaBegin + 1)) * static_cast<std::uint64_t>(*betaBegin), resultBegin + 1);
                    Add(static_cast<std::uint64_t>(*alphaBegin) * static_cast<std::uint64_t>(*(betaBegin + 1)), resultBegin + 1);

                    Add(static_cast<std::uint64_t>(*(alphaBegin + 1)) * static_cast<std::uint64_t>(*(betaBegin + 1)), resultBegin + 2);
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

        static void Add(std::uint64_t value, const typename DataType::iterator& place)
        {
            auto cPlace = place;
            while (value)
            {
                value += *cPlace;
                *cPlace = static_cast<T>(value);
                value >>= ChunkSizeBits();

                ++cPlace;
            }
        }

        static void Add(const BigNumData& a, const BigNumData& b, BigNumData& result)
        {
            result.data.reserve(std::max(a.data.size(), b.data.size()) + 1);
            result.data.resize(std::max(a.data.size(), b.data.size()));

            const BigNumData& big = (a.data.size() > b.data.size() ? a : b);
            const BigNumData& small = (a.data.size() > b.data.size() ? b : a);

            bool over = false;
            for (size_t i = 0; i != big.data.size(); i++)
            {
                std::uint64_t chunkSum = big.data[i];
                if (i < small.data.size())
                    chunkSum += small.data[i];

                if (i >= result.data.size())
                    result.data.push_back(0);

                if (over)
                    chunkSum++;

                result.data[i] = static_cast<T>(chunkSum);
                over = ((chunkSum >> ChunkSizeBits()) != 0);

            }
            if (over)
                result.data.push_back(1);
        }

        void Shrink()
        {
            for (auto i = data.end() - 1; i >= data.begin(); --i)
                if (*i)
                {
                    data.erase(i + 1, data.end());
                    return;
                }

            // BigNum is empty
            data.resize(1);
        }
    };

    typedef Prime::Num<BigNumData<std::uint32_t>> BigNum;

    template <>
    inline std::uint32_t BigNum::BitsNum() const
    {
        return m_num.BitsNum();
    }

    template <>
    inline bool BigNum::Bit(std::uint32_t index) const
    {
        return (m_num.data[index / m_num.ChunkSizeBits()] & (std::uint64_t(1) << (index % m_num.ChunkSizeBits()))) != 0;
    }

    template <>
    inline bool BigNum::IsOdd() const
    {
        return m_num.data[0] % 2 == 1;
    }
}

#endif // BIGNUM

