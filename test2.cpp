#include <stdio.h>
#include <iostream>

#include "altivecmm.h" 

vector unsigned short test( vector unsigned short a, vector unsigned short b ) 
{
  const vector unsigned short mask = (vector unsigned short)vec_cmplt( a, b );

  if (vec_all_ge(a, b)) 
  {
    return (a);
  }

  return (mask);
}

int main(int argc, char *argv[])
{
	int values[] = {1337, 1748, 1, 2};

	altivecmm::Vec2_uint64 v1(1, 2);
	altivecmm::Vec2_uint64 v2(3, 4);

	altivecmm::Vec<int> v3(values);

	v3.print(std::cout) << std::endl;

	(v2 - v1).print(std::cout) << std::endl;
	v1.print(std::cout) << std::endl;

	return 0;
}
