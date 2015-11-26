#ifndef PANNEAU_HH
#define PANNEAU_HH

#include <iostream> 
#include <string>
#include <math.h>

class Panneau
{
public:
  Panneau(int limitation);
  Panneau(int limitation, long time);
  Panneau(int limitation, long minute, int seconds);
  long getTime();
  std::string getTimeMinute();
  long getLimitation();
  std::string print();
private:
  /**
   * 95 -> 1:35
   */
  std::string secondsTominute();
  int limitation_;
  long time_ = -1; // en seconde
};

#endif
