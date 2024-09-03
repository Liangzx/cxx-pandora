// https://www.boost.org/doc/libs/1_86_0/libs/preprocessor/doc/AppendixA-AnIntroductiontoPreprocessorMetaprogramming.html

#define FOO(X) X // Unary identity macro

void func1()
{
  // a macro argument can never contain an unmatched parenthesis,
  // or a comma that is not surrounded by matched parentheses.
  // For example, both lines following the definition of FOO below are ill-formed:
  // FOO(, );        // un-parenthesized comma or two empty arguments
  // FOO());           // unmatched parenthesis or missing argument
}

void func2()
{
  // Note also that the following tokens do not have special status;
  // the preprocessor knows nothing about matched pairs of braces, brackets, or angle brackets:
  // {  }  [  ]  <  >
  // As a result, these lines are also ill-formed:
  // FOO(std::pair<int, long>);                // two arguments
  // FOO({ int x = 1, y = 2; return x+y; })   // two arguments
}

void func3()
{
  // It is possible to pass either string of tokens above as part
  // of a single macro argument, provided it is parenthesized:
  // FOO((std::pair<int,int>)) ;                // one argument
  // FOO(({ int x = 1, y = 2; return x+y; }));  // one argument
}

#include <boost/preprocessor/tuple/elem.hpp>
#define TUPLE (a, b, c, d)
void func4()
{
  // BOOST_PP_TUPLE_ELEM(4, 0, TUPLE) // 展开为 a
}


int main()
{

  return 0;
}
