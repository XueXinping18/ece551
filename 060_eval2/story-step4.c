#include <stdio.h>

#include "provided.h"
#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "The number of command line arguments should be 2 or 3!\n");
    exit(EXIT_FAILURE);
  }
  if (argc == 4 && strcmp(argv[1], "-n")) {
    fprintf(stderr, "The arguments are not valid!\n");
    exit(EXIT_FAILURE);
  }
  int isReusable = argc == 3;
  char * words_file = argc == 3 ? argv[1] : argv[2];
  char * template_file = argc == 3 ? argv[2] : argv[3];
  catarray_t cats = parseWords(words_file);
  int numOfLines = 0;
  char ** story = parseTemplate(&numOfLines, template_file, &cats, isReusable);
  printStory(stdout, story, numOfLines);
  freeStory(story, numOfLines);
  freeCatarray(&cats);
  return EXIT_SUCCESS;
}
