#ifndef GSTABSFILTER_H // avoid repeated expansion
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
  void (*onChange)(int, void *);
};

class GstAbsFilter {
public:
  int numTrackbars;
  TrackbarStruct *trackbars;
  bool on = true;

  GstAbsFilter() { cout << "GstAbsFilter created!" << endl; }

  virtual ~GstAbsFilter() {
    delete[] trackbars;
    cout << "GstAbsFilter removed!" << endl;
  }

  virtual void filter(const Mat &src, const Mat &src_gray, Mat &dst) = 0;
};

#endif
