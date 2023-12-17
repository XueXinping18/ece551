#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "cyoa.hpp"
// utility function to convert a integer to string
string toString(size_t x) {
  stringstream ss;
  ss << x;
  return ss.str();
}
// utility function to parse a variable from stringstream of form "string=long"
void getVariableFromStream(stringstream & ss, string & name, long & val) {
  getline(ss, name, '=');
  if (ss.fail() || ss.eof()) {
    throw runtime_error("No '=' sign is found while expected");
  }
  ss >> val;
  if (ss.fail()) {
    throw runtime_error("Fail to parse variable value.");
  }
}
