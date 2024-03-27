#include "Solutions.h"
#include <functional>

namespace LeetCode
{
	// Numb:2
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

	// Numb:43
	string Solutions::multiply(string num1, string num2)
	{
		int* nums2 = new int[num2.length()];
		return "";
	}

	// Numb:67
	string Solutions::addBinary(string a, string b)
	{
		string ret = "";
		char up = '0';
		int aidx = a.length();
		int bidx = b.length();
		while (aidx > 0 || bidx > 0)
		{
			char aVal = --aidx < 0 ? '0' : a[aidx];
			char bVal = --bidx < 0 ? '0' : b[bidx];
			auto val = up + aVal + bVal;
			if (val == 147)
			{
				ret.insert(0, "1");
				up = '1';
			}
			else if (val == 146)
			{
				ret.insert(0, "0");
				up = '1';
			}
			else if(val == 145)
			{
				ret.insert(0, "1");
				up = '0';
			}
			else
			{
				ret.insert(0, "0");
				up = '0';
			}
		}
		if (up == '1')
			ret.insert(0, "1");
		return ret;
	}

	// Numb:69
	int Solutions::mySqrt(int x)
	{
		if (x <= 1) {
			return x;
		}

		int left = 1;
		int right = x / 2;
		int result = 0;

		while (left <= right) {
			int mid = left + (right - left) / 2;

			if (mid <= x / mid) {
				left = mid + 1;
				result = mid;
			}
			else {
				right = mid - 1;
			}
		}

		return result;
	}

	/// <summary>
	/// Numb:70
	/// f(n) = f(n-1) + f(n-2)
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	int Solutions::climbStairs(int n) {
		int a = 1, b = 1, sum;
		for (int i = 0; i < n - 1; i++) {
			sum = a + b;
			a = b;
			b = sum;
		}
		return b;
	}
}