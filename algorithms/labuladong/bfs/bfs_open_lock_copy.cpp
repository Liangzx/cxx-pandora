#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <queue>

std::string plusOne(std::string s, int j)
{
    if (s[j] == '9')
    {
        s[j] = '0';
        return s;
    }
    s[j] += 1;
    return s;
}
std::string minusOne(std::string s, int j)
{
    if (s[j] == '0')
    {
        s[j] = '9';
        return s;
    }
    s[j] -= 1;
    return s;
}

/**
 * @brief
 *
 * @param deadends
 * @param target
 * @return int
 */
int openLock(std::string target)
{
    // 记录需要跳过的死亡密码

    // 记录 visited，防止走回头路
    std::set<std::string> visited;

    std::queue<std::string> q;

    // 从起点开始启动广度优先搜索
    int step = 0;
    q.push("0000");
    visited.insert("0000");

    //
    while (!q.empty())
    {
        auto sz = q.size();

        // 将当前队列中的所有节点向周围扩散
        for (int i = 0; i < sz; i++)
        {
            auto cur = q.front();
            q.pop();
            step++;
            // std::cout << cur << std::endl;
            // 将一个节点的未遍历相邻节点加入队列
            for (int j = 0; j < 4; j++)
            {
                // 加一
                std::string up = plusOne(cur, j);
                if (!visited.count(up))
                {
                    visited.insert(up);
                    q.push(up);
                }

                // 减一
                std::string down = minusOne(cur, j);
                if (!visited.count(down))
                {
                    visited.insert(down);
                    q.push(down);
                }
            }
        }
        // step++;
    }

    return step;
}

int main()
{
    std::cout << openLock("0000") << std::endl;
}

/**
 * @brief 传统的 BFS 框架就是从起点开始向四周扩散，遇到终点时停止；
 *        而双向 BFS 则是从起点和终点同时开始扩散，当两边有交集的时候停止
 *
 * @param deadends
 * @param target
 * @return int
 */
int openLock2(const std::vector<std::string> &deadends, std::string target)
{
    // 记录需要跳过的死亡密码
    std::set<std::string> deads;
    for (auto s : deadends)
    {
        deads.insert(s);
    }

    // 记录 visited，防止走回头路
    std::set<std::string> visited;
    // 用集合不用队列，可以快速判断元素是否存在
    std::set<std::string> q1;
    std::set<std::string> q2;

    //
    int step = 0;
    q1.insert("0000");
    q2.insert(target);

    while (!q1.empty() && !q2.empty())
    {
        std::set<std::string> temp;

        for (auto cur : q1)
        {
            // 判断是否到达终点
            if (target == cur)
            {
                return step;
            }

            // 判定死亡密码
            if (deads.count(cur))
            {
                continue;
            }

            // 标记 cur 为 visited
            if (!visited.count(cur))
            {
                visited.insert(cur);
            }

            // 将一个节点的未遍历相邻节点加入队列
            for (int j = 0; j < 4; j++)
            {
                // 加一
                std::string up = plusOne(cur, j);
                if (!visited.count(up))
                {
                    visited.insert(up);
                    q1.insert(up);
                }

                // 减一
                std::string down = minusOne(cur, j);
                if (!visited.count(down))
                {
                    visited.insert(down);
                    q1.insert(down);
                }
            }
        }

        // 在这里增加步数
        step++;

        // 这里交换 q1 q2，下一轮 while 就是扩散 q2
        q1 = q2;
        q2 = temp;
    }
    return -1;
}