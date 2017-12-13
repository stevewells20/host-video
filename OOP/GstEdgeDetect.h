#ifndef GstEdgeDetect_H // avoid repeated expansion
#define GstEdgeDetect_H

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

RNG rng(12345);

class GstEdgeDetect : public GstAbsFilter {
private:
  Mat detected_edges;
  int edgeThresh = 1;
  int lowThreshold;
  int const max_lowThreshold = 100;
  int ratio = 3;
  int kernel_size = 3;
  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;
  vector<vector<Point>> hull;

public:
  bool on = true;
  bool apply_contuors = true;
  bool apply_hull = true;

  enum trackType { MIN_THRESH, MAX_THRESH, LAST };

  GstEdgeDetect() {
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstEdgeDetect created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int min_slider,
    //								int max_slider,
    //								int max_capacity
    //}
    trackbars[MIN_THRESH] = {"min Threshold", 15, 100};
    trackbars[MAX_THRESH] = {"max Threshold", 40, 100};
    // trackbars[DISPERSION] = {"Dispersion", 20, 200};
    //////
  }

  ~GstEdgeDetect() { cout << "GstEdgeDetect removed!" << endl; }

  static void onChange(int i, void *v) { ; }

  void filter(const Mat &src, const Mat &src_gray, Mat &dst) {
    if (on) {
      ///////////////////////ImageManipulation//////////////////////////
      // cout << "GstEdgeDetect.filter()" << endl;
      // Reduce noise with a kernel 3x3
      blur(src_gray, detected_edges, Size(kernel_size, kernel_size));
      dst = Scalar::all(0);
      // Canny detector
      Canny(detected_edges, detected_edges, trackbars[MIN_THRESH].val,
            // trackbars[MIN_THRESH].val * ratio, //swapped in next line
            trackbars[MAX_THRESH].val * ratio, kernel_size);
      src.copyTo(dst, detected_edges);
      // Using Canny's output as a mask, we display our result
      if (apply_contuors) {
        cout << "applied" << endl;
        findContours(detected_edges, contours, hierarchy, RETR_TREE,
                     CHAIN_APPROX_SIMPLE, Point(0, 0));
        // Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        dst = Scalar::all(0);
        Scalar color1 = Scalar(255, 0, 0);
        Scalar color2 = Scalar(0, 255, 0);

        vector<vector<Point>> hull(contours.size());
        for (size_t i = 0; i < contours.size(); i++) {
          convexHull(Mat(contours[i]), hull[i], false);
        }
        // Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
        for (size_t i = 0; i < contours.size(); i++) {
          Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
                                rng.uniform(0, 255));
          drawContours(dst, contours, (int)i, color1, 1, 8, vector<Vec4i>(),
                       0, Point());
          drawContours(dst, hull, (int)i, color2, 1, 8, vector<Vec4i>(), 0,
                       Point());
        }
      }
      ///////////////////////////////////////////////////////////////////
    }
  }

  /// Colored mask edge detection
  /*//////////////////////////ImageManipulation////////////////////////
		// cout << "GstEdgeDetect.filter()" << endl;
    // Reduce noise with a kernel 3x3
    blur(src_gray, detected_edges, Size(kernel_size, kernel_size));
    dst = Scalar::all(0);
    // Canny detector
    Canny(detected_edges, detected_edges,
          trackbars[MIN_THRESH].val,
          // trackbars[MIN_THRESH].val * ratio, //swapped in next line
					trackbars[MAX_THRESH].val * ratio,
          kernel_size);
		src.copyTo(dst, detected_edges);
    // Using Canny's output as a mask, we display our result
	/*/ //////////////////////////////////////////////////////////////

  /// Black and white edge detection
  /*//////////////////////////ImageManipulation/////////////////////////
			// cout << "GstEdgeDetect.filter()" << endl;
	    // Reduce noise with a kernel 3x3
	    blur(src_gray, detected_edges, Size(kernel_size, kernel_size));
	    // dst = Scalar::all(0);
	    // Canny detector
	    Canny(detected_edges, dst,
	          trackbars[MIN_THRESH].val,
	          // trackbars[MIN_THRESH].val * ratio, //swapped in next line
						trackbars[MAX_THRESH].val * ratio,
	          kernel_size);
	    // Using Canny's output as a mask, we display our result
		/*/ //////////////////////////////////////////////////////////////////
};

#endif
