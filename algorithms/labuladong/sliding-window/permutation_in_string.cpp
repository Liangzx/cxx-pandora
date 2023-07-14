#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>

/*
给定两个字符串 s1 和 s2，写一个函数来判断 s2 是否包含 s1 的排列。
换句话说，第一个字符串的排列之一是第二个字符串的 子串 
*/

bool permutation_in_string(std::string t, std::string s)
{
    std::unordered_map<char, int> need;
    std::unordered_map<char, int> window;

    for (auto c : t)
    {
        need[c]++;
    }

    int left = 0;
    int right = 0;

    int valid = 0;

    while (right < s.size())
    {
        // c 为移入窗口的字符
        auto c = s[right];

        // 右移窗口
        right++;

        // 更新窗口
        if (need.count(c))
        {
            window[c]++;
            if (window[c] == need[c])
            {
                valid++;
            }
        }

        // 判断左侧窗口是否要收缩 - 窗口字符满足查找字符
        while (right - left >= t.size())
        {
            if (valid == need.size())
            {
                return true;
            }

            // d 为移出窗口的字符
            auto d = s[left];

            // 左移窗口
            left++;

            if (need.count(d))
            {
                if (need[d] == window[d])
                {
                    valid--;
                }
                window[d]--;
            }
        }
    }

    return false;
}
