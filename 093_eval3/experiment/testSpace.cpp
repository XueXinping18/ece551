#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int main() {
  string s = " -99 12345                awda ";
  stringstream ss(s);
  size_t i;
  ss >> i;
  if (ss) {
    cout << "good" << endl;
  }

  char c;
  ss.get(c);
  ss.get(c);
  cout << i << endl;
  cout << c << endl;
  stringstream ss2("word1 word2 word3");
  string s2;
  ss2 >> s2;
  ss2 >> s2;
  cout << ss2.eof() << endl;
  ss2 >> s2;
  cout << ss2.eof() << endl;
  cout << ss2.fail() << endl;
  ss2 >> s2;
  cout << ss2.eof() << endl;
  cout << ss2.fail() << endl;
  char ** x = NULL;
  string astring = "-100";
  const char * sss = astring.c_str();
  cout << strtoul(sss, x, 10) << endl;
}
