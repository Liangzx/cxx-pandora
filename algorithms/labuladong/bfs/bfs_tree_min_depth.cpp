#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>

struct TreeNode
{
    int val_;
    TreeNode *left_;
    TreeNode *right_;

    TreeNode() : val_(0), left_(nullptr), right_(nullptr) {}
    TreeNode(int val, TreeNode *left, TreeNode *right) : val_(val), left_(left), right_(right) {}
};

/**
 * @brief  二叉树的最小深度
 * 
 * @param root 
 * @return int 
 */
int min_depth(TreeNode *root)
{
    if (root == nullptr)
    {
        return 0;
    }

    std::queue<TreeNode *> que;

    // 加入第一个节点
    que.push(root);

    int depth = 1;

    while (!que.empty())
    {
        auto sz = que.size();
        for (int i = 0; i < sz; i++)
        {
            auto cur = que.front();
            que.pop();
            if (cur->left_ == nullptr && cur->right_ == nullptr)
            {
                return depth;
            }

            if (cur->left_ != nullptr)
            {
                que.push(cur->left_);
            }

            if (cur->right_ != nullptr)
            {
                que.push(cur->right_);
            }
        }
        ++ depth;
    }
    return depth;
}