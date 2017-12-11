#ifndef GstEdgeDetect_H					// avoid repeated expansion
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


class GstEdgeDetect : public GstAbsFilter {
private:
  Mat detected_edges;
  int edgeThresh = 1;
  int lowThreshold;
  int const max_lowThreshold = 100;
  int ratio = 3;
  int kernel_size = 3;

public:

	bool on = true;

	enum trackType {MIN_THRESH, MAX_THRESH, LAST};

	GstEdgeDetect()
	 {
		GstAbsFilter::numTrackbars=LAST;
		GstAbsFilter::trackbars=new TrackbarStruct[LAST];

		cout << "GstEdgeDetect created!" << endl;
		///////
		// trackbars[x] = {string "Name_of_variable",
		//								int min_slider,
		//								int max_slider,
		//								int max_capacity }
		trackbars[MIN_THRESH] = {"min Threshold", 15, 100};
		trackbars[MAX_THRESH] = {"max Threshold", 40, 100};
		// trackbars[DISPERSION] = {"Dispersion", 20, 200};
		//////
	}

	~GstEdgeDetect() {
		cout << "GstEdgeDetect removed!" << endl;
	}

	static void onChange(int i, void* v) { ; }


void filter(const Mat &src, const Mat &src_gray, Mat &dst) {
	if (on) {
///////////////////////////////Image manipulation//////////////////////////////
		// cout << "GstEdgeDetect.filter()" << endl;
    // Reduce noise with a kernel 3x3
    blur(src_gray, detected_edges, Size(kernel_size, kernel_size));
    // Canny detector
    Canny(detected_edges, dst,
          trackbars[MIN_THRESH].val,
          trackbars[MIN_THRESH].val * ratio,
          kernel_size);
    // Using Canny's output as a mask, we display our result
    // dst = Scalar::all(0);
	////////////////////////////////////////////////////////////////////////////
  }
}



};

#endif
