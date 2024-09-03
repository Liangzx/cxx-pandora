#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition.hpp>

#ifndef MAX_IS_FUNCTION_TESTER_PARAMS
#define MAX_IS_FUNCTION_TESTER_PARAMS 15
#endif

#define IS_FUNCTION_TESTER(Z, N, _)                                            \
  template<class R BOOST_PP_COMMA_IF(N) BOOST_PP_ENUM_PARAMS(N, class A)>      \
  bool is_function_tester(R (*)(BOOST_PP_ENUM_PARAMS(N, A)));                  \
  /**/

BOOST_PP_REPEAT(BOOST_PP_INC(MAX_IS_FUNCTION_TESTER_PARAMS),
                IS_FUNCTION_TESTER,
                _)

#undef IS_FUNCTION_TESTER
