#pragma once
#include <string>
#include "Datas.h"


namespace LeetCode
{
    using string = std::string;
    class Solutions {
    public:
        static ListNode* addTwoNumbers(ListNode* l1, ListNode* l2);

        static int divide(int dividend, int divisor);

        static string multiply(string num1, string num2);

        static string addBinary(string a, string b);

        int mySqrt(int x);
    };
}