# include <cmath>

#include "perspective-detector.hh"

#include <iostream>

//#define S_WIDTH 2304
#define S_WIDTH 1920

#define S_HEIGHT 1080


void PerspectiveDetector::computePos(double x1, double y1, double x2, double y2)
{
  x1 = x1 - S_WIDTH / 2;
  x2 = x2 - S_WIDTH / 2;

  y1 = y1 - S_HEIGHT / 2;
  y2 = y2 - S_HEIGHT / 2;

  //double psi1 = std::atan2(hypot(x1, y1) * scale_, focal_);
  //double psi2 = std::atan2(hypot(x2, y2) * scale_, focal_);
  //double gamma = std::atan2(hypot(x2, y2) * scale_, focal_) - psi;
  //double gamma = std::acos(0) * hypot(x2 - x1, y2 - y1) / hypot(S_WIDTH, S_HEIGHT);

  double A = (((hypot(x2, y2) * scale_)/ focal_) * length_);//tan(psi2 * L)
  double a = hypot(x2 - x1, y2 - y1) * scale_;
  double c = hypot(hypot(x1,y1) * scale_, focal_);//distance O->M1

  r_ = A * c /a;

  //  std::cout << (a / A - c / r_) << std::endl;

  //r_ = std::sin(psi - gamma) * length_ / std::sin(gamma); legacy
  //r_ = std::hypot(focal_, hypot(x1, y1) * scale_) * length_
  //  / (std::hypot(x1-x2, y1-y2) * scale_); //vetical detection
  theta_ = std::atan2(x1, focal_); //actually pi/2 - theta => replace cos by sin
  phi_ = atan2(y1, focal_);
}

void PerspectiveDetector::expectNextPos(double speed, double time,
                                               double x, double y, double z)
{
  z -= speed * time;

  if (z <= focal_)
    {
      x_screen_ = -1;
      y_screen_ = -1;
      return;
    }

  x_screen_ = x * focal_ / z;
  y_screen_ = y * focal_ / z;
}
