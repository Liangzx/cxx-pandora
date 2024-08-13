#include <boost/range/adaptors.hpp>
#include <vector>
#include <iostream>

// Define the Person struct.
struct Person {
    std::string name;
    int age;
};

// Define the Contact struct.
struct Contact {
    std::string name;
    std::string age; // We will store the age as a string for simplicity.
    Contact(const std::string& name, const std::string& age) : name(name), age(age) {}
    Contact(const Contact& other) : name(other.name), age(other.age) {}
    Contact& operator=(const Contact& other) {
        if (this != &other) {
            name = other.name;
            age = other.age;
        }
        return *this;
    }
    Contact(Contact&& other) noexcept : name(std::move(other.name)), age(std::move(other.age)) {
        std::cout << "move ctor" << std::endl;
    }
    Contact& operator=(Contact&& other) noexcept {
        if (this != &other) {
            name = std::move(other.name);
            age = std::move(other.age);
        }
        return *this;
    }
};

// A function to convert a Person to a Contact.
Contact personToContact(const Person& person) {
    return Contact{person.name, std::to_string(person.age)};
}

int main() {
    std::vector<Person> people = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35}
    };

    // Transform the vector of people into a vector of contacts.
    auto contacts = people | boost::adaptors::transformed(personToContact);

    // Print the transformed contacts.
    for (const auto& contact : contacts) {
        std::cout << "Name: " << contact.name << ", Age: " << contact.age << " age addr=" << &contact << std::endl;
    }

    std::vector<Contact> vcs;
    std::move(contacts.begin(),  contacts.end(), std::back_inserter(vcs));

    std::cout << "-----------" << std::endl;
    for (const auto& contact : contacts) {
        std::cout << "Name: " << contact.name << ", Age: " << contact.age << " age addr=" << &contact << std::endl;
    }

    std::cout << "-----------" << std::endl;
    for (const auto& contact : vcs) {
        std::cout << "Name: " << contact.name << ", Age: " << contact.age << std::endl;
    }

    return 0;
}
