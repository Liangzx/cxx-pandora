#include <boost/preprocessor/arithmetic.hpp>

BOOST_PP_ADD(1, 3) // expands to 7

BOOST_PP_DEC(BOOST_PP_DEC(6)) // expands to 4
BOOST_PP_DEC(0) // expands to 0
