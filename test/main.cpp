#include <aks.h>
#include <num64.h>
#include <aks_utils.h>

int main(int argc, char *argv[])
{
  Prime::Num64 b = Prime::Num64(64);

  auto c = b.BitsNum();
  auto d = Prime::IsPrimeAKS(b);

  return 0;
}
