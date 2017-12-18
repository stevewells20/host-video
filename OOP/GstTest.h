#ifndef GstTest_H // avoid repeated expansion
#define GstTest_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <stdio.h>
#include <string>
#include <vector> // std::vector

#include "GstAbsFilter.h"



using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

class GstTest : public GstAbsFilter {

public:
  bool on = true;

	enum trackType { THRESH, LAST };

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
    //////
  }


  ~GstTest() { cout << "GstTest removed!" << endl; }

	static void onChange(int i, void* ptr) {
		// cout << "onChange: " << i << "\n";
		GstTest *that = (GstTest*)ptr;
		that->flipBrisk=true;
 }

 // static void selectBox()


  void filter(const Mat &src, const Mat &src_gray, Mat &dst, Rect2d &ROI) {
    if (on) {

			/////////////////////////ImageManipulation/////////////////////////

			if (flipBrisk) {
				cout << "fliter : flipBrisk : " << flipBrisk << endl;
				ptrBrisk = BRISK::create(trackbars[THRESH].val, trackbars[OCTAVES].val);
				flipBrisk = false;
			}
			// ptrBrisk = BRISK::create();

			// Detect blobs.
			cout <<  ROI << endl;
			// if(src_gray.rows==0 || src_gray.cols==0) return;

			src_gray_ROI  = src_gray(ROI);

			ptrBrisk->detect(src_gray_ROI, keypointsA);
			// ptrBrisk->compute(src_gray_ROI, keypointsA, descripterA);
			// if (keypointsB.size() > 0) matcher.match(descripterA, descripterB, matches);
			// Add padding for uncropped translation
			for (int i = 0; i < keypointsA.size(); i++) {
				keypointsA[i].pt.x += ROI.x ;
				keypointsA[i].pt.y += ROI.y ;
			}
			// detector.detect( src_gray, keypointsA);

			// Draw detected blobs as red circles.
			// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob

			drawKeypoints( src, keypointsA, dst,
				Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );


			// if (keypointsB.size() > 0) drawKeypoints( dst, keypointsB, dst,
			// 	Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
				//DrawMatchesFlags:: {DRAW_RICH_KEYPOINTS,NOT_DRAW_SINGLE_POINTS,DRAW_OVER_OUTIMG,DEFAULT}
			//  &&  && matches.size() >  0)

				// cv::drawMatches( src_gray_ROI, keypointsA, src_gray_ROI, keypointsB,
			  //                    matches, dst, cv::Scalar::all(-1), cv::Scalar::all(-1),
			  //                    vector<char>(),cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

			rectangle(dst, ROI, Scalar(0,0,255));
			// src_gray_ROI.copyTo(dst);
			// cout << "DONE\n";
      /////////////////////////////////////////////////////////////////////

    }
  }
};
bool GstTest::flipBrisk = false;

#endif
