
#include <iostream>
#include <vector>

/**
 * @brief https://leetcode-cn.com/problems/longest-increasing-subsequence/
 * 
 * @param nums 
 * @return int 
 */
int lengthOfLIS(std::vector<int> nums)
{
    // dp[i] 表示以 nums[i] 这个数结尾的最长递增子序列的长度
    // base case：dp[i] 初始值为 1，因为以 nums[i] 结尾的最长递增子序列起码要包含它自己
    std::vector<int> dp(nums.size(), 1);

    for (int i = 0; i < nums.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (nums[i] > nums[j])
            {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
    }

    int res = 0;
    for (int i = 0; i < dp.size(); i++)
    {
        res = std::max(res, dp[i]);
    }
    return res;
}