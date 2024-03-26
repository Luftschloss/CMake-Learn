#pragma once
namespace LeetCode
{
    struct ListNode {
        int val;
        ListNode* next;
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode* next) : val(x), next(next) {}
    };

    static ListNode* GetNumList(int num)
    {
        ListNode* ret = new ListNode(num % 10);
		ListNode* tmp = ret;
        num /= 10;
        while (num > 0)
        {
			tmp->next = new ListNode(num % 10);
			tmp = tmp->next;
			num /= 10;
		}
		return ret;
	}
}