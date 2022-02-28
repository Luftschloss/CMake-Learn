#include <iostream>
#include "hellow.h"
#include "world.h"

int main()
{
	std::cout << "CmakeTest" << std::endl;
#ifdef USE_MACRO
		std::cout << "USE_MACRO" << std::endl;
#else
		std::cout << "UNUSE_MACRO" << std::endl;
#endif
	hellow();
	world();
	return 0;
}