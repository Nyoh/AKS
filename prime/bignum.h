#ifndef BIGNUM
#define BIGNUM

#include <vector>

#include "num.h"
#include "utils.h"

namespace Prime
{
  struct BigNumData
  {
    const static auto ChunkSizeBits = sizeof(std::uint32_t);
    const static auto ChunkSizeBytes = sizeof(std::uint32_t) / sizeof(char);
    std::vector<std::uint32_t> data;

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

  template <>
  BigNum& BigNum::operator <<=(int shift)
  {
    assert(shift >= 0);
    const int bigShift = shift / m_num.ChunkSizeBits;
    m_num.data.reserve(m_num.data.size() + bigShift + 1);
    m_num.data.resize(m_num.data.size() + bigShift);
    memmove(m_num.data.data() + bigShift * m_num.ChunkSizeBytes, m_num.data.data(), (m_num.data.size() - bigShift) * m_num.ChunkSizeBytes);

    shift %= m_num.ChunkSizeBits;
    if (!shift)
      return *this;

    m_num.data.push_back(0);
    std::uint64_t temp = 0;
    for (auto& chunk : m_num.data)
    {
      temp = (static_cast<std::uint64_t>(chunk) << shift) | (temp >> m_num.ChunkSizeBits);
      chunk = static_cast<std::uint32_t>(temp);
    }

    m_num.Shrink();
    return *this;
  }
}

#endif // BIGNUM

