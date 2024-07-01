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

class SSS
{
public:
	SSS(const std::string& n)
	{
		Name = n;
	}
	std::string Name;
};


int main(int argc, char* argv[])
{
	std::map<std::string, SSS*> map{};
	{
		std::vector<SSS*> list{};
		list.push_back(new SSS("s1"));
		list.push_back(new SSS("s2"));
		list.push_back(new SSS("s3"));
		list.push_back(new SSS("s3"));

		auto listItem2 = list[2];
		for (auto s : list)
		{
			if (map.count(s->Name) == 0)
			{
				map[s->Name] = std::move(s);
			}
		}

		list.clear();
	}

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
