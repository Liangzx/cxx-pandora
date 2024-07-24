#include <iostream>
#include <regex>

using namespace std;


/*
正则表达式：一个指定的模式用来对文本中的字符串提供精确且灵活的匹配
*/
void Func1() {
string str;
   while (true) {
      cin >> str;
	  //regex e("abc.", regex_constants::icase);   // .   表示除了换行符之外任意字符
	  //regex e("abc?");               // ?       0个或者1个前面的字符
	  //regex e("abc*");               // *       0个或多个前面的字符
	  //regex e("abc+");               // +       1个或多个前面的字符
	  //regex e("ab[cd]*");            // [...]   方括号中任意字符
	  //regex e("ab[^cd]*");           // [^...]   任意不在方括号中的字符
	  //regex e("ab[cd]{3,5}");
	  //regex e("abc|de[\]fg]");         // |       或者
	  //regex  e("(abc)de+\\1");       // \1      第1个子串
	  //regex  e("(ab)c(de+)\\2\\1");
	  //regex e("[[:w:]]+@[[:w:]]+\.com"); // [[:w:]] ：字母，数字，下划线

	  //regex e("abc.$");                 // $   行尾
	  regex e("^abc.+", regex_constants::grep);                 // ^   行首，切换正则表达式语法


	  //bool match = regex_match(str, e);    //str和e精确匹配
	  bool match = regex_search(str, e);        //str中中包含e

	  cout << (match? "Matched" : "Not matched") << endl << endl;
   }
}

/*

正则表达式语法:

   ECMAScript    //C++默认
   basic
   extended
   awk
   grep
   egrep

regex e("^abc.+", regex_constants::grep);     //    切换语法
 */

/***************  处理子表达式 *****************/

/*
  std::match_results<>  储存详细的匹配Store the detailed matches
  smatch                string类型的详细的匹配Detailed match in string

  smatch m;
  m[0].str()   整个匹配的字符串 (同m.str(), m.str(0))
  m[1].str()   第1个子串(同m.str(1))
  m[2].str()   第2个子串
  m.prefix()   所有匹配字符串之前的部分
  m.suffix()   所有匹配字符串之后的部分
*/

void Func2() {
string str;

   while (true) {
      cin >> str;
	  smatch m;        // typedef std::match_results<string>

	  regex e("([[:w:]]+)@([[:w:]]+)\\.com");

	  bool found = regex_search(str, m, e);        //只返回第一个匹配

      cout << "m.size() " << m.size() << endl;        //size()==子匹配个数+1
	  for (int n = 0; n< m.size(); n++) {
		   cout << "m[" << n << "]: str()=" << m[n].str() << endl;
		   cout << "m[" << n << "]: str()=" << m.str(n) << endl;
			cout << "m[" << n << "]: str()=" << *(m.begin()+n) << endl;
	  }
	  cout << "m.prefix().str(): " << m.prefix().str() << endl;
	  cout << "m.suffix().str(): " << m.suffix().str() << endl;
   }
}


/**************** Regex Iterator ******************/
void RegexIterator() {
  cout << "Hi" << endl;

  string str = "boq@yahoo.com, boqian@gmail.com; bo@hotmail.com";

  regex e("([[:w:]]+)@([[:w:]]+)\\.com");

  sregex_iterator pos(str.cbegin(), str.cend(), e);
  sregex_iterator end; // 默认构造定义了past-the-end迭代器
  for (; pos != end; pos++) {
    cout << "Matched:  " << pos->str(0) << endl;
    cout << "user name: " << pos->str(1) << endl;
    cout << "Domain: " << pos->str(2) << endl;
    cout << endl;
  }
  cout << "=============================\n\n";
}

/**************** Regex Token Iterator ******************/
void RegexTokenIterator() {
  cout << "Hi" << endl;

  // string str = "Apple; Orange, {Cherry}; Blueberry";
  string str = "boq@yahoo.com, boqian@gmail.com; bo@hotmail.com";

  // regex e("[[:punct:]]+");  // 空格，数字，字母以外的可打印字符
  // regex e("[ [:punct:]]+");
  regex e("([[:w:]]+)@([[:w:]]+)\\.com");

  sregex_token_iterator pos(
      str.cbegin(), str.cend(), e,
      0); // 最后一个参数指定打印匹配结果的哪一部分，0表达整个匹配字符串，1表示第1个子串，-1表示没有匹配的部分
  sregex_token_iterator end; // 默认构造定义了past-the-end迭代器
  for (; pos != end; pos++) {
    cout << "Matched:  " << *pos << endl;
  }
  cout << "=============================\n\n";
}

/**************** regex_replace ******************/

void RegexReplace() {
    cout << "Hi" << endl;

	string str = "boq@yahoo.com, boqian@gmail.com; bo@hotmail.com";

	// regex e("([[:w:]]+)@([[:w:]]+)\\.com");
	regex e("([[:w:]]+)@([[:w:]]+)\\.com", regex_constants::grep|regex_constants::icase );

	//cout << regex_replace(str, e, "$1 is on $2");
   cout << regex_replace(str, e, "$1 is on $2", regex_constants::format_no_copy|regex_constants::format_first_only);//format_no_copy不匹配的字符部分不拷贝到新串，只匹配第一个
	cout << regex_replace(str, e, "$1 is on $2");    // $1表示第一个子串

	std::cin >> str;
}

int main() {
  RegexIterator();

  return 0;
}


// https://www.cnblogs.com/logchen/p/10199662.html
// https://blog.csdn.net/u011436427/article/details/133211136
