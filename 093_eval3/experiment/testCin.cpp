#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int main() {
  string x = "-18446744073709551606";
  char * p;
  unsigned long b = strtoul(x.c_str(), &p, 10);
  cout << b << endl;
  return 0;
}
