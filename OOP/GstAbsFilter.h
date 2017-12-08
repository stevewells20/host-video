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
	int val;
	int cap;
};

class GstAbsFilter {
public:
	const static int numTrackbars = 3 ;
	TrackbarStruct trackbars[numTrackbars];
	bool on = true;

	enum trackType {MIN_RADIUS, MAX_RADIUS, DISPERSION};

	GstAbsFilter()
	 {
		cout << "GstAbsFilter created!" << endl;
		///////
		// trackbars[x] = {string "Name_of_variable",
		//								int min_slider,
		//								int max_slider,
		//								int max_capacity }
		trackbars[MIN_RADIUS] = {"min Radius", 15, 200};
		trackbars[MAX_RADIUS] = {"max Radius", 40, 200};
		trackbars[DISPERSION] = {"Dispersion", 20, 200};
		//////
	}

	~GstAbsFilter() { cout << "GstAbsFilter removed!" << endl; }

	static void onChange(int i, void* v) {
		cout << i << endl;}


void filter(Mat &src, Mat &src_gray, Mat &dst) {
	if (on) {
///////////////////////////////Image manipulation//////////////////////////////
    medianBlur(src_gray, src_gray, 5);
    vector<Vec3f> circles;
    HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1,
                 src_gray.rows / trackbars[DISPERSION].val,
                 100, 30, trackbars[MIN_RADIUS].val, trackbars[MAX_RADIUS].val
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
