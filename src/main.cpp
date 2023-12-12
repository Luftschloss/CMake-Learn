#include <stdio.h>
#include <stdlib.h>
#include "config.h"

using namespace std;

#include "Thread/background_task.h"
#include "Thread/future_test.h"
#include "Template/TemplateTest.h"
#include "C++11Test/test.h"


#ifdef USE_MYMATH
#include "Math/MathFuc.h"
#else
#include <math.h>
#endif

int main(int argc, char* argv[])
{
	CPP11::Test01();

	//TemplateTest01();
	future_test::test2();
	ThreadTest();

	if (argc < 3) {
		printf("Usage: %s base exponent \n", argv[0]);
		return 1;
	}
	double base = atof(argv[1]);
	int exponent = atoi(argv[2]);
#ifdef USE_MYMATH
	printf("Now we use our own Math library. \n");
	double result = power(base, exponent);
#else
	printf("Now we use the standard library. \n");
	double result = pow(base, exponent);
#endif
	printf("%g ^ %d is %g\n", base, exponent, result);

	return 0;
}
