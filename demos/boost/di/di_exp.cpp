// https://blog.csdn.net/weixin_43862847/article/details/122341510

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// 模板 SFINAE  判断是否存在自定注册函数
// 可以修改为宏支持更多函数
template <typename T>
class has_register_func {
    template <typename C = T>
    static true_type test(decltype(&C::customRegistThis));

    template <typename C = T>
    static false_type test(...);

public:
    enum {
        value = is_same<decltype(test(0)), true_type>::value
    };
};

// 基类，可根据业务修改添加接口
class BaseObject {
public:
    virtual ~BaseObject() {};
    virtual void process() = 0;
};

// DI容器
class DIContainer {
public:
    using FuncType = function<shared_ptr<BaseObject>()>;
    // 单例，低于c++11 不保证线程安全
    static DIContainer& instance()
    {
        static DIContainer fac;
        return fac;
    }

    bool registerType(string key, FuncType type)
    {
        if (m_map.find(key) != m_map.end()) {
            throw invalid_argument("this key has exist");
        }
        return m_map.emplace(key, type).second;
    }

    shared_ptr<BaseObject> resolve(string key)
    {
        if (m_map.find(key) == m_map.end()) {
            return nullptr;
        }
        auto func = m_map[key];
        return func();
    }
    map<string, FuncType> m_map;
};

// CRTP 自动注册模板,  具体实现继承该类以自动注册到DI容器中
template <typename T>
class AutoRegister : public BaseObject {
public:
    AutoRegister()
    {
        m_register.doNothing(); // m_register强制实例化，c++标准 模板类的隐式实例化不会导致模板类的静态成员实例化
    }

    struct register_exec {

        // ENABLE_IF 参数校验，派生类存在自定义注册方法，按照派生类的实现注册方法进行
        template <typename T1 = T, typename enable_if<has_register_func<T1>::value, int>::type = 0>
        register_exec()
        {
            if (!T1::customRegistThis()) {
                // assert(false);
            }
        }

        // ENABLE_IF 参数校验，不存在自定义注册方法时走默认
        // 需要注意c++标准没有规定typeid().name()的具体实现，再MSVC中为可读性字符串
        // 但是其他类似GUN，可能只具有唯一性，但是不具备可读性!
        template <typename T1 = T, typename enable_if<!has_register_func<T1>::value, int>::type = 0>
        register_exec()
        {
            DIContainer::instance().registerType(typeid(T1).name(), []() { return make_shared<T1>(); });
        }
        void doNothing() { }
    };

    static register_exec m_register;
};

// 模板类静态成员定义
template <typename T>
typename AutoRegister<T>::register_exec AutoRegister<T>::m_register;

// 实现类实现，注意提供自定义构造函数
class Test : public AutoRegister<Test> {
public:
    Test() {}; // 需要注意派生类需要提供自定义构造函数、或者在程序中有显示构造对象(如new test() make_shared<test>()等)，才会执行模板注册
    void process()
    {
        std::cout << 1 << endl;
    }

    static bool customRegistThis()
    {
        return DIContainer::instance().registerType(kModuleName, []() { return std::make_shared<Test>(); });
    }
    constexpr static char* kModuleName = "user_define";
};

class Test1 : public AutoRegister<Test1> {
public:
    Test1() {}; //统一提供构造函数，该类如果不提供无法自动注册
    void process()
    {
        std::cout << 2 << endl;
    }
};

class Test2 : public AutoRegister<Test2> {
public:
    Test2(std::shared_ptr<BaseObject> inner)
        : m_inner(inner) {};
    void process()
    {
        std::cout << "preprocesser"
                  << " ";
        m_inner->process();
    }
    static bool customRegistThis()
    {
        return DIContainer::instance().registerType(kModuleName, []() { return std::make_shared<Test2>(DIContainer::instance().resolve("user_define")); });
    }
    constexpr static char* kModuleName = "user_define_1";

private:
    std::shared_ptr<BaseObject> m_inner;
};

int main()
{

    std::cout << "class  test has register:" << has_register_func<Test>::value << endl
              << "class  test1 has register:" << has_register_func<Test1>::value << endl
              << "class  test2 has register:" << has_register_func<Test2>::value << std::endl;

    // 这部分可以从配置文件读取
    vector<string> input;
    std::cout << DIContainer::instance().m_map.size() << std::endl;
    for (auto& t : DIContainer::instance().m_map) {
        std::cout << t.first << std::endl;
        input.emplace_back(t.first);
    }

    // 构建处理链
    vector<shared_ptr<BaseObject>> handlechain;
    handlechain.reserve(input.size());
    for (const auto& key : input) {
        handlechain.emplace_back(move(DIContainer::instance().resolve(key)));
    }

    // 执行
    for (const auto& node : handlechain) {
        node->process();
    }
}
