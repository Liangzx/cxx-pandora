#include <vector>
#include <iostream>
#include <iterator>  // std::ostream_iterator
#include <algorithm> // std::copy

/**
 * @brief
 *
 * 步骤：
 *  1、将第一元素当成有序序列，后续元素当成无序序列，依次将无序序列元素插入到有序序列中
 *
 * 性质：
 *  稳定算法
 *
 * @param array
 */

void insertionSort(std::vector<int> &array)
{
    // i 代表无序序列首元素（无序序列前为有序序列）
    auto i = 1;
    while (i < array.size())
    {
        auto j = i - 1;
        int itermToInsert = array[i];
        while (j >= 0)
        {
            if (array[j] > itermToInsert)
            {
                array[j + 1] = array[j];
                j--;
            }
            else
            {
                break;
            }
        }
        array[j + 1] = itermToInsert;
        i++;
    }
}