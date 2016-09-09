#include <aks.h>
#include <bigunsigned.h>
#include <testnum.h>
#include <utils.h>

int main(int argc, char *argv[])
{
  BigUnsigned a((unsigned)5);
  Prime::TestNum b = 64;

  auto c = b.BitsNum();
  auto d = Prime::IsPrimeAKS(b);

  return 0;
}
