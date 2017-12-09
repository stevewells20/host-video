#ifndef GstFaceDetect_H // avoid repeated expansion
#define GstFaceDetect_H

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

class GstFaceDetect : public GstAbsFilter {
public:
  bool on = true;

  /// Filter Variables ///
  bool tryflip;
  string cascadeStr = "../haarcascades/haarcascade_frontalface_alt.xml";
  string nestedCascadeStr =
      "../haarcascades/haarcascade_eye_tree_eyeglasses.xml";
  CascadeClassifier cascade;
  CascadeClassifier nestedCascade;
  int scale = 100;
  double detectionTime = 0;
  vector<Rect> faces, faces2;
  Scalar colors[8];
  Mat smallImg;
  Mat smallImgROI;
  //////////////////////////
	Mat temp;

  enum trackType { SCALE };

  GstFaceDetect()
      : colors{Scalar(255, 0, 0), Scalar(255, 128, 0), Scalar(255, 255, 0),
               Scalar(0, 255, 0), Scalar(0, 128, 255), Scalar(0, 255, 255),
               Scalar(0, 0, 255), Scalar(255, 0, 255)} {
    if (!cascade.load(cascadeStr)) {
      cout << "Error loading cascade" << endl;
    }
    if (!nestedCascade.load(nestedCascadeStr)) {
      cout << "Error loading nestedCascade" << endl;
    }

    GstAbsFilter::numTrackbars = 1;
    GstAbsFilter::trackbars = new TrackbarStruct[GstAbsFilter::numTrackbars];

    cout << "GstFaceDetect created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int min_slider,
    //								int max_slider,
    //								int max_capacity
    //}
    trackbars[SCALE] = {"Scale", 100, 200};
    //////
  }

  ~GstFaceDetect() { cout << "GstFaceDetect removed!" << endl; }

  static void onChange(int i, void *v) { ; }

void filter(const Mat &src, const Mat &src_gray, Mat &dst) {
	if (on) {
			////////////////////////////Image manipulation//////////////////////////


			////////////////////////////////////////////////////////////////////////
  }

  double getScale() { return trackbars[SCALE].val / 100.0; }
  /// End class ///
};

#endif
