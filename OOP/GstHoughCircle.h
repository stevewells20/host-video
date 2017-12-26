#ifndef GstHoughCircle_H					// avoid repeated expansion
#define GstHoughCircle_H

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


class GstHoughCircle : public GstAbsFilter {
private:
	Mat temp;

public:

	bool on = true;

	enum trackType {MIN_RADIUS, MAX_RADIUS, DISPERSION, LAST};

	GstHoughCircle()
	 {
		GstAbsFilter::numTrackbars=LAST;
		GstAbsFilter::trackbars=new TrackbarStruct[LAST];

		cout << "GstHoughCircle created!" << endl;
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

	~GstHoughCircle() {
		cout << "GstHoughCircle removed!" << endl;
	}

	static void onChange(int i, void* v) { ; }


	void filter(const Mat &src, const Mat &src_gray, Mat &dst, Rect2d &ROI) {
	if (on) {
///////////////////////////////Image manipulation//////////////////////////////
		// cout << "GstHoughCircle.filter()" << endl;
    medianBlur(src_gray, temp, 5);
    vector<Vec3f> circles;
    HoughCircles(temp, circles, HOUGH_GRADIENT, 1,
                 temp.rows / trackbars[DISPERSION].val,
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
