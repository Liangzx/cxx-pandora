#include <iostream>

#include <boost/concept_check.hpp>

using namespace boost;

template<typename T>
class Calculator
{
public:
  BOOST_CONCEPT_ASSERT((IntegerConcept<T>));

  T add(T x, T y)
  {
    return x + y;
  }

  T subtract(T x, T y)
  {
    return x - y;
  }
};

// func
template<typename Func, typename Args>
class UnaryHandler final
{
public:
  BOOST_CONCEPT_ASSERT((UnaryFunctionConcept<Func, bool, Args>));

  explicit UnaryHandler(const Func& func, Args args)
    : func_{func}, args_{std::move(args)}
  {
  }

  void show()
  {
    func_(args_);
  }

private:
  const Func& func_;
  Args args_;
};

int main()
{
  auto func = [](int a) {
    std::cout << a << std::endl;
    return false;
  };

  UnaryHandler<decltype(func), int> uh(func, 3);
  uh.show();
  Calculator<double> calc;
  int sum = calc.add(5.4, 4.3);
  // int diff = calc.subtract(5, 3);

  // 这个将不会编译，因为std::string不支持+和-运算
  // Calculator<std::string> strCalc;

  return 0;
}

/**
   BOOST_concept(Integer, (T))
  {
      BOOST_CONCEPT_USAGE(Integer)
        {
            x.error_type_must_be_an_integer_type();
        }
   private:
      T x;
  };

typedef ::boost::concepts::detail::instantiate<
  &::boost::concepts::requirement_<void (*)(IntegerConcept<T>)>::failed>
  boost_concept_check11 __attribute__((__unused__));

template<typename T>
struct Integer;
template<typename T>
struct IntegerConcept: Integer<T>
{
};
template<typename T>
struct Integer
{
  typedef ::boost::concepts::detail::instantiate<
    &::boost::concepts::requirement_<void (*)(
      boost::concepts::usage_requirements<Integer>)>::failed>
    boost_concept_check72 __attribute__((__unused__));

  ~Integer()
  {
    x.error_type_must_be_an_integer_type();
  }

private:
  T x;
};

 */
