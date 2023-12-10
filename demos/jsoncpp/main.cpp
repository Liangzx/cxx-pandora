// https://www.cnblogs.com/__tudou__/p/14957646.html
// https://github.com/open-source-parsers/jsoncpp

#include <json/json.h>

#include <iostream>
#include <mutex>

// void parse_mem_objects() {
//   const std::string rawString =
//       R"({"name" : "tony", "salary" : 100, "msg" : "work hard", "files" : [
//       "1.ts", "2.txt" ]})";
//   Json::Reader reader;

//   Json::Value root;
//   root["hello"].append("world");

//   if (!reader.parse(rawString, root)) {
//     std::cerr << "json parse failed\n";
//     return;
//   }
//   std::string name = root["name"].asString();
//   std::cout << "name: " << name << std::endl;

//   int salary = root["salary"].asInt();
//   std::cout << "salary: " << salary << std::endl;

//   std::string msg = root["msg"].asString();
//   std::cout << "msg: " << msg << std::endl;

//   Json::Value files = root["files"];
//   for (const auto &v : files) {
//     std::cout << v.asString() << std::endl;
//   }
// }

void rm_array() {
  const std::string rawString =
      R"({"disks":[
                {
                    "adapter_type":"VIRTIO",
                    "advanced_config":{
                        "serial_number":"2383324608183-0"
                    },
                    "device_id":"ide0",
                    "discard":false,
                    "disk_type":"VIRTUAL_DISK",
                    "format_type":"METADATA",
                    "size_gb":15
                },
                {
                    "adapter_type":"VIRTIO",
                    "advanced_config":{
                        "serial_number":"2383324608183-1"
                    },
                    "device_id":"ide1",
                    "discard":false,
                    "disk_type":"VIRTUAL_DISK",
                    "format_type":"OFF",
                    "size_gb":5
                }
            ]})";

  Json::Value root;
  Json::Reader reader;

  if (!reader.parse(rawString, root)) {
    std::cerr << "json parse failed\n";
    return;
  }
  
  for (int i = 0; i < root["disks"].size(); i++) {
    if (root["disks"][i]["device_id"].asString() == "ide0") {
      Json::Value wt;
      wt = root["disks"][i];
      root["disks"].clear();
      root["disks"].append(std::move(wt));
    }
  }

  std::cout << root.toStyledString() << std::endl;
}

// void demo_write_simple() {
//   Json::Value root;  // root
//   Json::FastWriter writer;

//   Json::Value Person;  // 子Value
//   Person["name"] = "tony";
//   Person["age"] = 22;
//   // 注意append方法的使用场景，只适用在添加数组item
//   root.append(Person);

//   std::string jsonStr = writer.write(root);  // json到字符串

//   std::cout << "demo_write_simple ==============\n";
//   std::cout << jsonStr << std::endl;
//   // output: [{"age":22,"name":"tony"}]
// }

// // 创建json字符串 新式API的使用
// std::string createJson() {
//   std::string jsonStr;
//   Json::Value root, language, mail;
//   Json::StreamWriterBuilder writerBuilder;  // 新式API
//   std::ostringstream os;

//   // 设置默认无格式化的输出
//   writerBuilder.settings_["indentation"] = "";

//   root["Name"] = "Zhangsan";
//   root["Age"] = 25;

//   language[0] = "C";
//   language[1] = "C++";
//   root["Language"] = language;

//   mail["QQ"] = "zhangsan@qq.com";
//   mail["Google"] = "san.zhang@gmail.com";
//   root["E-mail"] = mail;

//   root["Industry"] = "IT";

//   // 这里使用智能指针
//   std::unique_ptr<Json::StreamWriter> jsonWriter(
//       writerBuilder.newStreamWriter());
//   jsonWriter->write(root, &os);  // json-->stringstream
//   jsonStr = os.str();            // 转为string

//   // 无格式化的输出
//   std::cout << "Json-none:\n" << jsonStr << std::endl;
//   // 格式化的输出
//   std::cout << "Json-formatted:\n" << root.toStyledString() << std::endl;

//   return jsonStr;
// }

// bool parseJSON() {
//   const std::string rawString =
//       R"({"name" : "tony", "salary" : 100, "msg" : "work hard"})";

//   Json::Value root;
//   Json::String errs;
//   Json::CharReaderBuilder readBuilder;  //
//   std::unique_ptr<Json::CharReader> jsonRead(readBuilder.newCharReader());
//   if (!jsonRead) {
//     std::cerr << "jsonRead is null" << std::endl;
//     return false;
//   }
//   // reader将Json字符串解析到root，root将包含Json里所有子元素
//   bool ret = jsonRead->parse(
//       rawString.c_str(), rawString.c_str() + rawString.length(), &root,
//       &errs);
//   if (!ret || !errs.empty()) {
//     std::cout << "parseJsonFromString error!" << errs << std::endl;
//     return false;
//   }

//   std::cout << "parsrJSON() read from memory using object start !
//       -- -- -- -- -\n ";
//       // 看一下对象中key value
//       std::string name = root["name"].asString();
//   int salary = root["salary"].asInt();
//   std::string msg = root["msg"].asString();
//   std::cout << "name: " << name << " salary: " << salary;
//   std::cout << " msg: " << msg << std::endl;
//   std::cout << "parseJSON() read from memory using object  end !---------\n";
//   return true;
// }

template <typename T>
T ConvertTo(Json::Value &v) {
  return v.asBool();
}

class A {
  constexpr static auto a = "hello";
};

// template <typename Source, typename V>
// bool Marshal(Source &&s, V &&v);

// template <typename Source, typename V>
// bool Marshal(std::string s, V &&v) {
//   Json::Value root;
// }

// template <typename D>
// struct Serializer {
//   bool Marshal(const std::string &src) {
//     return static_cast<D *>(this)->Marshal(src);
//   }
//   Json::Value v_;
// };

// template <typename T, typename std::enable_if<&T::Marshal>::type * = nullptr>
// bool Marshal(T &&v) {
//   return false;
// }

// struct Stu {
//   int a_;
//   std::string b_;
//   bool Marshal() {}
// };

// template <class T, class = std::void_t<>>
// struct HasToStringFunction : false_type {};

// template <class T>
// struct HasToStringFunction<T, std::void_t<decltype(declval<T>().toString())>>
//     : true_type {};

int main() {
  // Stu stu;
  // Marshal(&stu);
  // Json::Value root;
  // Json::Value vv;
  // Json::Reader r;
  // //    r.parse()

  // for (const auto &v : {"hello", "world"}) {
  //   vv["hello"] = v;
  //   root["xx"].append(vv);
  // }
  // std::cout << root.toStyledString() << std::endl;

  // std::int64_t a;
  // //    parse_mem_objects();
  // std::mutex t;
  rm_array();
  return 0;
}
