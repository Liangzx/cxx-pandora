#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

void backtrack(std::vector<int> &nums, std::vector<int> &track, std::vector<std::vector<int>> &results);

/**
 * @brief 排列组合问题
 * 
 * @param nums 
 * @return std::vector<std::vector<int>> 
 */
std::vector<std::vector<int>> permute(/*std::vector<int> nums*/)
{
    std::vector<std::vector<int>> results;
    std::vector<int> nums{1, 2, 3};
    std::vector<int> track;

    backtrack(nums, track, results);

    for (size_t i = 0; i < results.size(); i++)
    {
        std::copy(std::begin(results[i]), std::end(results[i]), std::ostream_iterator<int>(std::cout, " "));

        std::cout << std::endl;
    }

    return results;
}

// 路径：记录在 track 中，nums 中已经被选了的元素
// 选择列表：nums 中不存在于 track 的那些元素
// 结束条件：nums 中的元素全都在 track 中出现
void backtrack(std::vector<int> &nums, std::vector<int> &track, std::vector<std::vector<int>> &results)
{
    // 触发结束条件
    if (nums.size() == track.size())
    {
        results.push_back(track);
    }

    //
    for (auto v : nums)
    {
        if (std::find(track.begin(), track.end(), v) != track.end())
        {
            continue;
        }
        // 遍历完后回复节点状态
        track.push_back(v);
        backtrack(nums, track, results);
        track.pop_back();
    }
}

int main()
{
    permute();
}

