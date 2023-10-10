#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counts.h"
#include "kv.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
  counts_t * c = createCounts();
  size_t cap = 0;
  char * line_pointer = NULL;
  FILE * fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error found when open file.\n");
    exit(EXIT_FAILURE);
  }
  int length;
  while ((length = getline(&line_pointer, &cap, fp)) != -1) {
    line_pointer[length - 1] = '\0';
    char * val = lookupValue(kvPairs, line_pointer);
    addCount(c, val);
    // keys are no longer useful, should be freed immediately
    free(line_pointer);
    line_pointer = NULL;
    cap = 0;
  }
  // one additional free for the failure of getline
  free(line_pointer);

  if (fclose(fp) != 0) {
    perror("Error found when close file.\n");
  }
  return c;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
  //read the key/value pairs from the file named by argv[1] (call the result kv)
  if (argc < 2) {
    fprintf(stderr, "Too few arguments");
    exit(EXIT_FAILURE);
  }
  kvarray_t * kvs = readKVs(argv[1]);
  //count from 2 to argc (call the number you count i)
  for (int i = 2; i < argc; i++) {
    //count the values that appear in the file named by argv[i], using kv as the key/value pair
    //   (call this result c)
    counts_t * c = countFile(argv[i], kvs);
    //compute the output file name from argv[i] (call this outName)
    char * outName = computeOutputFileName(argv[i]);
    //open the file named by outName (call that f)
    FILE * f = fopen(outName, "w");
    if (f == NULL) {
      fprintf(stderr, "Unable to open file %s.", outName);
    }
    //print the counts from c into the FILE f
    printCounts(c, f);
    //close f
    if (fclose(f) != 0) {
      perror("Unable to close the file.\n");
    }
    //free the memory for outName and c
    free(outName);
    freeCounts(c);
  }
  //free the memory for kv
  freeKVs(kvs);
  return EXIT_SUCCESS;
}
