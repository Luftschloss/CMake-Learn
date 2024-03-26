#include "Solutions.h"
#include <functional>

namespace LeetCode
{
	//Numb:2
	ListNode* Solutions::addTwoNumbers(ListNode* l1, ListNode* l2)
	{
		auto sum = l1->val + l2->val;
		bool up = sum >= 10;
		ListNode* ret = new ListNode(sum % 10);
		ListNode* tmp = ret;
		l1 = l1->next;
		l2 = l2->next;
		while (l1 || l2)
		{
			sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + up;
			up = sum >= 10;
			if (l1)
				l1 = l1->next;
			if (l2)
				l2 = l2->next;
			tmp->next = new ListNode(sum % 10);
			tmp = tmp->next;
		}
		if(up)
			tmp->next = new ListNode(1);
		return ret;
	}
	
	/// <summary>
	/// Numb:29
	/// -2^31 <= dividend, divisor <= 2^31 - 1
	/// divisor != 0
	/// </summary>
	/// <param name="dividend"></param>
	/// <param name="divisor"></param>
	/// <returns></returns>
	int Solutions::divide(int dividend, int divisor)
	{
		std::function<bool(int)> sign = [](int a) { return a >= 0; };
		bool sign0 = sign(dividend);
		bool signV = sign0 ^ sign(divisor);
		signV = !signV;
		int ret = 0;
		divisor = signV ? divisor : -divisor;
		while (sign0 == sign(dividend - divisor) || dividend - divisor == 0)
		{
			dividend -= divisor;
			ret++;
		}
		return signV ? ret : -ret;
	}
}