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
        std::cout << "Name: " << contact.name << ", Age: " << contact.age << std::endl;
    }

    return 0;
}
