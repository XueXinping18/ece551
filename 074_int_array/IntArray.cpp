#include "IntArray.h"

#include <assert.h>

#include <ostream>

IntArray::IntArray() : data(NULL), numElements(0) {
}
IntArray::IntArray(int n) : data(new int[n]), numElements(n) {
}

IntArray::IntArray(const IntArray & rhs) :
    data(new int[rhs.numElements]), numElements(rhs.numElements) {
  for (int i = 0; i < numElements; i++) {
    data[i] = rhs.data[i];
  }
}
IntArray::~IntArray() {
  delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
  delete[] data;
  numElements = rhs.numElements;
  data = new int[numElements];
  for (int i = 0; i < numElements; i++) {
    data[i] = rhs.data[i];
  }
  return *this;
}
const int & IntArray::operator[](int index) const {
  assert(index >= 0 && index < numElements);
  return data[index];
}
int & IntArray::operator[](int index) {
  assert(index >= 0 && index < numElements);
  return data[index];
}

int IntArray::size() const {
  return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
  if (numElements != rhs.numElements)
    return false;
  for (int i = 0; i < numElements; i++) {
    if (rhs[i] != (*this)[i])
      return false;
  }
  return true;
}

bool IntArray::operator!=(const IntArray & rhs) const {
  return !(*this == rhs);
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  s << "{";
  if (rhs.size() > 0)
    s << rhs[0];
  for (int i = 1; i < rhs.size(); i++) {
    s << ", ";
    s << rhs[i];
  }
  s << '}';
  return s;
}
