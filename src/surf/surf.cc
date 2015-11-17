#include "surf.hh"

#include "opencv2/legacy/legacy.hpp"

int findObject(Mat sceneP, Mat objectP, int minHessian, Scalar color, Mat outImg)
{
	//vector of keypoints	
	vector<cv::KeyPoint> keypointsO;
	vector<cv::KeyPoint> keypointsS;
	cout << "Looking for object...\n";
	//Start the timer
	double duration;
	duration = static_cast<double>(cv::getTickCount());



	SIFT sift(50,1);
	Mat detector;
	sift(objectP, Mat(), keypointsO, detector);
	sift(sceneP, Mat(), keypointsS, detector);

	//-- Step 2: Calculate descriptors (feature vectors)
	SiftDescriptorExtractor extractor;
  Mat descriptors_object, descriptors_scene;

  extractor.compute( objectP, keypointsO, descriptors_object );
  extractor.compute( sceneP, keypointsS, descriptors_scene );
  
 

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;  
  std::vector< DMatch > matches;
  matcher.match( descriptors_object, descriptors_scene, matches );

  double max_dist = 0; double min_dist = 150;
  double dist;

  //Quick calculation of min and max distances between keypoints
  for(int i=0; i<descriptors_object.rows; i++)
  {
	dist = matches[i].distance;
	if( dist < min_dist ) min_dist = dist;
	if( dist > max_dist ) max_dist = dist;
  }


  vector< DMatch > good_matches;

  cout << "Matches: " << matches.size() << endl;

  for(int i = 0; i < descriptors_object.rows; i++)
  {
	  if( matches[i].distance < 300) 
		  good_matches.push_back( matches[i] );
  }


	duration = static_cast<double>(cv::getTickCount())-duration;
	duration = (duration/cv::getTickFrequency()) * 1000;
	
	
	 
	cout << "Good matches found: " << good_matches.size() << endl;
	cout << "Algorithm duration: " << duration << endl << "--------------------------------------" << endl;


	// drawing the results
	/*namedWindow("matches");
	Mat img_matches;
	drawMatches(objectP, keypointsO, sceneP, keypointsS, good_matches, img_matches);
	imwrite("show.jpg", img_matches);
	imshow("matches", img_matches);
	waitKey(100);*/
	return good_matches.size();
}
