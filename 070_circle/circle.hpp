#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "point.hpp"  // Assuming you have a point.hpp file

class Circle {
 private:
  Point center;
  const double radius;

 public:
  // Constructor
  Circle(const Point & center, const double radius);

  // Method to move the circle's center
  void move(double dx, double dy);

  // Method to compute the area of intersection
  double intersectionArea(const Circle & otherCircle);
};

#endif  // CIRCLE_HPP
