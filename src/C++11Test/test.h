#pragma once

namespace CPP11
{
	class HasPtrMem
	{
	public:
		HasPtrMem() :d(new int(0)) {}
		~HasPtrMem() { delete d; }
		int* d;
	};

	void Test01();
};