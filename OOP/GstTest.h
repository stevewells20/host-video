#ifndef GstTest_H // avoid repeated expansion
#define GstTest_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <stdio.h>
#include <string>
#include <vector> // std::vector

#include "GstAbsFilter.h"



using namespace cv;
// using namespace cv::xfeatures2d;
using namespace std;

class GstTest : public GstAbsFilter {

public:
	Mat cdst, cdstP, im_with_keypoints;
	SimpleBlobDetector detector;
	vector<KeyPoint> keypoints;
	Ptr<BRISK> ptrBrisk;
  bool on = true;
  enum trackType { THRESH, LAST };
	static bool flipBrisk;

  GstTest()
		: ptrBrisk(BRISK::create(80))
			// flipBrisk(false)
		{
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstTest created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int value,
    //								int max_capacity
    //}

		trackbars[THRESH] = {"Threshold", 30, 100, &onChange};
    //////
  }


  ~GstTest() { cout << "GstTest removed!" << endl; }

	static void onChange(int i, void* ptr) {
		// cout << "onChange: " << i << "\n";
		GstTest *that = (GstTest*)ptr;
		that->flipBrisk=true;
 }


  void filter(const Mat &src, const Mat &src_gray, Mat &dst) {
    if (on) {
			if (flipBrisk) {
				cout << "fliter : flipBrisk : " << flipBrisk << endl;
				ptrBrisk = BRISK::create(trackbars[THRESH].val);
				flipBrisk = false;
			}
			// ptrBrisk = BRISK::create();

      /////////////////////////ImageManipulation/////////////////////////
			// Detect blobs.
			ptrBrisk->detect(src_gray, keypoints);
			// detector.detect( src_gray, keypoints);

			// Draw detected blobs as red circles.
			// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
			drawKeypoints( src_gray, keypoints, im_with_keypoints,
				Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

			// Show blobs
			im_with_keypoints.copyTo(dst);

      /////////////////////////////////////////////////////////////////////
    }
  }
};
bool GstTest::flipBrisk = false;

#endif
