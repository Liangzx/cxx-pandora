#include <boost/preprocessor/empty.hpp>
#include <boost/preprocessor/identity.hpp>

#define BOOST_PP_DEF(cv) /* ... */                                             \
  template<class base>                                                         \
  cv() typename implement_subscript_using_begin_subscript<base>::value_type&   \
  implement_subscript_using_begin_subscript<base>::operator[] (index_type i)   \
    cv()                                                                       \
  {                                                                            \
    return base::begin()[i];                                                   \
  }                                                                            \
  /**/

BOOST_PP_DEF(BOOST_PP_EMPTY)
BOOST_PP_DEF(BOOST_PP_IDENTITY(const))
