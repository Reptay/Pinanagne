#include "speed.hh"

double Snapshot::operator-(const Snapshot& other)
{
  double dist = this->z_ - other.z_;
  //changer pour vrai distance après changement de repere;
  double time = this->t_ - other.t_;

  return dist/time;
}
