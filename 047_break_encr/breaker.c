#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char ** argv) {
  // examine the command line arguments
  if (argc != 2) {
    fprintf(stderr, "The number of command line arguments are incorrect!");
    return EXIT_FAILURE;
  }
  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    perror("Could not open file");
    return EXIT_FAILURE;
  }
  // count the frequency
  int characterCounter[26] = {0};
  int c;
  while ((c = fgetc(f)) != EOF) {
    if (!isalpha(c))
      continue;
    characterCounter[c - 'a']++;
  }
  char topOccurence = 'a';
  int max = 0;
  for (int i = 0; i < 26; i++) {
    if (characterCounter[i] >= max) {
      topOccurence = 'a' + i;
      max = characterCounter[i];
    }
  }
  int result = ((topOccurence + 26) - 'e') % 26;  // might be wrong as char / integer
  if (fclose(f) != 0) {
    perror("Failed to close the input file!");
    return EXIT_FAILURE;
  }
  fprintf(stdout, "%d\n", result);
  return EXIT_SUCCESS;
}
