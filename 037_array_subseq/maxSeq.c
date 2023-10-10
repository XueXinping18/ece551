#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
size_t maxSeq(int * array, size_t n) {
  if (array == NULL)
    return 0;
  size_t maxLength = 0;
  size_t currentMax = 0;
  for (int i = 0; i < n; i++) {
    if (i == 0 || array[i] > array[i - 1]) {
      currentMax++;
      maxLength = maxLength > currentMax ? maxLength : currentMax;
    }
    else {
      currentMax = 1;
    }
  }
  return maxLength;
}
