#include "point.hpp"

#include <cmath>
Point::Point() : x(0), y(0) {
}
void Point::move(double dx, double dy) {
  this->x += dx;
  this->y += dy;
}
double Point::distanceFrom(const Point & p) const {
  return std::sqrt((p.x - x) * (p.x - x) + (p.y - y) * (p.y - y));
}
