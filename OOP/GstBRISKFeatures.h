#ifndef GstBRISKFeatures_H // avoid repeated expansion
#define GstBRISKFeatures_H

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

class GstBRISKFeatures : public GstAbsFilter {

public:
	Mat cdst, cdstP, im_with_keypoints;
	SimpleBlobDetector detector;
	vector<KeyPoint> keypoints;
	Ptr<BRISK> ptrBrisk;
  bool on = true;
  enum trackType { THRESH, OCTAVES, LAST };
	static bool changeThresh;

  GstBRISKFeatures()
		: ptrBrisk(BRISK::create(80))
		{
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstBRISKFeatures created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int value,
    //								int max_capacity
    //}

		trackbars[THRESH] = {"Threshold", 100, 255, &onChange};
		trackbars[OCTAVES] = {"Octaves", 3, 9, &onChange};
    //////
  }


  ~GstBRISKFeatures() { cout << "GstBRISKFeatures removed!" << endl; }

	static void onChange(int i, void* ptr) {
		// cout << "onChange: " << i << "\n";
		GstBRISKFeatures *that = (GstBRISKFeatures*)ptr;
		that->changeThresh=true;
 }


  void filter(const Mat &src, const Mat &src_gray, Mat &dst) {
    if (on) {
			/* Add static bools (changeThresh) to
			signal changes on multithreaded processes (BRISK::create()) */
			if (changeThresh) {
				cout << "changeThresh : " << trackbars[THRESH].val << endl;
				ptrBrisk = BRISK::create(trackbars[THRESH].val, trackbars[OCTAVES].val);
				changeThresh = false;
			}

      /////////////////////////ImageManipulation/////////////////////////
			// Detect blobs.
			ptrBrisk->detect(src_gray, keypoints);
			// detector.detect( src_gray, keypoints);

			// Draw detected blobs as red circles.
			// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
			drawKeypoints( src, keypoints, dst,
				Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

			// Show blobs
			// im_with_keypoints.copyTo(dst);

      /////////////////////////////////////////////////////////////////////
    }
  }
};
bool GstBRISKFeatures::changeThresh = false;

#endif
