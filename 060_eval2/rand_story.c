#include "rand_story.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
/* Simply print each lines of the story*/
void printStory(FILE * fp, char ** story, int numLines) {
  for (int i = 0; i < numLines; i++) {
    fprintf(fp, "%s", story[i]);
  }
}
/* Free the heap memory of the story*/
void freeStory(char ** story, int numLines) {
  for (int i = 0; i < numLines; i++) {
    free(story[i]);
  }
  free(story);
}
/* Free a category */
void freeCategory(category_t * cat) {
  free(cat->name);
  for (size_t j = 0; j < cat->n_words; j++) {
    free(cat->words[j]);
  }
  free(cat->words);
}
/* Free the heap memory of the catarray */
void freeCatarray(catarray_t * cats) {
  /*The catarray_t itself is not on heap*/
  for (size_t i = 0; i < cats->n; i++) {
    category_t * cat = cats->arr + i;
    freeCategory(cat);
  }
  free(cats->arr);
}
/* Choose word according to the key, considering cache*/
char * chooseWordWithCache(char * key, catarray_t * cats, category_t * cache) {
  char * endptr = NULL;
  size_t cache_index = (size_t)strtol(key, &endptr, 10);
  if (*endptr == '\0' && isdigit(*key) && cache_index > 0 &&
      cache_index <= cache->n_words) {
    return cache->words[cache->n_words - cache_index];
  }
  return (char *)chooseWord(key, cats);
}
/* copy a new word and add it into the cache */
void updateCache(char * word, category_t * cache) {
  cache->words = realloc(cache->words, sizeof(*cache->words) * (cache->n_words + 1));
  char * word_copy = strdup(word);
  cache->words[cache->n_words] = word_copy;
  cache->n_words++;
}
/* Remove and free a word from a category in a catarray iff the word resides in the catarray
   i.e. do not remove or free the word if the word is from cache
*/
void removeAndFreeWordFromCatarray(char * word, char * categoryName, catarray_t * cats) {
  // it is guaranteed that the category is inside the catarray and the exactly same word pointer is inside the category
  for (int i = 0; i < cats->n; i++) {
    // Find the category by matching names
    if (strcmp(cats->arr[i].name, categoryName) == 0) {
      char ** words = cats->arr[i].words;
      for (int j = 0; j < cats->arr[i].n_words; j++) {
        // Find the word to be removed by matching pointers
        if (word == words[j]) {
          // Remove by swapping with the last element
          words[j] = words[cats->arr[i].n_words - 1];
          cats->arr[i].words =
              realloc(cats->arr[i].words,
                      sizeof(*cats->arr[i].words) * (cats->arr[i].n_words - 1));
          cats->arr[i].n_words--;
          free(word);
          break;
        }
      }
      break;
    }
  }
}
/* Convert a line w/wo blanks to a story line, return null if invalid line exists*/
char * translateLine(char * line, catarray_t * cats, category_t * cache, int isReusable) {
  // Error Checking: check the number of segments split by '_'
  int numSegs = 1;
  char * pr = line;
  while (*pr != '\0') {
    if ('_' == *pr) {
      numSegs++;
    }
    pr++;
  }
  if (numSegs % 2 == 0) {
    return NULL;  // return NULL as error
  }
  // replace '_'s by '\0's and record all the segments
  char * segs[numSegs];
  segs[0] = line;
  pr = line;
  int currentIndex = 1;
  while (*pr != '\0') {
    if (*pr == '_') {
      *pr = '\0';
      segs[currentIndex++] = pr + 1;
    }
    pr++;
  }
  // concatenate strings or the mapped strings
  int charCount = 1;
  char * newLine = malloc(charCount);
  newLine[0] = '\0';
  for (int i = 0; i < numSegs; i++) {
    // obtain the correct string to be added to the newLine
    char * translatedSeg;
    if (i % 2 == 0) {
      // case where not wrapped by dashes
      translatedSeg = segs[i];
    }
    else {
      // wrapped by dashes
      translatedSeg = chooseWordWithCache(segs[i], cats, cache);
      updateCache(translatedSeg, cache);
    }
    // concatenate the string
    charCount += strlen(translatedSeg);
    newLine = realloc(newLine, charCount);
    strcat(newLine, translatedSeg);
    // In the case where the word is translated, not reusable and obtained from catarray, remove and free the word
    if (i % 2 != 0 && !isReusable) {
      removeAndFreeWordFromCatarray(translatedSeg, segs[i], cats);
    }
  }
  return newLine;
}
category_t * computeCategoryIfAbsent(catarray_t * cats, char * key) {
  for (size_t i = 0; i < cats->n; i++) {
    if (strcmp(cats->arr[i].name, key) == 0) {
      return cats->arr + i;
    }
  }
  cats->arr = realloc(cats->arr, (cats->n + 1) * sizeof(*cats->arr));
  cats->arr[cats->n].name = strdup(key);
  cats->arr[cats->n].n_words = 0;
  cats->arr[cats->n].words = NULL;
  cats->n++;
  return cats->arr + cats->n - 1;
}
/*Add a word into a category*/
void addWordIntoCategory(category_t * cat, char * val) {
  // Assume allowing duplicates
  cat->words = realloc(cat->words, (cat->n_words + 1) * sizeof(*cat->words));
  cat->words[cat->n_words] = strdup(val);
  cat->n_words++;
}
/* parse a line in word file and add it into a catarray*/
int parseLineIntoCatarray(char * line, catarray_t * cats) {
  int numColons = 0;
  char * pr = line;
  char * val = line;
  while (*pr != '\0') {
    if (*pr == ':') {
      numColons++;
      val = pr + 1;
      *pr = '\0';
    }
    else if (*pr == '\n') {
      /*The input line is guaranteed to have a \n in the end*/
      *pr = '\0';
    }
    pr++;
  }
  if (numColons != 1) {
    return 1;  //error
  }
  // fetch category from catarray by key
  category_t * cat = computeCategoryIfAbsent(cats, line);
  // add word into category by val
  addWordIntoCategory(cat, val);
  return 0;
}
/* Parse the file of available words and convert them into a catarray_t */
catarray_t parseWords(char * fileName) {
  if (fileName == NULL) {
    fprintf(stderr, "NULL argument unexpectedly detected\n");
    exit(EXIT_FAILURE);
  }
  FILE * fp = fopen(fileName, "r");
  if (fp == NULL) {
    fprintf(stderr, "Unable to open the words file\n");
    exit(EXIT_FAILURE);
  }
  // initialize the catarray
  catarray_t catarray = {.arr = NULL, .n = 0};
  char * currentLine = NULL;
  size_t linecap = 0;
  while (getline(&currentLine, &linecap, fp) >= 0) {
    int hasError = parseLineIntoCatarray(currentLine, &catarray);
    if (hasError != 0) {
      fprintf(stderr, "Some line in Words file is invalid\n");
      free(currentLine);
      fclose(fp);
      exit(EXIT_FAILURE);
    }
    free(currentLine);
    currentLine = NULL;
  }
  free(currentLine);
  if (fclose(fp) != 0) {
    fprintf(stderr, "fail to close a file.\n");
    exit(EXIT_SUCCESS);
  }
  return catarray;
}

/* Convert a story template to a story */
char ** parseTemplate(int * numLinesPr,
                      char * fileName,
                      catarray_t * cats,
                      int isReusable) {
  // Examine the validity of inputs and file
  if (fileName == NULL || numLinesPr == NULL) {
    fprintf(stderr, "NULL argument unexpectedly detected\n");
    exit(EXIT_FAILURE);
  }
  FILE * fp = fopen(fileName, "r");
  if (fp == NULL) {
    fprintf(stderr, "Unable to open the template file\n");
    exit(EXIT_FAILURE);
  }
  // initialize the cache that records the used words
  category_t cache = {.name = NULL, .words = NULL, .n_words = 0};
  // initialize the story and the number of lines
  char ** story = NULL;
  *numLinesPr = 0;
  // initialize the arguments for getline
  char * currentLine = NULL;
  size_t linecap = 0;
  while (getline(&currentLine, &linecap, fp) != -1) {
    // Clean up the heap memory and exit if error is encountered when loading lines
    char * newLine = translateLine(currentLine, cats, &cache, isReusable);
    if (newLine == NULL) {
      fprintf(stderr, "Invalid line in template: %s", currentLine);
      free(currentLine);
      freeStory(story, *numLinesPr);
      fclose(fp);
      exit(EXIT_FAILURE);
    }
    story = realloc(story, (*numLinesPr + 1) * sizeof(*story));
    story[*numLinesPr] = newLine;
    *numLinesPr += 1;
    free(currentLine);
    currentLine = NULL;
  }
  free(currentLine);
  freeCategory(&cache);
  //close the file
  if (fclose(fp) != 0) {
    perror("Fail to close the file/n");
    exit(EXIT_FAILURE);
  }

  return story;
}
