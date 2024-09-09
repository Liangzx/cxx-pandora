#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/control/while.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define PRED(d, data) BOOST_PP_TUPLE_ELEM(2, 0, data)

#define OP(d, data)                                                            \
  (BOOST_PP_DEC(BOOST_PP_TUPLE_ELEM(2, 0, data)),                              \
   BOOST_PP_ADD_D(d, BOOST_PP_TUPLE_ELEM(2, 1, data), 2)) /**/

// increment 'x' by 2 'n' times
#define STRIDE(x, n) BOOST_PP_TUPLE_ELEM(2, 1, BOOST_PP_WHILE(PRED, OP, (n, x)))

STRIDE(10, 2) // expands to 14
STRIDE(51, 6) // expands to 63
