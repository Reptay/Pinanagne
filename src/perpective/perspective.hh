#ifndef PERSPECTIVE_HH
# define PERSPECTIVE_HH

#include <opencv2/core/core.hpp>

//first try : consider the image plane as vertical
//input m0 m1 output vecteur coordonées
//r = focal * length / hypot(x1 - x2, y1 - y2) distance au milieu du segment)
//pour la distance au point 1 prendre hypoth(focal, hypoth(x1, y1))
//theta = atan(x1/ focal)
//phi = atan(y1/focal)
// this is a very naive approach.

class PerspectiveDetector
{
public:
  PerspectiveDetector(double length, double focal, double scale)
    :focal_(focal), length_(length), scale_(scale)
  {}
  void computePos(double x1, double y1, double x2, double y2);
  double getX() const
  {
    return r_ * std::cos(phi_) * std::sin(theta_); //projette sur xoz puis sur x
  }
  double getY() const
  {
    return r_* std::cos(theta_) * std::sin(phi_); //projette sur yoz puis sur y
  }
  double getZ() const
  {
    return r_ * std::cos(phi_) * std::cos(theta_); //projette sur xoz puis sur z
  }
  double getDist() const
  {
    return r_;
  }
private:
  double focal_;
  double length_;
  double scale_;
  double r_ = 0;
  double theta_ = 0;
  double phi_ = 0;
};

#endif //!PERSPECTIVE_HH
