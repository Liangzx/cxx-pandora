
#include <iostream>
#include <vector>

/**
 * @brief 暴力递归
 * 
 * @param N 
 * @return int 
 */
int fib1(int N)
{
    if (N == 1 || N == 2)
    {
        return 1;
    }
    return fib1(N - 1) + fib1(N - 2);
}

/**
 * @brief 带备忘录的递归解法
 * 
 * @param N 
 * @return int 
 */
int fib2(int N)
{
    std::vector<int> memo(N, 0);
    return fib2Helper(memo, N);
}
//
int fib2Helper(std::vector<int> &memo, int n)
{
    if (n == 1 || n == 2)
    {
        return 1;
    }
    //
    if (memo[n] != 0)
    {
        return memo[n];
    }
    //
    memo[n] = fib2Helper(memo, n - 1) + fib2Helper(memo, n - 2);
    return memo[n];
}

/**
 * @brief dp 数组的迭代解法
 * 
 * @param N 
 * @return int 
 */
int fib3(int N)
{
    if (N < 1)
    {
        return 0;
    }
    //
    if (N == 1 || N == 2)
    {
        return 1;
    }
    //
    std::vector<int> dp(N, 0);
    dp[1] = dp[2] = 1;
    for (int i = 3; i < N; i++)
    {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    //
    return dp[N];
}

/**
 * @brief 状态压缩
 * 
 * @param N 
 * @return int 
 */
int fib4(int N)
{
    if (N < 1)
    {
        return 0;
    }
    if (N == 1 || N == 2)
    {
        return 1;
    }
    //
    int prev = 1;
    int cur = 1;
    //
    for (int i = 3; i < N; i++)
    {
        int sum = prev + cur;
        prev = cur;
        cur = sum;
    }
    return cur;
}