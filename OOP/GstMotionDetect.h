#ifndef GstMotionDetect_H // avoid repeated expansion
#define GstMotionDetect_H

#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>

#include "GstAbsFilter.h"

using namespace cv;
using namespace std;

class GstMotionDetect : public GstAbsFilter {
private:
	Mat fgMaskMOG2;
  Ptr<BackgroundSubtractorMOG2> pMOG2; // MOG2 background subtractor

public:
  bool on = true;
	int frameCount = 0;
	bool isFirstFrame = true;
  double maskSum;

  enum trackType { HISTORY, THRESHOLD, SHADOWS, BACK_RATIO, COMPLEXITY_REDUCE, MOVE_THRESH, LAST };

  GstMotionDetect()
	 {
    pMOG2 = createBackgroundSubtractorMOG2(); //MOG2 approach

    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstMotionDetect created!" << endl;
    ///////
    trackbars[HISTORY] = {"HISTORY", 110, 999};
    trackbars[THRESHOLD] = {"THRESHOLD", 32, 64};
    trackbars[SHADOWS] = {"SHADOWS", 0, 1};
    trackbars[BACK_RATIO] = {"BACK_RATIO", 127, 255};
		trackbars[COMPLEXITY_REDUCE] = {"COMPLEXITY_REDUCE", 55, 255};
		trackbars[MOVE_THRESH] = {"MOVE_THRESH", 5, 100};
    //////
  }


  ~GstMotionDetect() { cout << "GstMotionDetect removed!" << endl; }

  static void onChange(int i, void *ptr) { ; }


	void filter(const Mat &src, const Mat &src_gray, Mat &dst, Rect2d &ROI) {
    if (on) {
      ///////////////////////ImageManipulation//////////////////////////
      if (isFirstFrame) {
				Mat mask = Mat( dst.size(), CV_64FC3, Scalar(255,255,255) );
				maskSum = sum(mask)[0];
				isFirstFrame = false;
			}

      if (pMOG2->getHistory() != trackbars[HISTORY].val)
        pMOG2->setHistory(trackbars[HISTORY].val);
      if (pMOG2->getVarThreshold() != trackbars[THRESHOLD].val)
        pMOG2->setVarThreshold(trackbars[THRESHOLD].val);
      if (pMOG2->getDetectShadows() != bool(trackbars[SHADOWS].val))
        pMOG2->setDetectShadows(bool(trackbars[SHADOWS].val));
      if (round(pMOG2->getBackgroundRatio()) != trackbars[BACK_RATIO].val)
        pMOG2->setBackgroundRatio(double(trackbars[BACK_RATIO].val/100.0));
      if (round(pMOG2->getComplexityReductionThreshold()) != trackbars[COMPLEXITY_REDUCE].val)
        pMOG2->setComplexityReductionThreshold(double(trackbars[COMPLEXITY_REDUCE].val/100.0));

      pMOG2->apply(src, fgMaskMOG2);
      fgMaskMOG2.copyTo(dst);

      double dstSum = sum(dst)[0];
			double scale = dstSum/maskSum;
      stringstream stream;
			stream << "Percentage: " << scale << endl;
			putText(dst, stream.str(),  cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 1.8, cvScalar(200,200,250), 1, CV_AA);
			if (scale >= trackbars[MOVE_THRESH].val/100.0 )
				putText(dst, "MOVEMENT DETECTED",  cvPoint(30,60), FONT_HERSHEY_COMPLEX_SMALL, 3.0, cvScalar(255,0,0), 1, CV_AA);


      ///////////////////////////////////////////////////////////////////
    }
  }


};

#endif
