#include <vector>
#include <iostream>
#include <iterator>  // std::ostream_iterator
#include <algorithm> // std::copy

/**
 * @brief
 *
 *步骤:
 *  1、从头开始，每次比较相邻两元素，若大者在前则交换两个元素，直至数组末尾，此时最大元素为数组最后的元素
 *  2、重复以上步骤，从头开始至上一轮比较的末尾元素
 *
 *性质:
 *   稳定算法
 * @param array
 */
void bubbleSort(std::vector<int> &array)
{
    for (auto i = 0; i < array.size(); i++)
    {
        // 当前轮是否发生交换，如果没有则表明已有序
        auto isExchanged = false;
        for (auto j = 0; j < array.size() - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                std::swap(array[j], array[j + 1]);
                isExchanged = true;
            }
        }
        if (!isExchanged)
        {
            break;
        }
    }
}

int main(int argc, char **argv)
{
    std::vector<int> vec{2, 1, 9, 8, 7};
    bubbleSort(vec);
    //
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, ", "));
}