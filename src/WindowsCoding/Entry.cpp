#include "SEHTest.h"

#include <tchar.h>

int main(int argc, char* argv[])
{
	auto a = WindowsSEH::Funcensteinl();
	auto b = WindowsSEH::Funcenstein2();
	auto c = WindowsSEH::Funcenstein3(2);
	auto d = WindowsSEH::Funcenstein3(1);

	auto e = WindowsSEH::ExceptionCatch();
	return 0;
}
