#ifndef BIGNUM
#define BIGNUM

#include <cstring>
#include <vector>

#include "num.h"
#include "utils.h"

namespace Prime
{
    struct BigNumData
    {
        const static auto ChunkSizeBits = sizeof(std::uint32_t) * 8;
        const static auto ChunkSizeBytes = sizeof(std::uint32_t);
        std::vector<std::uint32_t> data;

        BigNumData() {}

        explicit BigNumData(std::uint32_t value)
            : data{value}
        {
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
                chunk = static_cast<std::uint32_t>(temp);
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
                *chunk = static_cast<std::uint32_t>(temp >> ChunkSizeBits);
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

    private:
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

                if (i < result.data.size())
                    result.data.push_back(0);

                if (over)
                    chunkSum++;

                result.data[i] = static_cast<std::uint32_t>(chunkSum);
                over = ((chunkSum >> ChunkSizeBits) != 0);

            }
            if (over)
                result.data.push_back(1);
        }
    };

    typedef Prime::Num<BigNumData> BigNum;

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

