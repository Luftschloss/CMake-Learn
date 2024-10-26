#include "SEHTest.h"

#include <vector>
#include <windows.h>

#include "CrashHandler.h"


long __stdcall callback(_EXCEPTION_POINTERS* excp)
{
	MessageBox(0, "Error", "error", MB_OK);
	printf("Error   address   %x/n", excp->ExceptionRecord->ExceptionAddress);
	printf("CPU   register:/n");
	return   EXCEPTION_EXECUTE_HANDLER;
}


int main(int argc, char* argv[])
{
	CrashHandler::InitCrashHandler();
	CrashHandler::CrashTest();

	int capacity = 5;
	std::vector<int> v;
	v.reserve(capacity);
	for (int i = 0; i < 10000; i++)
	{
		if (i >= capacity) {
			capacity *= 2;
			v.reserve(capacity);
		}
		v.push_back(i);
	}

	auto a = WindowsSEH::Funcensteinl();
	auto b = WindowsSEH::Funcenstein2();
	auto c = WindowsSEH::Funcenstein3(2);
	auto d = WindowsSEH::Funcenstein3(1);
	//auto e = WindowsSEH::ExceptionCatch();
	SetUnhandledExceptionFilter(callback);
	int arr[3] = { 1,2,3 };
	arr[3] = 5;

	return 0;
}
