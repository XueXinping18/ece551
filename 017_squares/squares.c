#include <stdio.h>
int max(int a, int b) {
  return a > b ? a : b;
}
int between(int x, int a, int b) {
  if (x < a || x >= b)
    return 0;
  return 1;
}
int hit_margin(int x, int a, int b) {
  if (x == a || x == b - 1)
    return 1;
  return 0;
}
void squares(int size1, int x_offset, int y_offset, int size2) {
  //compute the max of size1 and (x_offset + size2).  Call this w
  int w = max(size1, x_offset + size2);
  //compute the max of size1 and (y_offset + size2).  Call this h
  int h = max(size1, y_offset + size2);
  //count from 0 to h. Call the number you count with y
  for (int y = 0; y < h; y++) {
    //count from 0 to w. Call the number you count with x
    for (int x = 0; x < w; x++) {
      //check if  EITHER
      //    ((x is between x_offset  and x_offset +size2) AND
      //     y is equal to either y_offset OR y_offset + size2 - 1 )
      //  OR
      //    ((y is between y_offset and y_offset + size2) AND
      //     x is equal to either x_offset OR x_offset + size2 -1)
      // if so, print a *

      //if not,
      // check if EITHER
      //    x is less than size1 AND (y is either 0 or size1-1)
      // OR
      //    y is less than size1 AND (x is either 0 or size1-1)
      //if so, print a #

      //else print a space
      //when you finish counting x from 0 to w,
      //print a newline
      if ((between(x, x_offset, x_offset + size2) &&
           hit_margin(y, y_offset, y_offset + size2)) ||
          (between(y, y_offset, y_offset + size2) &&
           hit_margin(x, x_offset, x_offset + size2)))
        printf("*");
      else if ((x < size1 && hit_margin(y, 0, size1)) ||
               (y < size1 && hit_margin(x, 0, size1)))
        printf("#");
      else
        printf(" ");
    }
    printf("\n");
  }
}
