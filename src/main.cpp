#include <iostream>
#include "hello.h"
#include "world.h"

int main()
{
	std::cout << "CmakeTest" << std::endl;
#ifdef USE_MACRO
		std::cout << "USE_MACRO" << std::endl;
#else
		std::cout << "UNUSE_MACRO" << std::endl;
#endif
	hello();
	world();
	return 0;
}