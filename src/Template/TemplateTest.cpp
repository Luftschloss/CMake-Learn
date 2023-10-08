#include <iostream>
#include "TemplateTest.h"

using namespace std;

void TemplateTest01()
{
	BigNumber<> a(1), b(2);
	cout << equivalent(a, b) << endl;
	cout << equivalent<double>(3, 3) << endl;
}