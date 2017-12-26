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

  enum trackType { SCALE, LAST};

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

    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

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

	void filter(const Mat &src, const Mat &src_gray, Mat &dst, Rect2d &ROI) {
	if (on) {
      ////////////////////////////Image manipulation//////////////////////////
			// cout << "GstFaceDetect.filter()" << endl;
      double fx = 1 / getScale();
      resize(src_gray, smallImg, Size(), fx, fx, INTER_LINEAR);
      equalizeHist(smallImg, smallImg);
      detectionTime = (double)getTickCount();
      cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0
                               //|CASCADE_FIND_BIGGEST_OBJECT
                               //|CASCADE_DO_ROUGH_SEARCH
                               | CASCADE_SCALE_IMAGE,
                               Size(30, 30));
      if (tryflip) {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale(smallImg, faces2, 1.1, 2, 0
                                 //|CASCADE_FIND_BIGGEST_OBJECT
                                 //|CASCADE_DO_ROUGH_SEARCH
                                 | CASCADE_SCALE_IMAGE,
                                 Size(30, 30));
        for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r) {
          faces.push_back(
              Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
      }
      detectionTime = (double)getTickCount() - detectionTime;
      // printf("detection time = %g ms\n",detectionTime* 1000 / getTickFrequency());
      for (size_t i = 0; i < faces.size(); i++) {
        Rect r = faces[i];
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = (colors[i % 8]);
        int radius;
        double aspect_ratio = (double)r.width / r.height;
        if (0.75 < aspect_ratio && aspect_ratio < 1.3) {
          center.x = cvRound((r.x + r.width * 0.5) * getScale());
          center.y = cvRound((r.y + r.height * 0.5) * getScale());
          radius = cvRound((r.width + r.height) * 0.25 * getScale());
          circle(dst, center, radius, color, 3, 8, 0);
        } else {
          rectangle(dst, cvPoint(cvRound(r.x * getScale()),
                    cvRound(r.y * getScale())),
                    cvPoint(cvRound((r.x + r.width - 1) * getScale()),
                    cvRound((r.y + r.height - 1) * getScale())),
                    color, 3, 8, 0);
        }
        if (nestedCascade.empty())
          continue;
        smallImgROI = smallImg(r);
        nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2, 0
                                       //|CASCADE_FIND_BIGGEST_OBJECT
                                       //|CASCADE_DO_ROUGH_SEARCH
                                       //|CASCADE_DO_CANNY_PRUNING
                                       | CASCADE_SCALE_IMAGE,
                                       Size(30, 30));
        for (size_t j = 0; j < nestedObjects.size(); j++) {
          Rect nr = nestedObjects[j];
          center.x = cvRound((r.x + nr.x + nr.width * 0.5) * getScale());
          center.y = cvRound((r.y + nr.y + nr.height * 0.5) * getScale());
          radius = cvRound((nr.width + nr.height) * 0.25 * getScale());
          circle(dst, center, radius, color, 3, 8, 0);
        }
      }
    }
    ////////////////////////////////////////////////////////////////////////////
  }

  double getScale() { return trackbars[SCALE].val / 100.0; }
  /// End class ///
};

#endif
