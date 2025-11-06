#include <iostream>

/**
形态|语法|展开顺序 (以参数 E1, E2, E3 为例)|
一元左折叠|(... op args)|((E1 op E2) op E3)|
一元右折叠|(args op ...)|E1 op (E2 op E3)|
二元左折叠​|(init op ... op args)|((init op E1) op E2) op E3|
二元右折叠|(args op ... op init)|E1 op (E2 op (E3 op init))|
 */

template <typename... Args> auto left_fold(Args... args) {
  return (... - args); // 一元左折叠: ((arg1 - arg2) - arg3) ...
}

template <typename... Args> auto right_fold(Args... args) {
  return (args - ...); // 一元右折叠: arg1 - (arg2 - (arg3 ...))
}

void func1() {
  // 左折叠: ((10 - 1) - 2) - 3 = (9 - 2) - 3 = 7 - 3 = 4
  std::cout << left_fold(10, 1, 2, 3) << std::endl; // 输出 4

  // 右折叠: 10 - (1 - (2 - 3)) = 10 - (1 - (-1)) = 10 - 2 = 8
  std::cout << right_fold(10, 1, 2, 3) << std::endl; // 输出 8
}

/**
11.2.1 Dealing with Empty Parameter Packs
If a fold expression is used with an empty parameter pack, the following rules
apply: • If operator && is used, the value is true. • If operator || is used,
the value is false. • If the comma operator is used, the value is void(). • For
all other operators, the call is ill-formed
 */

// 例子1：逻辑运算符 &&和 ||
/**
规则指出：
&&空包为 true
||空包为 false
这类似于数学中的“单位元”概念：true是 &&的单位元（任何值与
true相与结果不变），false是 ||的单位元。
 */

// 使用二元左折叠 (init op ... op pack)
template <typename... Args> bool all(Args... args) {
  // 等价于： return (true && args && ...);
  return (args && ...); // 一元右折叠
}

template <typename... Args> bool any(Args... args) {
  // 等价于： return (false || args || ...);
  return (args || ...); // 一元右折叠
}

void func2() {
  // 情况1：非空参数包
  std::cout << std::boolalpha;
  std::cout << all(true, true, true) << std::endl;   // 输出: true
  std::cout << any(false, true, false) << std::endl; // 输出: true

  // 情况2：空参数包 - 应用图片中的规则！
  std::cout << all() << std::endl; // 展开为：折叠空包 &&，值为 true
  std::cout << any() << std::endl; // 展开为：折叠空包 ||，值为 false
}

int main() {
  //   func1();
  func2();

  return 0;
}

/**
 fold expression 空包处理逻辑
在C++的折叠表达式（fold expression）中，当处理空参数包（empty parameter
pack）时，&& 和 || 之所以有不同的默认行为，
本质上是出于“逻辑合理性”和“数学上的幺元（identity
element）”考虑。我们可以从「逻辑运算的语义」和「折叠表达式的设计初衷」两个角度来理解：

1. 先回顾折叠表达式与空包

折叠表达式是C++17引入的语法，用来对参数包（parameter
pack）里的所有元素做“折叠”计算（比如累加、累乘、逻辑与/或等等）。它有两种形式：
• 一元折叠（unary fold）：( pack op ... ) 或 ( ... op pack )

• 二元折叠（binary fold）：( init op pack op ... ) 或 ( ... op pack op init )

当参数包为空时，就进入了“空包折叠”的场景。此时编译器需要给它一个“合理”的默认值，否则程序就会陷入“未定义”或者无意义的状态。

2. 为什么 && 空包时返回 true？

逻辑与（&&）的幺元是 true。换句话说，在逻辑运算里：
• 对任意布尔值 b，都有 true && b ≡ b

• 反过来，b && true ≡ b

如果参数包为空，相当于“没有任何元素参与 && 运算”，那么让结果等于幺元 true，
可以保证：一旦往这个空包里添加任意个元素，整个逻辑与的结果都不会因为“曾经是空包”而改变。

举个例子：假设你有一个可变参数模板，想判断“所有参数是否都是真”，写成折叠表达式就是
(args && ...)。 如果用户传参时一个参数都没传（空包），按照“幺元”的逻辑，空包的
&& 结果为 true 是最自然的——因为“没有任何元素不满足条件”，等价于“全满足”。
Ps：把“所有条件都成立”理解成“不存在不成立的条件”。空集合里当然“不存在不成立的条件”，所以为
true。

3. 为什么 || 空包时返回 false？

逻辑或（||）的幺元是 false。也就是说，在逻辑运算里：
• 对任意布尔值 b，都有 false || b ≡ b

• 反过来，b || false ≡ b

类似地，当参数包为空时，让 || 的结果等于幺元
false，就能保证：“一旦往空包里添加任意个元素，整个逻辑或的结果都不会因为‘曾经是空包’而改变”。

举个例子：假设你要判断“是否存在至少一个参数为真”，写成折叠表达式就是 (args ||
...)。 如果用户传参时一个参数都没传（空包），按照“幺元”的逻辑，空包的 || 结果为
false 是最自然的——因为“没有任何元素满足条件”，等价于“全不满足”。 Ps：||-->
“存在成立的条件”。空集合里当然“不存成立的条件”， 所以为 false。

4. 其他运算符为何“非法（ill - formed）”？

对于除 &&、||、逗号（,）之外的其他运算符（比如 +、*、^
…），C++标准并没有为它们定义“空包时的自然幺元”。
如果允许空包折叠，结果既没有数学上的合理性，也容易导致代码歧义。因此标准规定：对这些运算符做空包折叠时，程序是“病式（ill
- formed）”，编译器会直接报错。

而逗号运算符（,）比较特殊：它的返回值是“最后一个子表达式的值”，但空包里没有子表达式，所以标准规定它返回
void()（一个无操作的函数类型，也可以理解成“无值”）。

总结一下，&& 空包返回 true、|| 空包返回
false，是为了让折叠表达式在“空参数包”这种边界情况下，仍然能保持逻辑运算本身的幺元性质，
从而让代码的“空包语义”更符合直觉、更易于维护。这也体现了C++在语言设计上对“边界情况”和“数学一致性”的重视。
 */
