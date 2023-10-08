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

// ����ģ����ģ�����
template<typename T, int i> 
class cp00;
// ͨ��
template<typename T1, typename T2, int i, template<typename, int> class cp> 
class TMP;
// ����1(��ȫ)��ע���ڶ���ģ������֮ǰ�����Ѿ���ģ��ͨ����������
template<> 
class TMP<int, float, 2, cp00>;
// ����2����һ��������const����
template<typename T1, typename T2, int i, template<typename, int> class cp>
class TMP<const T1, T2, i, cp>;
// ����3����һ��������Ϊcp00��ʵ��������һ����ϵ�����ĸ�����Ϊcp00
template<typename T, int i>
class TMP<cp00<T, i>, cp00<T, i + 10>, i, cp00>;