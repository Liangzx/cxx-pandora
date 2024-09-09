

#include <boost/preprocessor/array.hpp>

#define ARRAY (3, (x, y, z))
void Array()
{
  // 3
  // BOOST_PP_ARRAY_SIZE(ARRAY)

  // (x, y, z)
  // BOOST_PP_ARRAY_DATA(ARRAY)

  // 从一个数组中提取第i个元素
  // x
  // BOOST_PP_ARRAY_ELEM(0, ARRAY)

  // 向一个数组中插入一个元素（原数组第i个位置之前）
  // (4, (d , x , y , z))
  // BOOST_PP_ARRAY_INSERT(ARRAY, 0, d)

  // 从一个数组的尾部弹出一个元素
  // (2, ( x , y))
  // BOOST_PP_ARRAY_POP_BACK(ARRAY)

  // 从一个数组的头部弹出一个元素
  // (2, ( y , z))
  // BOOST_PP_ARRAY_POP_FRONT(ARRAY)

  // BOOST_PP_ARRAY_POP_FRONT_Z(2, ARRAY)
}
