#include <aks.h>
#include <bigunsigned.h>
#include <num64.h>
#include <utils.h>

int main(int argc, char *argv[])
{
  BigUnsigned a((unsigned)5);
  Prime::Num64 b = Prime::Num64(64);

  auto c = b.BitsNum();
  auto d = Prime::IsPrimeAKS(b);

  return 0;
}
