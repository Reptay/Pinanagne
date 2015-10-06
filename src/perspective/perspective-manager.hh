#ifndef PERSPECTIVE_MANAGER_HH
# define PERSPECTIVE_MANAGER_HH

#include "perspective-detector.hh"
#include "snapshot.hh"
#include <map>

enum LineType {T1, T2, T3, T4, Tp1, Tp3};

//may be moved to a singleton later on

class PerspectiveManager
{
public:
  PerspectiveManager();
  ~PerspectiveManager();
  Snapshot* position(double x1, double y1, double x2, double y2,
                     double t, LineType line);
  PerspectiveManager(const PerspectiveManager& other) = delete;
  PerspectiveManager& operator=(const PerspectiveManager&) = delete;
private:
  std::map<LineType, PerspectiveDetector*> detectors_;
};

#endif // !PERSPECTIVE_MANAGER_HH
