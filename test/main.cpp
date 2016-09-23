#include <iostream>

#include <aks.h>
#include <num64.h>
#include <aks_utils.h>
#include <polynomial.h>

int main(int argc, char *argv[])
{
  Prime::Num64 b = Prime::Num64(64);

  auto c = b.BitsNum();
  auto d = Prime::IsPrimeAKS(b);

//  for (int i = 0; i < 20; i++)
  //{
    //std::cout << i << " " << IsPrimeAKS(Prime::Num64(i)) << std::endl;
  //}

  for (int i = 17; i <= 20; i++)
    std::cout << i << " " << IsPrimeAKS(Prime::Num64(i)) << std::endl;


  return 0;
}
