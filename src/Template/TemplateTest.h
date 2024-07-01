//类型参数用typename或class标记
template<typename T>
bool equivalent(const T& a, const T& b)
{
	return !(a < b) && !(b < a);
}

//模板参数可以有默认值（c++11开始）
template<typename T = int>
class BigNumber
{
	T _v;
public:
	BigNumber(T a) :_v(a) {}
	inline bool operator <(const BigNumber& rhs) const;
};

//
template<typename T>
inline bool BigNumber<T>::operator<(const BigNumber& rhs) const
{
	return _v < rhs._v;
}

void TemplateTest01();

//函数模板的和函数参数类型有关的模板参数可以自动推导，类模板参数不存在推导机制
template<typename T, int N>
class Vec {
	T _v[N];
};

template<>
class Vec<float, 4> {
	float _v[4];
};

template<int N>
class Vec<bool, N> {
	char _v[(N + sizeof(char) - 1) / sizeof(char)];// 对 Vec<bool, N> 进行专门实现，如用一个比特位表示一个bool
};

//==============================关于模板（函数模板、类模板）的模板参数==================================
//1、类型参数（type template parameter），用 typename 或 class 标记；
//2、非类型参数（non - type template parameter）可以是：整数及枚举类型、对象或函数的指针、对象或函数的引用、对象的成员指针，非类型参数是模板实例的常量；
//3、模板型参数（template template parameter），如“template<typename T, template<typename> class A> someclass {}; ”；
//4、模板参数可以有默认值（函数模板参数默认是从 C++11 开始支持）；
//5、函数模板的和函数参数类型有关的模板参数可以自动推导，类模板参数不存在推导机制；
//6、C++11 引入变长模板参数。
// 
// 用于模板型模板参数
template<typename T, int i> 
class cp00;
// 通例
template<typename T1, typename T2, int i, template<typename, int> class cp> 
class TMP;
// 1、完全特例化（注：在定义模板特例之前必须已经有模板通例的声明）
template<> 
class TMP<int, float, 2, cp00>;
// 2、第一个参数有const修饰
template<typename T1, typename T2, int i, template<typename, int> class cp>
class TMP<const T1, T2, i, cp>;
// 3、第一二个参数为cp00的实例且满足一定关系，第四个参数为cp00
template<typename T, int i>
class TMP<cp00<T, i>, cp00<T, i + 10>, i, cp00>;

template<template<typename T, int i> class BP>
class TMP<int, float, 10, BP>;

//==========================关于模板特例化============================
//1、在定义模板特例之前必须已经有模板通例（primary template）的声明；
//2、模板特例并不要求一定与通例有相同的接口，但为了方便使用（体会特例的语义）一般都相同；
//3、匹配规则，在模板实例化时如果有模板通例、特例加起来多个模板版本可以匹配，则依据如下规则：
//	 对版本AB，如果 A 的模板参数取值集合是B的真子集，则优先匹配 A，如果 AB 的模板参数取值集合是
//	 “交叉”关系（AB 交集不为空，且不为包含关系），则发生编译错误，对于函数模板，用函数重载分辨
//	 （overload resolution）规则和上述规则结合并优先匹配非模板函数。

template<typename T1, typename T2>
class theSameType {
public:
	enum { ret = false };
};

template<typename T>
class theSameType<T, T> {
public:
	enum { ret = true };
};

template<typename T, int i> class aTMP {};


//=========================关于模板实例化（template instantiation）=============
//1、指在编译或链接时生成函数模板或类模板的具体实例源代码，即用使用模板时的实参类型替换模板类型参数（还有非类型参数和模板型参数）；
//2、隐式实例化（implicit instantiation）：当使用实例化的模板时自动地在当前代码单元之前插入模板的实例化代码，
//   模板的成员函数一直到引用时才被实例化；
//3、显式实例化（explicit instantiation）：直接声明模板实例化，模板所有成员立即都被实例化；
//4、实例化也是一种特例化，被称为实例化的特例（instantiated(or generated) specialization）。

template<typename T>
class aTMP1 {
public:
	std::string f1() { return "f1()"; };
	std::string f2() { return "f2()"; };
};

template<int N>
class aTMP2 {
public:
	enum { ret = N == 0 ? 1 : N * aTMP2<N - 1>::ret };
};

void TM1();