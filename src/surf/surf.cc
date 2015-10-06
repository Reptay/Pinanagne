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



	/*SiftFeatureDetector surf;
	surf.detect(objectP,keypointsO);
	surf.detect(sceneP,keypointsS);
*/
	SIFT sift(10,1);
	Mat detector;
	sift(objectP, Mat(), keypointsO, detector);
	sift(sceneP, Mat(), keypointsS, detector);

	//-- Step 2: Calculate descriptors (feature vectors)
  //SurfDescriptorExtractor extractor;
	SiftDescriptorExtractor extractor;
  Mat descriptors_object, descriptors_scene;

  extractor.compute( objectP, keypointsO, descriptors_object );
  extractor.compute( sceneP, keypointsS, descriptors_scene );
  
 

  //-- Step 3: Matching descriptor vectors using FLANN matcher
  FlannBasedMatcher matcher;  
  //BFMatcher matcher(NORM_L2);
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

 /* cout << "-- Max Dist: " << max_dist << endl;
  cout << "-- Min Dist: " << min_dist << endl;*/

  vector< DMatch > good_matches;

  cout << "Matches: " << matches.size() << endl;

  for(int i = 0; i < descriptors_object.rows; i++)
  {
	  if( matches[i].distance < 300) 
		  good_matches.push_back( matches[i] );
  }

	//drawMatches(objectP,keypointsO,sceneP,keypointsS,matches,outImg,Scalar::all(-1), Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	/*drawKeypoints(objectP,keypointsO,objectP,Scalar(0,0,255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	namedWindow("SURF");
	imshow("SURF",objectP);*/

	//-- Localize the object

 /* if( good_matches.size() >=8 && good_matches.size() <= 30)
  {
	cout << "OBJECT FOUND!" << endl;
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for( unsigned int i = 0; i < good_matches.size(); i++ )
	{
		//-- Get the keypoints from the good matches
		obj.push_back( keypointsO[ good_matches[i].queryIdx ].pt );
	    scene.push_back( keypointsS[ good_matches[i].trainIdx ].pt );
	}

	Mat H = findHomography( obj, scene, CV_RANSAC );

  

	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( objectP.cols, 0 );
	obj_corners[2] = cvPoint( objectP.cols, objectP.rows ); obj_corners[3] = cvPoint( 0, objectP.rows );
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform( obj_corners, scene_corners, H);

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
  
	line( outImg, scene_corners[0] , scene_corners[1], Scalar(255,255,0), 2 ); //TOP line
	line( outImg, scene_corners[1] , scene_corners[2], color, 2 );
	line( outImg, scene_corners[2] , scene_corners[3], color, 2 );
	line( outImg, scene_corners[3] , scene_corners[0] , color, 2 );
  }
  else cout << "OBJECT NOT FOUND!" << endl;
*/	
	duration = static_cast<double>(cv::getTickCount())-duration;
	duration = (duration/cv::getTickFrequency()) * 1000;
	
	
	 
	//cout <<  << endl;
	cout << "Good matches found: " << good_matches.size() << endl;
	cout << "Algorithm duration: " << duration << endl << "--------------------------------------" << endl;


	// drawing the results
	namedWindow("matches");
	Mat img_matches;
	drawMatches(objectP, keypointsO, sceneP, keypointsS, good_matches, img_matches);
	imwrite("show.jpg", img_matches);
	imshow("matches", img_matches);
	waitKey(100);
	return good_matches.size();
}
