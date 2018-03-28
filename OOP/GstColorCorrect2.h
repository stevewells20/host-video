#ifndef GstColorCorrect2_H // avoid repeated expansion
#define GstColorCorrect2_H

#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>
#include <vector> // std::vector

#include "GstAbsFilter.h"

using namespace cv;
using namespace std;

class GstColorCorrect2 : public GstAbsFilter {
private:
  Mat temp;

public:
  bool on = true;
	double clip_max=40;
	Size grid_size;

  enum trackType { CLIP_LIMIT, LAST };

  GstColorCorrect2() {
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstColorCorrect2 created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int min_slider,
    //								int max_slider,
    //								int max_capacity
    //}
    trackbars[CLIP_LIMIT] = {"Clip Limit", 4, clip_max};
    //////
		grid_size = Size(2,2);
  }

  ~GstColorCorrect2() { cout << "GstColorCorrect2 removed!" << endl; }

	static void onChange(int i, void* ptr) {
		cout << "onChange: " << i << "\n";
		// GstEdgeDetect *that = (GstEdgeDetect*)ptr;
 }

  void filter(const Mat &src, const Mat &src_gray, Mat &dst, Rect2d &ROI) {
    if (on) {
      /////////////////////////ImageManipulation/////////////////////////
      // cout << "GstColorCorrect2.filter()" << endl;
      vector<Mat> channels;
      split (src, channels);
      for (int i = 0; i < src.channels(); i++)
        equalizeHist( channels[i], channels[i] );
      merge(channels, dst);

      /////////////////////////////////////////////////////////////////////
    }
  }
};

#endif
