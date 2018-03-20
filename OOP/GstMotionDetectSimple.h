#ifndef GstMotionDetectSimple_H // avoid repeated expansion
#define GstMotionDetectSimple_H

#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

#include "GstAbsFilter.h"

using namespace cv;
using namespace std;

class GstMotionDetectSimple : public GstAbsFilter {
private:
  Mat detected_edges;
  int edgeThresh = 1;
  int lowThreshold;
  int const max_lowThreshold = 100;
  int ratio = 3;
	Mat mask;
	Scalar maskSum;

public:
  bool on = true;
  bool apply_gaussian = true;
  bool apply_blur = false;
	Mat smoothed;
	Mat laplace;
	Mat t1, t2;
	int frameCount = 0;
	bool isFirstFrame = true;
	bool maskUpdatePending = false;

  enum trackType { SENSITIVITY, LAST };

  GstMotionDetectSimple()
	 {
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstMotionDetectSimple created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int min_slider,
    //								int max_slider,
    //								int max_capacity
    //}
    trackbars[SENSITIVITY] = {"Sensitivity", 128, 255, &onChange};
    // trackbars[MAX_THRESH] = {"max Threshold", 40, 100};
    // trackbars[DISPERSION] = {"Dispersion", 20, 200};
    //////
  }

  ~GstMotionDetectSimple() { cout << "GstMotionDetectSimple removed!" << endl; }

  static void onChange(int i, void *ptr) { ;
		// GstMotionDetectSimple *that = (GstMotionDetectSimple*)ptr;
		// that->maskUpdatePending = true;
		// cout << "HERE!!" << endl;
	}
 //  void updateMask() {
 //  mask = Mat( mask.size(), CV_64FC3, Scalar(trackbars[SENSITIVITY].val,trackbars[SENSITIVITY].val,trackbars[SENSITIVITY].val) );
 //  maskSum = sum(sum(mask));
 // }

	void diffImg(const Mat t0, const Mat t1, const Mat t2, Mat dst) {
		Mat d1, d2;
		absdiff(t2, t1, d1);
		absdiff(t1, t0, d2);
		bitwise_or(d1, d2, dst);
	}

	void filter(const Mat &src, const Mat &src_gray, Mat &dst, Rect2d &ROI) {
    if (on) {
      ///////////////////////ImageManipulation//////////////////////////

			if (frameCount < 2) {
				t1.copyTo(t2);
				src.copyTo(t1);
				frameCount++;
				mask = Mat( dst.size(), CV_64FC3, Scalar(trackbars[SENSITIVITY].val,trackbars[SENSITIVITY].val,trackbars[SENSITIVITY].val) );
				maskSum = sum(sum(mask));
				return;
			}
			// if (maskUpdatePending) {
			// 	updateMask();
			// 	maskUpdatePending = false;
			// }

			diffImg( src, t1, t2, dst );

			Scalar dstSum = sum(sum(dst));
			//cout << "Image: " << dstSum << endl;

			double percent;
			//for (int i = 0; i < 3; i++) {
			percent =  (dstSum[0] / maskSum[0]) * 100;
			//}
			cout << "Percentage: " << percent << endl;
			stringstream stream;
			stream << "Percentage: " << float(percent) << endl;
			putText(dst, stream.str(),  cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 1.8, cvScalar(200,200,250), 1, CV_AA);

			if (percent > 3.0) {
				putText(dst, "!!! MOVEMENT DETECTED !!!",  cvPoint(100,100), FONT_HERSHEY_COMPLEX_SMALL, 3.4, cvScalar(0,0,255), 1, CV_AA);
			}

			t1.copyTo(t2);
			src.copyTo(t1);

			// if (isFirstFrame) {
			// 	src.copyTo(last);
			// 	src.copyTo(dst);
			// 	isFirstFrame = false;
			// 	return;
			// }
			// subtract(src, last, dst);
			// src.copyTo(last);

			/*
      int ksize = (trackbars[SIGMA].val*4)|1;
			if(apply_gaussian)
					GaussianBlur(src, smoothed, Size(ksize, ksize), trackbars[SIGMA].val, trackbars[SIGMA].val);
			else if(apply_blur)
					blur(src, smoothed, Size(ksize, ksize));
			else
					medianBlur(src, smoothed, ksize);

			Laplacian(smoothed, laplace, CV_16S, 5);
			convertScaleAbs(laplace, dst, (trackbars[SIGMA].val+1)*0.25);
*/
      ///////////////////////////////////////////////////////////////////
    }
  }


};

#endif
