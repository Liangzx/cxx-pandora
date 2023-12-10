#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>
#include <string>

using namespace boost::multi_index;

// 定义一个Person结构体
struct Person {
  std::string name;
  int age;
  std::string address;

  Person(const std::string &name, int age, const std::string &address)
      : name(name), age(age), address(address) {}
};

// 定义multi_index_container
typedef boost::multi_index_container<
    Person,
    indexed_by<ordered_unique<member<Person, std::string, &Person::name>>,
               ordered_non_unique<member<Person, int, &Person::age>>,
               hashed_unique<member<Person, std::string, &Person::address>>>>
    PersonContainer;

int main() {
  // 创建一个PersonContainer对象
  PersonContainer persons;

  // 向容器中插入一些Person对象
  persons.insert(Person("John", 25, "123 Main St"));
  persons.insert(Person("Alice", 30, "456 Elm St"));
  persons.insert(Person("Bob", 35, "789 Oak St"));

  // 按照name索引遍历
  std::cout << "Index by name:" << std::endl;
  const auto &name_index = persons.get<0>();
  for (const auto &person : name_index) {
    std::cout << person.name << std::endl;
  }

  // 按照age索引遍历
  std::cout << "Index by age:" << std::endl;
  const auto &age_index = persons.get<1>();
  for (const auto &person : age_index) {
    std::cout << person.name << " (" << person.age << " years old)"
              << std::endl;
  }

  // 按照address索引查找
  std::cout << "Index by address:" << std::endl;
  const auto &address_index = persons.get<2>();
  auto it = address_index.find("456 Elm St");
  if (it != address_index.end()) {
    const auto &person = *it;
    std::cout << person.name << " (age: " << person.age << ")" << std::endl;
  }

  return 0;
}

// https://blog.csdn.net/AAA123524457/article/details/83789113
