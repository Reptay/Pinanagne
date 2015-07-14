#include "filters.hh"


int main(int argc, char* argv[])
{
 if (argc == 2)
 {
  Mat img;
  img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  if (img.data)
  {
   BlueRedFilter(img);
   getContour(img);
   Mat cpy(img), end(img);
   getOrientation(cpy);
   
   namedWindow("Display", WINDOW_AUTOSIZE);
   imshow("Display", img);
   waitKey(0);
   imshow("Display", cpy);
   waitKey(0);
   findcenter(end,cpy);
   imshow("Display", end);
   waitKey(0);
   return 0;
  }
  return 1;
 }
 return 2;
}
