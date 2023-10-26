#include "circle.hpp"

#include <cmath>

Circle::Circle(const Point & center, const double radius) :
    center(center), radius(radius) {
}

void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}

double Circle::intersectionArea(const Circle & otherCircle) {
  double d = center.distanceFrom(otherCircle.center);

  if (d >= radius + otherCircle.radius) {
    return 0.0;  // Circles do not intersect
  }

  if (d <= fabs(radius - otherCircle.radius)) {
    // One circle is entirely within the other
    double smallerRadius = (radius < otherCircle.radius) ? radius : otherCircle.radius;
    return M_PI * smallerRadius * smallerRadius;
  }

  // Calculate the areas of intersection

  double angleA =
      2 * acos((radius * radius + d * d - otherCircle.radius * otherCircle.radius) /
               (2 * radius * d));
  double angleB =
      2 * acos((otherCircle.radius * otherCircle.radius + d * d - radius * radius) /
               (2 * otherCircle.radius * d));

  double areaA = 0.5 * angleA * radius * radius - 0.5 * radius * radius * sin(angleA);
  double areaB = 0.5 * angleB * otherCircle.radius * otherCircle.radius -
                 0.5 * otherCircle.radius * otherCircle.radius * sin(angleB);

  return areaA + areaB;
}
