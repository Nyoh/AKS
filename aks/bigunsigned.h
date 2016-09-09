#ifndef BIGUNSIGNED_H
#define BIGUNSIGNED_H

#include <cstdint>
#include <vector>

class BigUnsigned
{
public:
  BigUnsigned() {}
  BigUnsigned(std::uint8_t x);
  BigUnsigned(std::uint16_t x);
  BigUnsigned(std::uint32_t x);

protected:
  std::vector<std::uint32_t> m_data;
};

#endif // BIGUNSIGNED_H
