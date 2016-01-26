#ifndef SPEED_HH
#define SPEED_HH

class Snapshot
{
public:
  Snapshot(double x, double y, double z, double t)
    :x_(x), y_(y), z_(z), t_(t)
  {}
  double operator-(const Snapshot& other) const;//compute speed
  static double mps_to_kph(double mps_speed)
  {
    return mps_speed * 3.6;
  }
  double get_x()
  {
    return x_;
  }
  double get_y()
  {
    return y_;
  }
  double get_z()
  {
    return z_;
  }
  double get_t()
  {
    return t_;
  }
private:
  double x_;
  double y_;
  double z_;
  double t_;
};
#endif // !SPEED_HH
