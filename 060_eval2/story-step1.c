#include <stdio.h>

#include "provided.h"
#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "The number of command line arguments should be 1!\n");
    exit(EXIT_FAILURE);
  }
  int numOfLines = 0;
  char ** story = parseTemplate(&numOfLines, argv[1], NULL, 1);
  printStory(stdout, story, numOfLines);
  freeStory(story, numOfLines);
  return EXIT_SUCCESS;
}
