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

// 用于模板型模板参数
template<typename T, int i> 
class cp00;
// 通例
template<typename T1, typename T2, int i, template<typename, int> class cp> 
class TMP;
// 特例1(完全)（注：在定义模板特例之前必须已经有模板通例的声明）
template<> 
class TMP<int, float, 2, cp00>;
// 特例2：第一个参数有const修饰
template<typename T1, typename T2, int i, template<typename, int> class cp>
class TMP<const T1, T2, i, cp>;
// 特例3：第一二个参数为cp00的实例且满足一定关系，第四个参数为cp00
template<typename T, int i>
class TMP<cp00<T, i>, cp00<T, i + 10>, i, cp00>;