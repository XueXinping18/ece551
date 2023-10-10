#include "outname.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * computeOutputFileName(const char * inputName) {
  int n = strlen(inputName);
  char * src = ".counts";
  char * newString = malloc((n + strlen(src) + 1) * sizeof(*newString));
  strcpy(newString, inputName);
  strcat(newString, src);
  return newString;
}
