#include <boost/preprocessor/tuple.hpp>

#define TUPLE (a, b, c, d)

void Tuple()
{
  // (x, (y, (z, BOOST_PP_NIL)))
  // BOOST_PP_TUPLE_TO_LIST(3, (x, y, z))

  // (a)(b)(c)
  // BOOST_PP_TUPLE_TO_SEQ(3, (a, b, c))

  // 展开为 a
  // BOOST_PP_TUPLE_ELEM(4, 0, TUPLE);

  // x, y, z
  // BOOST_PP_TUPLE_REM(3)(x, y, z)

  // x, y, z
  // BOOST_PP_TUPLE_REM_CTOR(3, (x, y, z))

  // (z, y, x)
  // BOOST_PP_TUPLE_REVERSE(3, (x, y, z))

  // 展开为一个宏，该宏将吞掉一个指定长度的tuple,  展开为空
  // BOOST_PP_TUPLE_EAT(2)( 1,2 )
}

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
  BOOST_PP_ARRAY_POP_BACK(ARRAY)

  // 从一个数组的头部弹出一个元素
  // (2, ( y , z))
  // BOOST_PP_ARRAY_POP_FRONT(ARRAY)

  BOOST_PP_ARRAY_POP_FRONT_Z(2, ARRAY)
}
