#include <iostream>
#include <string>

class Product
{
public:
    virtual ~Product() = default;
    [[nodiscard]] virtual std::string Operation() const = 0;
};
//
class ConcreteProductA : public Product
{
public:
    std::string Operation() const override
    {
        return "{Result of the ConcreteProductA}";
    }
};
//
class ConcreteProductB : public Product
{
public:
    std::string Operation() const override
    {
        return "{Result of the ConcreteProductB}";
    }
};
//
class Creator
{
public:
    virtual ~Creator() {}
    //
    virtual Product *FactoryMethod() const = 0;
    //
    std::string SomeOperation() const
    {
        Product *product = this->FactoryMethod();
        std::string result = "Creator: The same creator's code has just worked with " + product->Operation();
        delete product;
        return result;
    }
};
//
class ConcreteCreatorA : public Creator
{
    Product *FactoryMethod() const override
    {
        return new ConcreteProductA();
    }
};
//
class ConcreteCreatorB : public Creator
{
    Product *FactoryMethod() const override
    {
        return new ConcreteProductB();
    }
};
//
void ClientCode(const Creator &creator)
{
    std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
              << creator.SomeOperation() << std::endl;
}
//

void func()
{
    return;
}


int main()
{
    int a = 123;
    std::cout << "App: Launched with the ConcreteCreator1.\n";
    Creator *creatorA = new ConcreteCreatorA();
    ClientCode(*creatorA);
    std::cout << std::endl;
    std::cout << "App: Launched with the ConcreteCreator2.\n";
    Creator *creatorB = new ConcreteCreatorB();
    ClientCode(*creatorB);

    auto ss = std::string{"hello"};
    std::cout <<  ss << std::endl;

    delete creatorA;
    delete creatorB;
    return 0;
}