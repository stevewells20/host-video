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
#include <vector>
#include <opencv2/tracking.hpp>
// #include <opencv2/xfeatures2d.hpp>

#include "GstAbsFilter.h"



using namespace cv;
// using namespace cv::tracking;
using namespace std;

class GstTest : public GstAbsFilter {

public:
  bool on = true;
  enum trackType { THRESH, OCTAVES, LAST };
	static bool callBoolck;

	Mat descripterA, descripterB, src_gray_ROI;
////
	MultiTracker tracker;
////

  GstTest()

		{
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstTest created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int value,
    //								int max_capacity
    //}

		trackbars[THRESH] = {"Threshold", 100, 200, &onChange};
		trackbars[OCTAVES] = {"Octaves", 3, 9, &onChange};
    //////
  }


  ~GstTest() { cout << "GstTest removed!" << endl; }

	static void onChange(int i, void* ptr) {
		// cout << "onChange: " << i << "\n";
		GstTest *that = (GstTest*)ptr;
		that->callBoolck=true;
 }

 // static void selectBox()


  void filter(const Mat &src, const Mat &src_gray, Mat &dst, Rect2d &ROI) {
    if (on) {

			/////////////////////////ImageManipulation/////////////////////////

			if (callBoolck) {
				cout << "fliter : callBoolck : " << callBoolck << endl;
				// selection = Rect2d(ROI);
				callBoolck = false;
			}
			// cout <<  ROI << endl;

			src_gray_ROI  = src_gray(ROI);



			rectangle(dst, ROI, Scalar(0,0,255));

      /////////////////////////////////////////////////////////////////////

    }
  }
};
bool GstTest::callBoolck = false;

#endif
