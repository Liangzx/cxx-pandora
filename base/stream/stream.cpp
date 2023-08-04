#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;

// https://blog.csdn.net/gongluck93/article/details/73844582
void istreambuf_iterator_demo()
{
    ifstream in("stream.cpp");
    istreambuf_iterator<char> isb(in), end;
    ostreambuf_iterator<char> osb(cout);

    while (isb != end)
        *osb++ = *isb++;
    cout << endl;

    return;
}

void istream_iterator_demo()
{
    ifstream in("stream.cpp");
    istream_iterator<char> isb(in), end;
    ostream_iterator<char> osb(cout);
    while (isb != end)
        *osb++ = *isb++;
    cout << endl;
    return;
}

int main()
{
    // istreambuf_iterator_demo();
    istream_iterator_demo();

    return 0;
}
