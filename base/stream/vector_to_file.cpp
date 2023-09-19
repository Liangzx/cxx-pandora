#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct Person
{
    int age_;
    double money_;
    std::string name_;
    
    friend std::ostream &operator<<(std::ostream &out, const Person &person)
    {
        out << person.age_ << person.money_ << person.name_;
        // out.write((char *)&person, sizeof(person));
        return out;
    }

    friend std::istream &operator>>(std::istream &is, Person &person)
    {
        is >> person.age_ >> person.money_ >> person.name_;
        // is.read((char *)&person, sizeof(person));
        return is;
    }
};

int main()
{
    //
    std::ifstream person_file_ifs("");
    std::vector<Person> persons();
    std::copy(std::istream_iterator<Person>(person_file_ifs), std::istream_iterator<Person>(), std::back_inserter(persons));
    std::string ss;

    return 0;
}
