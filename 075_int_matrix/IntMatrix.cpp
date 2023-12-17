#include "IntMatrix.h"

IntMatrix::IntMatrix() : numRows(0), numColumns(0), rows(NULL) {
}
IntMatrix::IntMatrix(int r, int c) : numRows(r), numColumns(c) {
  rows = new IntArray *[r];
  for (int i = 0; i < r; i++) {
    rows[i] = new IntArray(c);
  }
}
IntMatrix::IntMatrix(const IntMatrix & rhs) :
    numRows(rhs.numRows), numColumns(rhs.numColumns), rows(new IntArray *[numRows]) {
  for (int i = 0; i < rhs.numRows; i++) {
    rows[i] = new IntArray(rhs[i]);
  }
}
IntMatrix::~IntMatrix() {
  for (int i = 0; i < numRows; i++) {
    delete rows[i];
  }
  delete[] rows;
}
IntMatrix & IntMatrix::operator=(const IntMatrix & rhs) {
  this->~IntMatrix();
  numColumns = rhs.numColumns;
  numRows = rhs.numRows;
  rows = new IntArray *[rhs.numRows];
  for (int i = 0; i < rhs.numRows; i++) {
    rows[i] = new IntArray(rhs[i]);
  }
  return *this;
}
int IntMatrix::getRows() const {
  return numRows;
}
int IntMatrix::getColumns() const {
  return numColumns;
}
const IntArray & IntMatrix::operator[](int index) const {
  assert(index >= 0 && index < numRows);
  return *rows[index];
}
IntArray & IntMatrix::operator[](int index) {
  assert(index >= 0 && index < numRows);
  return *rows[index];
}

bool IntMatrix::operator==(const IntMatrix & rhs) const {
  if (numRows != rhs.numRows || numColumns != rhs.numColumns) {
    return false;
  }
  for (int i = 0; i < numRows; i++) {
    if ((*this)[i] != rhs[i])
      return false;
  }
  return true;
}

IntMatrix IntMatrix::operator+(const IntMatrix & rhs) const {
  assert(numRows == rhs.numRows && numColumns == rhs.numColumns);
  IntMatrix result = IntMatrix(numRows, numColumns);
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numColumns; j++) {
      result[i][j] = (*this)[i][j] + rhs[i][j];
    }
  }
  return result;
}

std::ostream & operator<<(std::ostream & s, const IntMatrix & rhs) {
  s << "[ ";
  if (rhs.getRows() > 0)
    s << rhs[0];
  for (int i = 1; i < rhs.getRows(); i++) {
    s << ",\n";
    s << rhs[i];
  }
  s << " ]";
  return s;
}
