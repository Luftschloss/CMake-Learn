#pragma once
#include <vector>

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


	class SmartPtr
	{
	public:
		SmartPtr(float v, std::string n = "default")
		{
			val = v; 
			name = n;
		}

		SmartPtr(std::shared_ptr<SmartPtr> ptr)
		{
			val = ptr->val;
			name = ptr->name;
		}

		~SmartPtr()
		{
			std::cout << "~SmartPtr : " << name.c_str() << std::endl;
		}

	private:
		float val;
		std::string name;
	};

	class UniqueT
	{
	public:
		static std::unique_ptr<UniqueT> MakeOne(std::string n)
		{
			return std::make_unique<UniqueT>(n);
		}
		UniqueT(std::string n) :name(n) {};
		std::string GetName() { return name; }
	private:
		std::string name = "UniqueT";
	};

	void TestUniquePtr();

	int TestVa_List(int count, ...);

	namespace Regex
	{
		void Test01();
		void Test02();
	}

	namespace Container
	{
		void QueueTest();
	}
};