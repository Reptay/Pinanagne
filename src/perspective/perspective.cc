#include "perpective.hh"
# include <cmath>

void PerspectiveDetector::computePos(double x1, double y1, double x2, double y2)
{
  double psi = std::atan(hypot(x1, y1) * scale_ / focal_);
  double gamma = std::atan(hypot(x2, y2) * scale_ / focal_) - psi;

  r_ = std::sin(psi - gamma) * length_ / std::sin(psi);
  //r_ = std::hypot(focal_, hypot(x1, y1) * scale_) * length_
  //  / (std::hypot(x1-x2, y1-y2) * scale_); //vetical detection
  theta_ = std::atan2(x1, focal_); //actually pi/2 - theta => replace cos by sin
  phi_ = atan2(y1, focal_);
}
