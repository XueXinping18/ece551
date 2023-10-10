#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);

unsigned correctPower(unsigned x, unsigned y) {
  int start = 1;
  for (int i = 0; i < y; i++) {
    start *= x;
  }
  return start;
}
int test(unsigned x, unsigned y, unsigned expected) {
  return expected == power(x, y);
}
int main() {
  if (!test(0, 0, 1))
    exit(EXIT_FAILURE);
  if (!test(0, 2819, 0))
    exit(EXIT_FAILURE);
  if (!test(12914190, 1, 12914190))
    exit(EXIT_FAILURE);
  if (!test(1, 1000, 1))
    exit(EXIT_FAILURE);
  if (!test(1412314121, 0, 1))
    exit(EXIT_FAILURE);
  for (int x = 1; x < 1000; x = x * 2) {
    for (int y = 0; y < 10; y += 2) {
      if (!test(x, y, correctPower(x, y)))
        exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
