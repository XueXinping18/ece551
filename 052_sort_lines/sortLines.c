#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//This function is used to figure out the ordering of the strings
//in qsort. You do not need to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void read_sort_print(FILE * fp);
int main(int argc, char ** argv) {
  //WRITE YOUR CODE HERE!
  // argc is guaranteed to be at least 1
  int n = argc == 1 ? 1 : argc - 1;
  FILE * streams[n];
  // read all files
  if (argc == 1) {
    streams[0] = stdin;
  }
  else {
    for (int i = 0; i < n; i++) {
      streams[i] = fopen(argv[i + 1], "r");
      if (streams[i] == NULL) {
        fprintf(stderr, "Unable to open the file: %s\n", argv[i + 1]);
        for (int j = 0; j < i; j++) {
          fclose(streams[j]);
        }
        exit(EXIT_FAILURE);
      }
    }
  }
  // perform all other steps
  for (int i = 0; i < n; i++) {
    read_sort_print(streams[i]);
    if (streams[i] != stdin) {
      int success = fclose(streams[i]);
      if (success != 0) {
        fprintf(stderr, "Unable to close the file!");
        exit(EXIT_FAILURE);
      }
    }
  }
  return EXIT_SUCCESS;
}

void read_sort_print(FILE * fp) {
  size_t length_cap = 4;
  int line_cap = 4;
  char ** line_pointers = malloc(line_cap * sizeof(*line_pointers));
  for (int k = 0; k < line_cap; k++) {
    line_pointers[k] = NULL;
  }
  int i = 0;
  while (getline(line_pointers + i, &length_cap, fp) != -1) {
    if (i == line_cap - 1) {
      line_cap *= 2;
      line_pointers = realloc(line_pointers, line_cap * sizeof(*line_pointers));
      for (int k = line_cap / 2; k < line_cap; k++) {
        line_pointers[k] = NULL;
      }
    }
    i++;
  }
  sortData(line_pointers, i);
  // print and free
  for (int j = 0; j < i; j++) {
    printf("%s", line_pointers[j]);
    free(line_pointers[j]);
  }
  free(line_pointers[i]);
  free(line_pointers);
}
