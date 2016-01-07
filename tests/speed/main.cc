#include <iostream>

#include "perspective-detector.hh"

#define FOCAL 0.031 //focal distance
#define SENSOR_SIZE FOCAL / 2.4 // Sensor size (width 0.0048, height 0.0036)
#define SENSOR_PXL 1534.4380078712857//Sensor size in pixel (2304x1536)

#define SCALE SENSOR_SIZE / SENSOR_PXL

using namespace std;

int main(int argc, char **argv)
{
  cout << SCALE << endl;
  int x1 = 0;
  int x2 = 0;
  int y1 = 0;
  int y2 = 0;

  PerspectiveDetector p(1.5, FOCAL, SCALE);

  cout << "first point" << endl;
  cin >> x1 >> y1;
  cout << "second point" << endl;
  cin >> x2 >> y2;

  p.computePos(x1, y1,x2, y2);
  cout << p.getDist() << endl;
}
