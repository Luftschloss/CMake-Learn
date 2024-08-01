#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include "config.h"

using namespace std;

#include "Thread/thread_test.h"
#include "Thread/future_test.h"
#include "Template/TemplateTest.h"
#include "C++11Test/test.h"
#include "LeetCode/Solutions.h"


#ifdef USE_MYMATH
#include "Math/MathFuc.h"
#else
#include <math.h>
#endif
#include <map>


int main(int argc, char* argv[])
{
	float number = 5.5;
	std::cout << "Number: " << std::setfill('0') << std::setw(3) << number << std::endl;

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
