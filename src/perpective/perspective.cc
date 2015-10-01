#include "perpective.hh"
# include <cmath>

void PerspectiveDetector::computePos(double x1, double y1, double x2, double y2)
{
  r_ = std::hypot(focal_, hypot(x1, y1) * scale_) * length_
    / (std::hypot(x1-x2, y1-y2) * scale_);
  theta_ = std::atan2(x1, focal); //actually pi/2 - theta => replace cos by sin
  phi_ = atan2(y1, focal);
}
