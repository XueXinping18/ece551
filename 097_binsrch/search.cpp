#include "search.h"

#include <stdexcept>

int binarySearchForZero(Function<int, int> * f, int low, int high) {
  if (low >= high || high - low == 1) {
    return low;
  }
  while (low < high - 1) {
    int mid = low + (high - low) / 2;
    int midValue = f->invoke(mid);

    if (midValue == 0) {
      return mid;
    }
    else if (midValue < 0) {
      low = mid;
    }
    else {
      high = mid;
    }
  }

  return low;
}
