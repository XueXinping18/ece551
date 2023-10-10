#include "kv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readKV(char * string, kvpair_t * kvpair) {
  int i = 0;
  int equalIndex = -1;
  while (string[i] != '\n' && string[i] != '\0') {
    if (equalIndex == -1 && string[i] == '=') {
      string[i] = '\0';
      equalIndex = i;
    }
    i++;
  }
  // clean the \n into \0
  if (string[i] == '\n')
    string[i] = '\0';
  kvpair->key = string;
  kvpair->value = string + equalIndex + 1;
  return;
}

kvarray_t * readKVs(const char * fname) {
  //WRITE ME
  // open file
  FILE * fp = fopen(fname, "r");
  if (fp == NULL) {
    fprintf(stderr, "fail to open the KV file");
  }
  // create ks array on the stack
  kvarray_t * kvs = malloc(sizeof(*kvs));
  kvs->size = 0;
  kvs->pairs = malloc(0 * sizeof(*kvs->pairs));
  // initialize the array
  size_t default_string_cap = 10;
  char ** string_buffer = malloc(1 * sizeof(*string_buffer));
  *string_buffer = NULL;
  while (getline(string_buffer + kvs->size, &default_string_cap, fp) != -1) {
    kvs->size++;
    default_string_cap = 0;
    // size up the string buffers and kv pairs
    string_buffer = realloc(string_buffer, (kvs->size + 1) * sizeof(*string_buffer));
    string_buffer[kvs->size] = NULL;
    kvs->pairs = realloc(kvs->pairs, kvs->size * sizeof(*kvs->pairs));
    // read kv into the new kv pair
    readKV(string_buffer[kvs->size - 1], kvs->pairs + kvs->size - 1);
  }
  // To avoid memory leak, the additional string in the string buffer must be freed BEFORE return
  free(*(string_buffer + kvs->size));
  // The pointers to string in string_buffer is allocated on heap for resizability. They must be freed before return
  free(string_buffer);

  // close the file
  if (fclose(fp) != 0) {
    perror("Fail to close the KV file");
  }
  return kvs;
}

void freeKVs(kvarray_t * pairs) {
  // free strings, note that every free will free both the key and value because the split is conducted in place.
  for (int i = 0; i < pairs->size; i++) {
    free((pairs->pairs + i)->key);
  }
  // free all kvpairs : once is enough as them are resized by realloc instead of malloc
  free(pairs->pairs);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  //WRITE ME
  for (int i = 0; i < pairs->size; i++) {
    printf("key = '%s' value = '%s'\n", pairs->pairs[i].key, pairs->pairs[i].value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  //WRITE ME
  for (int i = 0; i < pairs->size; i++) {
    if (strcmp(key, pairs->pairs[i].key) == 0)
      return pairs->pairs[i].value;
  }
  return NULL;
}
