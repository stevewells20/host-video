#ifndef GstLaplaceEdgeDetect_H // avoid repeated expansion
#define GstLaplaceEdgeDetect_H

#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>

#include "GstAbsFilter.h"

using namespace cv;
using namespace std;

class GstLaplaceEdgeDetect : public GstAbsFilter {
private:
  Mat detected_edges;
  int edgeThresh = 1;
  int lowThreshold;
  int const max_lowThreshold = 100;
  int ratio = 3;

public:
  bool on = true;
  bool apply_gaussian = true;
  bool apply_blur = false;
	Mat smoothed;
	Mat laplace;

  enum trackType { SIGMA, LAST };

  GstLaplaceEdgeDetect() {
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstLaplaceEdgeDetect created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int min_slider,
    //								int max_slider,
    //								int max_capacity
    //}
    trackbars[SIGMA] = {"Sigma", 3, 100};
    // trackbars[MAX_THRESH] = {"max Threshold", 40, 100};
    // trackbars[DISPERSION] = {"Dispersion", 20, 200};
    //////
  }

  ~GstLaplaceEdgeDetect() { cout << "GstLaplaceEdgeDetect removed!" << endl; }

  static void onChange(int i, void *v) { ; }

  void filter(const Mat &src, const Mat &src_gray, Mat &dst) {
    if (on) {
      ///////////////////////ImageManipulation//////////////////////////

      int ksize = (trackbars[SIGMA].val*4)|1;
			if(apply_gaussian)
					GaussianBlur(src, smoothed, Size(ksize, ksize), trackbars[SIGMA].val, trackbars[SIGMA].val);
			else if(apply_blur)
					blur(src, smoothed, Size(ksize, ksize));
			else
					medianBlur(src, smoothed, ksize);

			Laplacian(smoothed, laplace, CV_16S, 5);
			convertScaleAbs(laplace, dst, (trackbars[SIGMA].val+1)*0.25);

      ///////////////////////////////////////////////////////////////////
    }
  }


};

#endif
