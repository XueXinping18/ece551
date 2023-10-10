#include "counts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
counts_t * createCounts(void) {
  //WRITE ME
  counts_t * c = malloc(sizeof(*c));
  c->counts = malloc(0);
  c->unknown_count = 0;
  c->size = 0;
  return c;
}
void addCount(counts_t * c, const char * name) {
  //WRITE ME
  // unknown name
  if (name == NULL) {
    c->unknown_count++;
    return;
  }
  int i = 0;
  // known name: already created
  for (; i < c->size; i++) {
    if (strcmp(name, c->counts[i].string) == 0) {
      c->counts[i].value++;
      break;
    }
  }
  // known name but not yet created entry
  if (i == c->size) {
    c->size++;
    c->counts = realloc(c->counts, c->size * sizeof(*c->counts));
    c->counts[c->size - 1].string = name;
    c->counts[c->size - 1].value = 1;
  }
  return;
}

void printCounts(counts_t * c, FILE * outFile) {
  for (int i = 0; i < c->size; i++) {
    fprintf(outFile, "%s: %d\n", c->counts[i].string, c->counts[i].value);
  }
  if (c->unknown_count > 0) {
    fprintf(outFile, "<unknown> : %d\n", c->unknown_count);
  }
}

void freeCounts(counts_t * c) {
  free(c->counts);
  free(c);
}
