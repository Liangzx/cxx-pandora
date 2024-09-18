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
#define PP_CONCAT_IMPL1(A, B) A B

#define BAR() bar
#define FOO(N) PP_CONCAT(foo_, N)

  FOO(bar())                  // -> foo_bar
  FOO(BAR())                  // -> foo_bar
  PP_CONCAT_IMPL(foo, BAR())  // fooBAR() BAR()--不会展开
  PP_CONCAT_IMPL1(foo, BAR()) // foo bar --会展开
  PP_CONCAT(foo, BAR())       // foobar
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
#define PP_DEC_3 2
#define PP_DEC_4 3
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
#define PP_BOOL_3 1
#define PP_BOOL_4 1

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
  // 另外，这里只能用 PP_GET_N(8, ...)，而不能用 PP_GET_N_8()：
  PP_GET_N(0, 1 PP_COMMA() 2) // -> 1
  PP_GET_N_0(1 PP_COMMA() 2)  // -> 1 , 2
}

// 遍历访问
{
// 借助 PP_CONCAT() 和 PP_NARG()，我们可以 遍历 (traverse) 变长参数：
#define PP_FOR_EACH(DO, CTX, ...)                                              \
  PP_CONCAT(PP_FOR_EACH_, PP_NARG(__VA_ARGS__))(DO, CTX, 0, __VA_ARGS__)

#define PP_FOR_EACH_0(DO, CTX, IDX, ...)

#define PP_FOR_EACH_1(DO, CTX, IDX, VAR, ...) DO(VAR, IDX, CTX)

#define PP_FOR_EACH_2(DO, CTX, IDX, VAR, ...)                                  \
  DO(VAR, IDX, CTX)                                                            \
  PP_FOR_EACH_1(DO, CTX, PP_INC(IDX), __VA_ARGS__)

#define PP_FOR_EACH_3(DO, CTX, IDX, VAR, ...)                                  \
  DO(VAR, IDX, CTX)                                                            \
  PP_FOR_EACH_2(DO, CTX, PP_INC(IDX), __VA_ARGS__)

#define DO_EACH(VAR, IDX, CTX) PP_COMMA_IF(IDX) CTX VAR

  PP_FOR_EACH(DO_EACH, void, )       // (empty)
  PP_FOR_EACH(DO_EACH, int, a, b, c) // -> int a, int b, int c
  PP_FOR_EACH(DO_EACH, bool, x)      // -> bool x
  PP_FOR_EACH_1(DO_EACH, bool, 0, a) // ->bool a
  PP_FOR_EACH(DO_EACH, int, a, b)
  PP_FOR_EACH_2(DO_EACH, int, 0, a, b)
  DO_EACH(a, 0, int)                // -> int a
  PP_FOR_EACH_1(DO_EACH, int, 1, b) // -> , int b --> int a, int b

  // PP_FOR_EACH() 的参数分为三部分：元素的转换操作 DO、遍历的上下文参数 CTX 和
  // 变长参数 ... 其中 DO() 接受三个参数：当前元素 VAR、对应下标 IDX 和
  // 遍历的上下文 CTX，并返回元素 VAR 转换后的结果 先通过 PP_CONCAT() 和
  // PP_NARG() 选择 变长参数长度 对应的 PP_FOR_EACH_I 符号 PP_FOR_EACH_I()
  // 的参数分为四部分：元素的转换操作 DO、遍历的上下文参数 CTX、当前元素下标 IDX
  // 和 变长参数 ... 展开为两部分：变长参数 第一个元素 的转换 DO() 和 变长参数
  // 剩余元素 递归调用 I - 1 宏（下标更新为 IDX + 1） 当 I == 0
  // 时，展开为空，递归终止
}

// 符号匹配
{
// 借助 PP_CONCAT() 和 PP_IS_EMPTY()，我们可以 匹配任意的特定符号：
#define PP_IS_SYMBOL(PREFIX, SYMBOL) PP_IS_EMPTY(PP_CONCAT(PREFIX, SYMBOL))
#define IS_VOID_void
#define IS_INT_int

  PP_IS_SYMBOL(IS_VOID_, void)           // -> 1
  PP_IS_SYMBOL(IS_VOID_, )               // -> 0
  PP_IS_SYMBOL(IS_VOID_, int)            // -> 0
  PP_IS_SYMBOL(IS_VOID_, void*)          // -> 0
  PP_IS_SYMBOL(IS_VOID_, void x)         // -> 0
  PP_IS_SYMBOL(IS_VOID_, void(int, int)) // -> 0
  PP_IS_SYMBOL(IS_INT_, int)             // ->1

// 先定义一个辅助宏 IS_VOID_void：字面量是前缀 IS_VOID_ 和 目标结果 void
// 的拼接，展开为空 再通过 PP_CONCAT(PREFIX, SYMBOL) 把 前缀 和 参数
// 拼接为新的符号，并用 PP_IS_EMPTY() 检查拼接结果 展开后是否为空 只有 SYMBOL
// 是单个符号 void，才能展开为空

// 借助 PP_IS_EMPTY()，我们还可以 检查符号序列 是否是元组：
#define PP_EMPTY_V(...)
#define PP_IS_PARENS(SYMBOL) PP_IS_EMPTY(PP_EMPTY_V SYMBOL)

  PP_IS_PARENS()               // -> 0
  PP_IS_PARENS(foo)            // -> 0
  PP_IS_PARENS(foo())          // -> 0
  PP_IS_PARENS(() foo)         // -> 0
  PP_IS_PARENS(())             // -> 1
  PP_IS_PARENS((foo))          // -> 1
  PP_IS_PARENS(((), foo, bar)) // -> 1

  // 先定义一个辅助宏 PP_EMPTY_V()：用于 吃掉 变长参数，展开为空
  // 再通过 PP_IS_EMPTY() 检查 PP_EMPTY_V SYMBOL 拼接结果 展开后是否为空
  // 只有 SYMBOL 符合 (...) 的形式，PP_EMPTY_V (...) 才能展开为空
  // 在 gmock-1.10.0 中，MOCK_METHOD() 借助 PP_IS_PARENS()，自动识别 参数是不是
  // 元组，再进行 选择性的 元组解包 —— 使用时可以只把 包含逗号的参数
  // 变为元组，而其他参数保持不变：
}

// 递归重入
{
// 因为 自参照宏 (self referential macro)
// 不会被展开 —— 在展开一个宏时，如果遇到 当前宏 的符号，则不会继续展
// 开，避免 无限展开 (infinite expansion) —— 所以宏 不支持 递归/重入。
// 例如，PP_FOR_EACH() 在遍历 两层嵌套元组 时，DO_EACH_1() 无法展开
// 内层元组，结果保留 PP_FOR_EACH(...) 的形式：
#define OUTER(N, T) PP_FOR_EACH(DO_EACH_1, N, PP_REMOVE_PARENS(T))

#define DO_EACH_1(VAR, IDX, CTX)                                               \
  PP_FOR_EACH(DO_EACH_2,                                                       \
              CTX.PP_GET_TUPLE(0, VAR),                                        \
              PP_REMOVE_PARENS(PP_GET_TUPLE(1, VAR)))

#define DO_EACH_2(VAR, IDX, CTX) CTX.VAR = VAR;

  // -> PP_FOR_EACH(DO_EACH_2, obj.x, x1, x2) PP_FOR_EACH(DO_EACH_2, obj.y, y1)
  OUTER(obj, ((x, (x1, x2)), (y, (y1))))

// 一种解决方法是，在 预扫描 阶段，先展开 内层元组，再把展开结果
// 作为参数，传递给 外层元组，从而避免 递归调用（但不一定适用于所有场景）：
#define OUTER(N, T) PP_FOR_EACH(DO_EACH_1, N, PP_REMOVE_PARENS(T))

#define DO_EACH_1(VAR, IDX, CTX) CTX.VAR;

#define INNER(N, T) PP_FOR_EACH(DO_EACH_2, N, PP_REMOVE_PARENS(T))
#define DO_EACH_2(VAR, IDX, CTX) PP_COMMA_IF(IDX) CTX.VAR = VAR
  // -> obj.x.x1 = x1; obj.x.x2 = x2; obj.y.y1 = y1;
  OUTER(obj, (INNER(x, (x1, x2)), INNER(y, (y2))))

// 另一种解决方法是，定义另一个相同功能的宏 PP_FOR_EACH_INNER()，用于内层循环，
// 从而避免和外层循环冲突（如果遍历三层嵌套，则需要再定义一个类似的宏）：
#define PP_FOR_EACH_INNER(DO, CTX, ...)                                        \
  PP_CONCAT(PP_FOR_EACH_INNER_, PP_NARG(__VA_ARGS__))                          \
  (DO, CTX, 0, __VA_ARGS__)
#define PP_FOR_EACH_INNER_0(DO, CTX, IDX, ...)
#define PP_FOR_EACH_INNER_1(DO, CTX, IDX, VAR, ...) DO(VAR, IDX, CTX)
#define PP_FOR_EACH_INNER_2(DO, CTX, IDX, VAR, ...)                            \
  DO(VAR, IDX, CTX)                                                            \
  PP_FOR_EACH_INNER_1(DO, CTX, PP_INC(IDX), __VA_ARGS__)
  // ...

#define OUTER(N, T) PP_FOR_EACH(DO_EACH_1, N, PP_REMOVE_PARENS(T))
#define DO_EACH_1(VAR, IDX, CTX)                                               \
  PP_FOR_EACH_INNER(DO_EACH_2,                                                 \
                    CTX.PP_GET_TUPLE(0, VAR),                                  \
                    PP_REMOVE_PARENS(PP_GET_TUPLE(1, VAR)))
#define DO_EACH_2(VAR, IDX, CTX) CTX.VAR = VAR;

  // -> obj.x.x1 = x1; obj.x.x2 = x2; obj.y.y1 = y1;
  OUTER(obj, ((x, (x1, x2)), (y, (y1))))
}

// 条件循环
{
  // 上文提到的 PP_FOR_EACH() 主要用于 遍历 变长参数的元素，输出长度和输入相同。
  // 但有时候，我们仍需要一个用于 迭代 (iterate) 的 条件循环
  // PP_WHILE()，最后只输出一个结果：
#define PP_WHILE PP_WHILE_1
#define PP_WHILE_1(PRED, OP, VAL)                                              \
  PP_IF(PRED(VAL), PP_WHILE_2, VAL PP_EMPTY_V)                                 \
  (PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_2(PRED, OP, VAL)                                              \
  PP_IF(PRED(VAL), PP_WHILE_3, VAL PP_EMPTY_V)                                 \
  (PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_3(PRED, OP, VAL)                                              \
  PP_IF(PRED(VAL), PP_WHILE_4, VAL PP_EMPTY_V)                                 \
  (PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_4(PRED, OP, VAL)                                              \
  PP_IF(PRED(VAL), PP_WHILE_5, VAL PP_EMPTY_V)                                 \
  (PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
#define PP_WHILE_5(PRED, OP, VAL)                                              \
  PP_IF(PRED(VAL), PP_WHILE_6, VAL PP_EMPTY_V)                                 \
  (PRED, OP, PP_IF(PRED(VAL), OP, PP_EMPTY_V)(VAL))
  // ...
#define PRED(VAL) PP_GET_TUPLE(1, VAL)
#define OP(VAL)                                                                \
  (PP_GET_TUPLE(0, VAL) + PP_GET_TUPLE(1, VAL), PP_DEC(PP_GET_TUPLE(1, VAL)))

  PP_GET_TUPLE(0, PP_WHILE(PRED, OP, (x, 3))) // -> x + 2 + 1
  PRED((x, 2))  // -> 2
  OP((x, 3)) // (x + 3, 2)
}
