#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void sortAndPrintLines(istream & is) {
  vector<string> vec;
  string line;
  while (getline(is, line)) {
    vec.push_back(line);
  }
  std::sort(vec.begin(), vec.end());
  for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it) {
    cout << *it << endl;
  }
}
int main(int argc, char ** argv) {
  if (argc == 1) {
    sortAndPrintLines(std::cin);
  }
  else {
    for (int i = 1; i < argc; i++) {
      std::ifstream ifs(argv[i]);
      if (!ifs.good()) {
        cerr << "Unable to open a file" << endl;
        exit(EXIT_FAILURE);
      }
      sortAndPrintLines(ifs);
      ifs.close();
    }
  }
  return EXIT_SUCCESS;
}
