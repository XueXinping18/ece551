#include <stdio.h>

#include "provided.h"
#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc != 3) {
    fprintf(stderr, "The number of command line arguments should be 2!\n");
    exit(EXIT_FAILURE);
  }
  catarray_t cats = parseWords(argv[1]);
  int numOfLines = 0;
  char ** story = parseTemplate(&numOfLines, argv[2], &cats, 1);
  printStory(stdout, story, numOfLines);
  freeStory(story, numOfLines);
  freeCatarray(&cats);
  return EXIT_SUCCESS;
}
