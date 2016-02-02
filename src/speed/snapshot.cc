#include <cmath>

#include "snapshot.hh"


double Snapshot::operator-(const Snapshot& other) const
{
  double dist = hypot(this->z_ - other.z_, hypot(this->y_ - other.y_, this->x_ - other.x_));
  //changer pour vrai distance après changement de repere;
  double time = this->t_ - other.t_;

  return dist/time;
}
