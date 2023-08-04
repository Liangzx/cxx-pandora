#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

struct Person
{
    int age_;
    double monney_;
    std::string name_;
    friend std::ostream &operator<<(std::ostream &out, const Person &person)
    {
        out << person.age_ << person.monney_ << person.name_;
        return out;
    }
    friend std::istream &operator>>(std::istream &is, Person &person)
    {
        is >> person.age_ >> person.monney_ >> person.name_;
        return is;
    }
};

int main()
{
    
    return 0;
}
