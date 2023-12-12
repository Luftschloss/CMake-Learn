#include <iostream>
#include "TemplateTest.h"

using namespace std;

void TemplateTest01()
{
	BigNumber<> a(1), b(2);
	cout << equivalent(a, b) << endl;
	cout << equivalent<double>(3, 3) << endl;

	typedef unsigned int uint;
	typedef uint uint2;
	cout << "Template Instantiation" << endl;
	cout << theSameType<unsigned, uint2>::ret << endl;
	cout << theSameType<aTMP<unsigned, 2>, aTMP<uint2, 2>>::ret << endl;
	cout << theSameType<aTMP<int, 2>, aTMP<int, 3>>::ret << endl;
	cin.get();

	aTMP1<int> a1;
	cout << a1.f1() << endl;
	cout << a1.f2() << endl;
}