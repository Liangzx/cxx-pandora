#include <iostream>
#include <vector>

/**
 * @brief 暴力解法
 * 
 * @param coins 
 * @param amount 
 * @return int 
 */
int coinChange1(std::vector<int> coins, int amount)
{
    coinChange1Helper(coins, amount);
}
//
int coinChange1Helper(std::vector<int> coins, int amount)
{

    if (amount < 0)
    {
        return -1;
    }

    if (amount == 0)
    {
        return 0;
    }

    int res = INT32_MAX;

    for (int i = 0; i < coins.size(); i++)
    {
        auto r = coinChange1Helper(coins, amount - coins[i]);
        if (r == -1)
        {
            continue;
        }
        // r = 0 那么刚好需要一枚该面值的
        // r < 0 则说明面值太大了不合适继续选下一个面值
        // r > 0 面值小了还需要用其他面值的
        // 所以这里 r + 1
        res = std::min(res, r + 1);
    }

    return res == INT32_MAX ? -1 : res;
}

// TODO: 带备忘录的递归
int coinChange2(std::vector<int> coins, int amount)
{
    std::vector<int> memo(amount + 1, -666);
    coinChange1Helper(coins, amount, memo);
}
//
int coinChange1Helper(std::vector<int> coins, int amount, std::vector<int> memo)
{

    if (amount < 0)
    {
        return -1;
    }

    if (amount == 0)
    {
        return 0;
    }
    //
    if(memo[amount] != -666)
    {
        return memo[amount];
    }
    //
    int res = INT32_MAX;

    for (int i = 0; i < coins.size(); i++)
    {
        auto r = coinChange1Helper(coins, amount - coins[i]);
        if (r == -1)
        {
            continue;
        }
        // r = 0 那么刚好需要一枚该面值的
        // r < 0 则说明面值太大了不合适继续选下一个面值
        // r > 0 面值小了还需要用其他面值的
        // 所以这里 r + 1
        res = std::min(res, r + 1);
    }

    return res == INT32_MAX ? -1 : res;
}

/**
 * @brief dp 数组的迭代解法
 * 
 * @param coins 
 * @param amount 
 * @return int 
 */
int coinChange3(std::vector<int> coins, int amount)
{
    std::vector<int> dp(amount + 1, amount + 1);

    dp[0] = 0;

    for (std::size_t i = 1; i < dp.size(); i++)
    {
        for (int coin : coins)
        {
            if (i - coin < 0)
            {
                continue;
            }
            dp[i] = std::min(dp[i], dp[i - coin] + 1);
        }
    }

    return dp[amount] == amount + 1 ? -1 : dp[amount];
}