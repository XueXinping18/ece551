#include "vector.hpp"

#include <cmath>
#include <cstdio>

double Vector2D::getMagnitude() const {
  return std::sqrt(x * x + y * y);
}
Vector2D Vector2D::operator+(const Vector2D & rhs) const {
  Vector2D result(x + rhs.x, y + rhs.y);
  return result;
}
Vector2D & Vector2D::operator+=(const Vector2D & rhs) {
  x += rhs.x;
  y += rhs.y;
  return *this;
}

double Vector2D::dot(const Vector2D & rhs) const {
  return x * rhs.x + y * rhs.y;
}

void Vector2D::print() const {
  std::printf("<%.2f, %.2f>", x, y);
}
