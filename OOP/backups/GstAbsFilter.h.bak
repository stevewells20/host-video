#ifndef GSTABSFILTER_H					// avoid repeated expansion
#define GSTABSFILTER_H

#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>

using namespace cv;
using namespace std;

struct TrackbarStruct {
	string name;
	int min;
	int max;
	int cap;
};

class GstAbsFilter {
public:
	TrackbarStruct trackbars[2];
	bool on = true;

	enum trackType {RADIUS, DISPERSION};

	GstAbsFilter()
	 {
		cout << "GstAbsFilter created!" << endl;
		///////
		// trackbars[x] = {string "Name_of_variable",
		//								int min_slider,
		//								int max_slider,
		//								int max_capacity }
		trackbars[0] = {"Radius", 0, 20, 200};
		trackbars[1] = {"Dispersion", 0, 20, 200};
		//////
	}

	~GstAbsFilter() { cout << "GstAbsFilter removed!" << endl; }


void filter(Mat &src, Mat &src_gray, Mat &dst) {
	if (on) {
///////////////////////////////Image manipulation//////////////////////////////
    medianBlur(src_gray, src_gray, 5);
    vector<Vec3f> circles;
    HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1,
                 src_gray.rows / 20,
                 100, 30, trackbars[RADIUS].min, trackbars[RADIUS].max
                 );
    for (size_t i = 0; i < circles.size(); i++) {
      Vec3i c = circles[i];
      Point center = Point(c[0], c[1]);
      // circle center
      circle(dst, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
      // circle outline
      int radius = c[2];
      circle(dst, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
    }
  }
	////////////////////////////////////////////////////////////////////////////
}



};

#endif
