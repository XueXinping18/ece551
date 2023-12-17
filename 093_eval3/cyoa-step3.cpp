#include <cstdio>
#include <cstdlib>

#include "cyoa.hpp"
int main(int argc, char ** argv) {
  if (argc != 2) {
    cerr << "number of arguments for step 2 is wrong" << endl;
    exit(EXIT_FAILURE);
  }
  string directory = argv[1];
  try {
    Story story = Story::parseStory(directory);
    story.postInstantiationCheck();
    story.printWinningPaths();
  }
  catch (const exception & e) {
    cerr << e.what() << endl;
    exit(EXIT_FAILURE);
  }
  fcloseall();
  return EXIT_SUCCESS;
}
