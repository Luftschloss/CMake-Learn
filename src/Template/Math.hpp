/*
ģ��Ԫ�������
1�������ڼ��㣬�ڳ�������ǰ�����ܶ���һЩ����
2������ά��
3��
*/

template<unsigned long N>
struct binary
{
	static unsigned const value = binary<N / 10>::value * 2 + binary<N % 10>::value;
};

template<>
struct binary<0>
{
	static unsigned const value = 0;
};

template<>
struct binary<1>
{
	static unsigned const value = 1;
};


template<class Iterator> struct iterrator_traits;

template<class ForwardIterator1, class ForwardIterator2>
void iter_swap(ForwardIterator1 i1, ForwardIterator2 i2)
{
	typename iterator_traits<ForwardIterator1>::value_type tmp = *i1;
	*i1 = *i2;
	*i2 = tmp;
}