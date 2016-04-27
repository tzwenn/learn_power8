#include <altivec.h>
#include <stdio.h>

int square(int a)
{
	return a * a;
}

int ret(int a)
{
	return a;
}

vector float buildVec(const float a, const float b, const float c, const float d)
{
	vector float returnme;
	float *returnme_ptr = (float *) &returnme;
	returnme_ptr[0] = a;
	returnme_ptr[1] = b;
	returnme_ptr[2] = c;
	returnme_ptr[3] = d;
	return returnme;
}

void printVec(const vector float v)
{
	float *pv = (float *)&v;
	printf("vector float <%f, %f, %f, %f>", pv[0], pv[1], pv[2], pv[3]);
}

vector unsigned short test( vector unsigned short a, vector unsigned short b ) 
{
  const vector unsigned short mask = (vector unsigned short)vec_cmplt( a, b );

  if (vec_all_ge(a, b)) 
  {
    return (a);
  }

  return (mask);
}

int ld_test()
{
	unsigned long data[4] = {0, 1, 2, 3};
	(void) vec_ld(2, data);
}

int main(int argc, char *argv[])
{
	vector float a = buildVec(1, 2, 3, 4);
	vector float b = buildVec(4, 3, 2, 1);

	vector float c = vec_add(a, b);

	ld_test();

	printVec(c);
	puts("");

	return 0;
}
