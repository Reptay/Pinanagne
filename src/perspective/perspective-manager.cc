#include "perspective-manager.hh"

#define FOCAL 0.003 //focal distance
#define SENSOR_SIZE 0.006 // Sensor size (width 0.0048, height 0.0036)
#define SENSOR_PXL 2769.063379556344//Sensor size in pixel (2304x1536)

#define SCALE SENSOR_SIZE / SENSOR_PXL

PerspectiveManager::PerspectiveManager()
{
  PerspectiveDetector* tmp = new PerspectiveDetector(3, FOCAL, SCALE);
  //T1 T2 and T3 line are all 3m long
  detectors_[LineType.T1] = tmp;
  detectors_[LineType.T2] = tmp;
  detectors_[LineType.T3] = tmp;

  tmp = new PerspectiveDetector(39, FOCAL, SCALE);
  //T4 has 39M lines
  detectors_[LineType.T4] = tmp;

  tmp = new PerspectiveDetector(1.5, FOCAL, SCALE);
  //Tp1 has 1.5m lines
  detectors_[LineType.Tp1] = tmp;

  tmp = new PerspectiveDetector(20, FOCAL, SCALE);
  //Tp3 has 20m lines
  detectors_[LineType.Tp3] = tmp;
}

PerspectiveManager::~PerspectiveManager()
{
  for (PerspectiveDetector* p : detectors_)
    delete p;
}

Snapshot* PerspectiveManager::position(double x1, double y1, double x2,
                                       double y2, double t, LineType line)
{
  PerspectiveDetector* det = detectors_[line];
  det->computePos(x1, x2, y1, y2);
  return new Snapshot(det->getX(), det->getY(), det->getZ(), t);
}
