//
// Copyright (C) 2021 Vadim Zeitlin
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//

// This is the simplest possible example of using SOCI: just connect to the
// database using the provided command line argument.

/**
 * 参考：https://blog.csdn.net/littlewhite1989/article/details/54691367
 */

//#include <soci/soci.h>
//
//#include <cstdlib>
//#include <iostream>
//
//struct Person {
//    uint32_t id{};
//    std::string first_name;
//    std::string last_name;
//};
//
//namespace soci {
//    template<>
//    struct type_conversion<Person> {
//        typedef values base_type;
//        static void from_base(const values &v, indicator ind, Person &person) {
//            person.id = v.get<long long>("id");
//            person.first_name = v.get<std::string>("first_name");
//            person.last_name = v.get<std::string>("last_name");
//        }
//        static void to_base(const Person &person, values &v, indicator &ind) {
//            v.set("id", (long long) person.id);
//            v.set("first_name", person.first_name);
//            v.set("last_name", person.last_name);
//        }
//    };
//}// namespace soci


