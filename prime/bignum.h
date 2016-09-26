#ifndef BIGNUM
#define BIGNUM

#include <array>
#include <cstring>
#include <deque>
#include <vector>

#include "num.h"
#include "utils.h"

namespace Prime
{
    template <typename T>
    struct BigNumData
    {
        static_assert(sizeof(T) * 2 <= sizeof(std::uint64_t), "");

        const static auto ChunkSizeBits = sizeof(T) * 8;
        const static auto ChunkSizeBytes = sizeof(T);
        typedef std::vector<T> DataType;
        DataType data;

        BigNumData() {}

        explicit BigNumData(std::uint64_t value)
        {
            data.reserve(sizeof(std::uint64_t) / sizeof(T));
            while (value)
            {
                data.push_back(static_cast<T>(value));
                value >>= ChunkSizeBits;
            }
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
            const std::size_t bigShift = shift / ChunkSizeBits;
            data.reserve(data.size() + bigShift + 1);
            data.resize(data.size() + bigShift);

            memmove(data.data() + bigShift, data.data(), (data.size() - bigShift) * ChunkSizeBytes);
            std::memset(data.data(), 0, bigShift * ChunkSizeBytes);

            shift %= ChunkSizeBits;
            if (!shift)
                return *this;

            data.push_back(0);
            std::uint64_t temp = 0;
            for (auto& chunk : data)
            {
                temp = (static_cast<std::uint64_t>(chunk) << shift) | (temp >> ChunkSizeBits);
                chunk = static_cast<T>(temp);
            }

            Shrink();
            return *this;
        }

        BigNumData& operator >>=(int shift)
        {
            assert(shift >= 0);
            const std::size_t bigShift = shift / ChunkSizeBits;
            if (bigShift >= data.size())
            {
                data.resize(1);
                data[0] = 0;
                return *this;
            }

            memmove(data.data(), data.data() + bigShift, (data.size() - bigShift) * ChunkSizeBytes);
            data.resize(data.size() - bigShift);

            shift %= ChunkSizeBits;
            if (!shift)
                return *this;

            std::uint64_t temp = 0;
            for (auto chunk = data.rbegin(); chunk != data.rend(); ++chunk)
            {
                temp = (static_cast<std::uint64_t>(*chunk) << (ChunkSizeBits - shift));
                *chunk = static_cast<T>(temp >> ChunkSizeBits);
                temp >>= ChunkSizeBits;
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

    private:
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
                value >>= ChunkSizeBits;

                ++cPlace;
            }
        }

        static void Add(const BigNumData& a, const BigNumData& b, BigNumData& result)
        {
            result.data.reserve(std::max(a.data.size(), b.data.size()) + 1);
            result.data.resize(result.data.capacity() - 1);

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
                over = ((chunkSum >> ChunkSizeBits) != 0);

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

    typedef Prime::Num<BigNumData<std::uint8_t>> BigNum;

    template <>
    std::uint32_t BigNum::BitsNum() const
    {
        return (m_num.data.size() - 1) * m_num.ChunkSizeBits + HighestBit(m_num.data.back());
    }

    template <>
    bool BigNum::Bit(std::uint32_t index) const
    {
        return (m_num.data[index / m_num.ChunkSizeBits] & (std::uint64_t(1) << (index % m_num.ChunkSizeBits))) != 0;
    }
}

#endif // BIGNUM

