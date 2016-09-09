#include "bigunsigned.h"

BigUnsigned::BigUnsigned(std::uint8_t x)
  : m_data(1, x)
{
}

BigUnsigned::BigUnsigned(std::uint16_t x)
  : m_data(1, x)
{
}

BigUnsigned::BigUnsigned(std::uint32_t x)
  : m_data(1, x)
{
}
