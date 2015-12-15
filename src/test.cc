#include <iostream>

#include "smalljet.hh"

#define test(var) \
  std::cout <<"\033[36m"<< #var <<"\033[0m"<< " = " << var << std::endl;

int main(int argc, char** argv)
{
  using namespace small_jet;
  test( sizeof(jet_alg<antikt,1>) )
  test( sizeof(jet_alg<antikt,2>) )
  test( sizeof(jet_alg<antikt,3>) )
  test( sizeof(jet_alg<antikt,4>) )
  test( sizeof(jet_alg<antikt,8>) )
  test( sizeof(jet_alg<antikt,16>) )

  return 0;
}
