#include <iostream>
#include <array>

#define test(var) \
  std::cout <<"\033[36m"<< #var <<"\033[0m"<< " = " << var << std::endl;

struct cci { char c1,c2; int i1; };
struct cic { char c1; int i1; char c2; };
struct icc { int i1; char c1,c2; };

struct cciA { std::array<char,2> c1,c2; std::array<int,2> i1; };
struct cicA { std::array<char,2> c1; std::array<int,2> i1; std::array<char,2> c2; };

int main(int argc, char** argv)
{
  test( sizeof(cci) )
  test( sizeof(cic) )
  test( sizeof(icc) )

  test( sizeof(std::array<char,2>) )
  test( sizeof(std::array<int,2>) )

  test( sizeof(cciA) )
  test( sizeof(cicA) )

  return 0;
}

