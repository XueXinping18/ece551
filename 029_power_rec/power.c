#include <stdio.h>
unsigned int power(unsigned int x, unsigned int y) {
  if (y == 0)
    return 1;
  return x * power(x, y - 1);
}
