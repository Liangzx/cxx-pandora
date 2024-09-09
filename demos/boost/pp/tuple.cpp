#include <boost/preprocessor/tuple.hpp>

#define TUPLE (a, b, c, d)

// (x, (y, (z, BOOST_PP_NIL)))
// BOOST_PP_TUPLE_TO_LIST(3, (x, y, z))

// (a)(b)(c)
// BOOST_PP_TUPLE_TO_SEQ(3, (a, b, c))

// 展开为 a
BOOST_PP_TUPLE_ELEM(4, 0, TUPLE);

// x, y, z
// BOOST_PP_TUPLE_REM(3)(x, y, z)

// x, y, z
// BOOST_PP_TUPLE_REM_CTOR(3, (x, y, z))

// (z, y, x)
// BOOST_PP_TUPLE_REVERSE(3, (x, y, z))

// 展开为一个宏，该宏将吞掉一个指定长度的tuple,  展开为空
// BOOST_PP_TUPLE_EAT(2)( 1,2 )
