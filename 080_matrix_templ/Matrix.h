#ifndef __T_MATRIX_H___
#define __T_MATRIX_H___

#include <assert.h>

#include <cstdlib>
#include <iostream>
#include <vector>
//YOUR CODE GOES HERE!
using namespace std;
template<typename T>
class Matrix {
 private:
  int numRows;
  int numColumns;
  vector<T> ** rows;

 public:
  Matrix() : numRows(0), numColumns(0), rows(NULL) {}
  Matrix(int r, int c) : numRows(r), numColumns(c) {
    rows = new vector<T> *[r];
    for (int i = 0; i < r; i++) {
      rows[i] = new vector<T>(c);
    }
  }
  Matrix(const Matrix & rhs) :
      numRows(rhs.numRows), numColumns(rhs.numColumns), rows(new vector<T> *[numRows]) {
    for (int i = 0; i < rhs.numRows; i++) {
      rows[i] = new vector<T>(rhs[i]);
    }
  }
  ~Matrix() {
    for (int i = 0; i < numRows; i++) {
      delete rows[i];
    }
    delete[] rows;
  }
  Matrix<T> & operator=(const Matrix<T> & rhs) {
    this->~Matrix();
    numColumns = rhs.numColumns;
    numRows = rhs.numRows;
    rows = new vector<T> *[rhs.numRows];
    for (int i = 0; i < rhs.numRows; i++) {
      rows[i] = new vector<T>(rhs[i]);
    }
    return *this;
  }
  int getRows() const { return numRows; }
  int getColumns() const { return numColumns; }
  const vector<T> & operator[](int index) const {
    assert(index >= 0 && index < numRows);
    return *rows[index];
  }
  vector<T> & operator[](int index) {
    assert(index >= 0 && index < numRows);
    return *rows[index];
  }

  bool operator==(const Matrix<T> & rhs) const {
    if (numRows != rhs.numRows || numColumns != rhs.numColumns) {
      return false;
    }
    for (int i = 0; i < numRows; i++) {
      if ((*this)[i] != rhs[i])
        return false;
    }
    return true;
  }

  Matrix<T> operator+(const Matrix<T> & rhs) const {
    assert(numRows == rhs.numRows && numColumns == rhs.numColumns);
    Matrix<T> result = Matrix(numRows, numColumns);
    for (int i = 0; i < numRows; i++) {
      for (int j = 0; j < numColumns; j++) {
        result[i][j] = (*this)[i][j] + rhs[i][j];
      }
    }
    return result;
  }
};
template<typename T>
std::ostream & operator<<(std::ostream & s, const Matrix<T> & rhs) {
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
template<typename T>
std::ostream & operator<<(std::ostream & s, const vector<T> & rhs) {
  s << "{";
  if (rhs.size() > 0)
    s << rhs[0];
  for (size_t i = 1; i < rhs.size(); i++) {
    s << ", ";
    s << rhs[i];
  }
  s << '}';
  return s;
}

#endif
