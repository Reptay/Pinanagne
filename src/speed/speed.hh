#ifndef SPEED_HH
#define SPEED_HH

class Snapshot
{
public:
  Snapshot(double x, double y, double z, double t)
    :x_(x), y_(y), z_(z) t_(t)
  {}
  double operator-(const Snapshot& other) const;//compute speed
private:
  double x_;
  double y_;
  double z_;
  double t_;
}

#endif // !SPEED_HH
