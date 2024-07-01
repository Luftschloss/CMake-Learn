//���Ͳ�����typename��class���
template<typename T>
bool equivalent(const T& a, const T& b)
{
	return !(a < b) && !(b < a);
}

//ģ�����������Ĭ��ֵ��c++11��ʼ��
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

//����ģ��ĺͺ������������йص�ģ����������Զ��Ƶ�����ģ������������Ƶ�����
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
	char _v[(N + sizeof(char) - 1) / sizeof(char)];// �� Vec<bool, N> ����ר��ʵ�֣�����һ������λ��ʾһ��bool
};

//==============================����ģ�壨����ģ�塢��ģ�壩��ģ�����==================================
//1�����Ͳ�����type template parameter������ typename �� class ��ǣ�
//2�������Ͳ�����non - type template parameter�������ǣ�������ö�����͡����������ָ�롢������������á�����ĳ�Աָ�룬�����Ͳ�����ģ��ʵ���ĳ�����
//3��ģ���Ͳ�����template template parameter�����硰template<typename T, template<typename> class A> someclass {}; ����
//4��ģ�����������Ĭ��ֵ������ģ�����Ĭ���Ǵ� C++11 ��ʼ֧�֣���
//5������ģ��ĺͺ������������йص�ģ����������Զ��Ƶ�����ģ������������Ƶ����ƣ�
//6��C++11 ����䳤ģ�������
// 
// ����ģ����ģ�����
template<typename T, int i> 
class cp00;
// ͨ��
template<typename T1, typename T2, int i, template<typename, int> class cp> 
class TMP;
// 1����ȫ��������ע���ڶ���ģ������֮ǰ�����Ѿ���ģ��ͨ����������
template<> 
class TMP<int, float, 2, cp00>;
// 2����һ��������const����
template<typename T1, typename T2, int i, template<typename, int> class cp>
class TMP<const T1, T2, i, cp>;
// 3����һ��������Ϊcp00��ʵ��������һ����ϵ�����ĸ�����Ϊcp00
template<typename T, int i>
class TMP<cp00<T, i>, cp00<T, i + 10>, i, cp00>;

template<template<typename T, int i> class BP>
class TMP<int, float, 10, BP>;

//==========================����ģ��������============================
//1���ڶ���ģ������֮ǰ�����Ѿ���ģ��ͨ����primary template����������
//2��ģ����������Ҫ��һ����ͨ������ͬ�Ľӿڣ���Ϊ�˷���ʹ�ã�������������壩һ�㶼��ͬ��
//3��ƥ�������ģ��ʵ����ʱ�����ģ��ͨ�����������������ģ��汾����ƥ�䣬���������¹���
//	 �԰汾AB����� A ��ģ�����ȡֵ������B�����Ӽ���������ƥ�� A����� AB ��ģ�����ȡֵ������
//	 �����桱��ϵ��AB ������Ϊ�գ��Ҳ�Ϊ������ϵ��������������󣬶��ں���ģ�壬�ú������طֱ�
//	 ��overload resolution����������������ϲ�����ƥ���ģ�庯����

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


//=========================����ģ��ʵ������template instantiation��=============
//1��ָ�ڱ��������ʱ���ɺ���ģ�����ģ��ľ���ʵ��Դ���룬����ʹ��ģ��ʱ��ʵ�������滻ģ�����Ͳ��������з����Ͳ�����ģ���Ͳ�������
//2����ʽʵ������implicit instantiation������ʹ��ʵ������ģ��ʱ�Զ����ڵ�ǰ���뵥Ԫ֮ǰ����ģ���ʵ�������룬
//   ģ��ĳ�Ա����һֱ������ʱ�ű�ʵ������
//3����ʽʵ������explicit instantiation����ֱ������ģ��ʵ������ģ�����г�Ա��������ʵ������
//4��ʵ����Ҳ��һ��������������Ϊʵ������������instantiated(or generated) specialization����

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