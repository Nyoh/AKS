#include <aks.h>
#include <num64.h>
#include <aks_utils.h>
#include <polynomial.h>

int main(int argc, char *argv[])
{
  Prime::Num64 b = Prime::Num64(64);

  auto c = b.BitsNum();
  auto d = Prime::IsPrimeAKS(b);

  Prime::Polynomial<Prime::Num64> a1;
  a1.SetDegree(1);
  a1.m_coefficients[0] = Prime::Num64(1);
  a1.m_coefficients[1] = Prime::Num64(3);
  Prime::Polynomial<Prime::Num64> a2 = a1;
  a2 *= a1;

  return 0;
}
