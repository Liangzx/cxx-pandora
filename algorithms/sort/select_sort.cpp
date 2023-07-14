#include <vector>
#include <iostream>
#include <iterator>  // std::ostream_iterator
#include <algorithm> // std::copy

/**
 * @brief
 *
 * 步骤：
 *  1、搜索整个列表找出最小项，若该项不为第一项则与第一项交换位子
 *  2、重复上述步骤，每次搜索未被排序的剩余列表并将最小元素与已排序段的后一位交换直至所有元素都被排序
 *
 * 性质：
 *  不稳定算法
 *
 * @param array
 */
void selectSort(std::vector<int> &array)
{
    for (auto i = 0; i < array.size(); i++)
    {
        auto minIndex = i;
        for (auto j = i + 1; j < array.size(); j++)
        {
            if (array[minIndex] > array[j])
            {
                minIndex = j;
            }
        }
        if (minIndex != i)
        {
            std::swap(array[minIndex], array[i]);
        }
    }
}

int main(int argc, char **argv)
{
    std::vector<int> vec{2, 1, 9, 8, 7};
    selectSort(vec);
    //
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, ", "));
}