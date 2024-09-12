// 特殊符号
{
#define PP_REMOVE_PARENS(T) PP_REMOVE_PARENS_IMPL T
#define PP_REMOVE_PARENS_IMPL(...) __VA_ARGS__
#define FOO(A, B) int foo(A x, B y)
#define BAR(A, B) int(PP_REMOVE_PARENS(A), PP_REMOVE_PARENS(B))

  FOO(bool, IntPair) // -> int foo(bool x, IntPair y)
  BAR((bool),
      (std::pair<int, int>)) // -> int foo(bool x, std::pair<int, int> y)
}

// 符号拼接
{
#define PP_CONCAT(A, B) PP_CONCAT_IMPL(A, B)
#define PP_CONCAT_IMPL(A, B) A##B

#define BAR() bar
#define FOO(N) PP_CONCAT(foo_, N)

  FOO(bar())                 // -> foo_bar
  FOO(BAR())                 // -> foo_bar
  PP_CONCAT_IMPL(foo, BAR()) // fooBAR() BAR()--不会展开
  PP_CONCAT(foo, BAR())      // foobar
}

// 自增自减
{
#define PP_INC(N) PP_CONCAT(PP_INC_, N)
#define PP_INC_0 1
#define PP_INC_1 2
  PP_INC(1) // -> 2

#define PP_DEC(N) PP_CONCAT(PP_DEC_, N)
#define PP_DEC_1 0
#define PP_DEC_2 1
#define PP_DEC_256 255
#define PP_DEC_255 254
  PP_INC(256) // -> PP_INC_256 (overflow)
  PP_DEC(0)   // -> PP_DEC_0  (underflow)
  PP_DEC(256) // 255
}

// 逻辑运算
{
#define PP_NOT(N) PP_CONCAT(PP_NOT_, N)
#define PP_NOT_0 1
#define PP_NOT_1 0

  PP_NOT(1) // 0

#define PP_AND(A, B) PP_CONCAT(PP_AND_, PP_CONCAT(A, B))
#define PP_AND_00 0
#define PP_AND_01 0
#define PP_AND_10 0
#define PP_AND_11 1

  PP_AND(1, 1) // 1
}

// 布尔转换
{
#define PP_BOOL(N) PP_CONCAT(PP_BOOL_, N)
#define PP_BOOL_0 0
#define PP_BOOL_1 1
#define PP_BOOL_2 1

  PP_AND(PP_NOT(PP_BOOL(2)), PP_BOOL(0)) // -> 0
  PP_NOT(PP_BOOL(1000))                  // -> PP_NOT_PP_BOOL_1000
}

// 条件选择
{
#define PP_IF(PRED, THEN, ELSE) PP_CONCAT(PP_IF_, PP_BOOL(PRED))(THEN, ELSE)
#define PP_IF_1(THEN, ELSE) THEN
#define PP_IF_0(THEN, ELSE) ELSE

#define DEC_SAFE(N) PP_IF(N, PP_DEC(N), 0)

  DEC_SAFE(2) // -> 1
  DEC_SAFE(1) // -> 0
  DEC_SAFE(0) // -> 0
}

// 惰性求值
{
#define PP_COMMA() ,
#define PP_LPAREN() (
#define PP_RPAREN() )
#define PP_EMPTY()

#define PP_COMMA_IF(N) PP_IF(N, PP_COMMA(), PP_EMPTY())
// 需要注意 PP_IF() 的参数会在 预扫描 阶段被完全展开（例如 PP_COMMA()
// 会被立即展开为逗号，导致参数个数错误）：
// PP_COMMA_IF(1)  // -> PP_IF(1, , , ) (too many arguments after prescan)

// 常用的技巧是 惰性求值 (lazy evaluation)，即条件选择先返回宏函数，再传递参数
// 延迟调用：
#define PP_COMMA_IF(N) PP_IF(N, PP_COMMA, PP_EMPTY)()
  PP_COMMA_IF(0) // (empty)
  PP_COMMA_IF(1) // -> ,
  PP_COMMA_IF(2) // -> ,

#define SURROUND(N) PP_IF(N, PP_LPAREN, [ PP_EMPTY)() \
                    N                                 \
                    PP_IF(N, PP_RPAREN, ] PP_EMPTY)()

  SURROUND(0) // -> [0]
  SURROUND(1) // -> (1)
  SURROUND(2) // -> (2)
}

// 变长参数
{
#define log(format, ...) printf("LOG: " format, ##__VA_ARGS__)
  log("%d%f", 1, .2);   // -> printf("LOG: %d%f", 1, .2);
  log("hello world");   // -> printf("LOG: hello world");
  log("hello world", ); // -> printf("LOG: hello world", );
}

// 下标访问
{
#define PP_GET_N(N, ...) PP_CONCAT(PP_GET_N_, N)(__VA_ARGS__)
#define PP_GET_N_0(_0, ...) _0
#define PP_GET_N_1(_0, _1, ...) _1
#define PP_GET_N_2(_0, _1, _2, ...) _2
#define PP_GET_N_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8

  PP_GET_N(0, foo, bar) // -> foo
  PP_GET_N(1, foo, bar) // -> bar

#define PP_GET_TUPLE(N, T) PP_GET_N(N, PP_REMOVE_PARENS(T))
  PP_GET_TUPLE(0, (foo, bar))

/**
 * 对于 P == 0 的情况，FOO() 只返回 T 的第一个元素但是另一个分支里的
 *  PP_GET_TUPLE(1, T) 仍会被展开，从而要求 T 有至少两个元素
 */
#define FOO(P, T) PP_IF(P, PP_GET_TUPLE(1, T), PP_GET_TUPLE(0, T))
  FOO(0, (foo, bar)) // -> foo
  FOO(1, (foo, bar)) // -> bar
  FOO(0, (baz))      // -> PP_GET_N_1(baz) (too few arguments)

// 类似的，我们可以借助 惰性求值 避免该问题：
#define FOO(P, T) PP_IF(P, PP_GET_N_1, PP_GET_N_0) T
  FOO(0, (baz))
}

// 长度判空
{
#define PP_IS_EMPTY(...)                                                       \
  PP_AND(PP_AND(PP_NOT(PP_HAS_COMMA(__VA_ARGS__)),                             \
                PP_NOT(PP_HAS_COMMA(__VA_ARGS__()))),                          \
         PP_AND(PP_NOT(PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__)),                  \
                PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__())))
#define PP_HAS_COMMA(...) PP_GET_N_8(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 0, 0)
#define PP_COMMA_V(...) ,

  PP_HAS_COMMA(foo, hue) //  -> 1
  PP_HAS_COMMA(foo)      //  -> 1
  PP_IS_EMPTY()          // -> 1
  PP_IS_EMPTY(foo)       // -> 0
  PP_IS_EMPTY(foo())     // -> 0
  PP_IS_EMPTY(())        // -> 0
  PP_IS_EMPTY(() foo)    // -> 0
  PP_IS_EMPTY(PP_EMPTY)  // -> 0
  PP_IS_EMPTY(PP_COMMA)  // -> 0
  PP_IS_EMPTY(, )        // -> 0
  PP_IS_EMPTY(foo, bar)  // -> 0
  PP_IS_EMPTY(, , , )    // -> 0
// 借助 PP_COMMA_IF() 和 PP_IS_EMPTY()，我们可以实现 C++ 20 的 __VA_OPT__(,)
// 功能：
#define PP_VA_OPT_COMMA(...) PP_COMMA_IF(PP_NOT(PP_IS_EMPTY(__VA_ARGS__)))
#define log(format, ...)                                                       \
  printf("LOG: " format PP_VA_OPT_COMMA(__VA_ARGS__) __VA_ARGS__)

  log("%d%f", 1, .2);   // -> printf("LOG: %d%f", 1, .2);
  log("hello world");   // -> printf("LOG: hello world");
  log("hello world", ); // -> printf("LOG: hello world");
}

// 长度计算
{
// 借助 PP_GET_N() 和 PP_VA_OPT_COMMA()，我们可以计算 变长参数的个数（长度）：
#define PP_NARG(...)                                                           \
  PP_GET_N(8,                                                                  \
           __VA_ARGS__ PP_VA_OPT_COMMA(__VA_ARGS__) 8,                         \
           7,                                                                  \
           6,                                                                  \
           5,                                                                  \
           4,                                                                  \
           3,                                                                  \
           2,                                                                  \
           1,                                                                  \
           0)

  PP_NARG()         // -> 0
  PP_NARG(foo)      // -> 1
  PP_NARG(foo())    // -> 1
  PP_NARG(())       // -> 1
  PP_NARG(() foo)   // -> 1
  PP_NARG(PP_EMPTY) // -> 1
  PP_NARG(PP_COMMA) // -> 1
  PP_NARG(, )       // -> 2
  PP_NARG(foo, bar) // -> 2
  PP_NARG(, , , )   // -> 4
}
