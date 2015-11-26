#include "panneau.hh"

Panneau::Panneau(int limitation)
  :limitation_(limitation)
{
}

Panneau::Panneau(int limitation, long time)
  :limitation_(limitation), time_(time)
{
}

Panneau::Panneau(int limitation, long minute, int seconds)
  : limitation_(limitation), time_(minute*60+seconds){}

long Panneau::getTime()
{
  return time_;
}

long Panneau::getLimitation()
{
  return limitation_;
}

std::string Panneau::print()
{
  std::string str = std::to_string(limitation_) + " " + secondsTominute();
  return str;
}

std::string Panneau::getTimeMinute()
{
  return secondsTominute();
}

std::string Panneau::secondsTominute()
{
  long mn = floor (time_/60);
  int s = time_%60;
  std::string zero = "";
  if (s < 10)
    zero = "0";
  return std::to_string(mn)+":"+zero+std::to_string(s);
}
