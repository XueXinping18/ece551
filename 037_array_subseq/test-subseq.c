#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
size_t maxSeq(int * array, size_t n);
int test(int * array, size_t n);
size_t correct_maxSeq(int * array, size_t n) {
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
int main() {
  int array1[] = {1, 2, 1, 3, 3, 4, 4, 7, 2, 4, 6, -8, -6, -3, 0, 1, 5, 10, 20, -30, 50};
  size_t n1 = 21;
  int array2[] = {0};
  size_t n2 = 1;
  int array3[] = {9, 8, 7, 7, 8, 10, -12, -7, -13, -16};
  size_t n3 = 10;
  int array4[] = {-1, -2, -3, 0, 1, 3, 4, -6, 7};
  size_t n4 = 3;
  int * array5 = NULL;
  size_t n5 = 0;

  /* size_t n3 = 100; */
  /* size_t n4 = 5; */
  /* size_t n5 = 0; */
  if (!test(array1, n1))
    exit(EXIT_FAILURE);
  if (!test(array2, n2))
    exit(EXIT_FAILURE);
  if (!test(array3, n3))
    exit(EXIT_FAILURE);
  if (!test(array4, n4))
    exit(EXIT_FAILURE);
  if (!test(array5, n5))
    exit(EXIT_FAILURE);

  return EXIT_SUCCESS;
}
int test(int * array, size_t n) {
  if (maxSeq(array, n) == correct_maxSeq(array, n))
    return 1;
  return 0;
}
