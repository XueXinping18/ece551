#include <stdio.h>

#include "provided.h"
#include "rand_story.h"
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "The number of arguments should be 1\n");
    exit(EXIT_FAILURE);
  }
  catarray_t cats = parseWords(argv[1]);
  printWords(&cats);
  freeCatarray(&cats);
  return EXIT_SUCCESS;
}
