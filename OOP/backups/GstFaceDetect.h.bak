#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>

#include "GstStreamCapper.h"

using namespace cv;
using namespace std;

class GstFaceDetect {

public:
  /// Filter Variables ///
  bool tryflip;
  string cascadeStr = "haarcascade/haarcascade_frontalface_alt.xml";
  // CascadeClassifier cascade(cascadeStr);
  string nestedCascadeStr = "haarcascades/haarcascade_eye_tree_eyeglasses.xml";
  // CascadeClassifier nestedCascade(nestedCascadeStr);
  double scale = 1.0;
  double t = 0;
  vector<Rect> faces, faces2;
  Scalar colors[8];
  //////////////////////////

  string windowName = "newFaceFilter";
  GstStreamCapper *capper;
  bool gogo = true;

  GstFaceDetect()
      : colors{Scalar(255, 0, 0),   Scalar(255, 128, 0), Scalar(255, 255, 0), Scalar(0, 255, 0),
               Scalar(0, 128, 255), Scalar(0, 255, 255), Scalar(0, 0, 255),   Scalar(255, 0, 255)}

  {
    cout << "-GstFaceDetect created!" << endl;
  }

  // GstFaceDetect(GstStreamCapper *newCapper)
  // : cascade(cascadeStr), nestedCascade(nestedCascadeStr)
  // colors{(255, 0, 0), (255, 128, 0), (255, 255, 0),
  // 			 (0, 255, 0), (0, 128, 255), (0, 255, 255),
  // 			 (0, 0, 255), (255, 0, 255)}

  // {
    // cout << "-GstFaceDetect created!" << endl;
    // capper = newCapper;
  // }

  void filterRun() {
    int minRad = 2;
    // createButton("End", onChange, this, QT_RADIOBOX);
    namedWindow(windowName, CV_WINDOW_AUTOSIZE);
    // createTrackbar("Min Radius:", windowName, &minRad, 200, onChange, this);
    /// Show the image
    DetectFaces();
  }

  static void onChange(int, void *ptr) {
    GstFaceDetect *that = (GstFaceDetect *)ptr;
    that->DetectFaces();
  }

  void DetectFaces() {
    Mat smallImg;
    CascadeClassifier cascade(cascadeStr);
    CascadeClassifier nestedCascade(nestedCascadeStr);
    while (gogo) {
      capper->cap >> capper->src;
      if (capper->src.empty())
        return;
      capper->src.copyTo(capper->dst);

      // Convert the image to grayscale
      cvtColor(capper->src, capper->src_gray, CV_BGR2GRAY);

      /////////////////////////Image_manipulation///////////////////////////

      double fx = 1 / scale;
      resize(capper->src_gray, smallImg, Size(), fx, fx, INTER_LINEAR);
      equalizeHist(smallImg, smallImg);
      t = (double)getTickCount();
      cascade.detectMultiScale(smallImg, faces, 1.1, 2,
                               0
                                   //|CASCADE_FIND_BIGGEST_OBJECT
                                   //|CASCADE_DO_ROUGH_SEARCH
                                   | CASCADE_SCALE_IMAGE,
                               Size(30, 30));
      if (tryflip) {
        flip(smallImg, smallImg, 1);
        cascade.detectMultiScale(smallImg, faces2, 1.1, 2,
                                 0
                                     //|CASCADE_FIND_BIGGEST_OBJECT
                                     //|CASCADE_DO_ROUGH_SEARCH
                                     | CASCADE_SCALE_IMAGE,
                                 Size(30, 30));
        for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end();
             ++r) {
          faces.push_back(
              Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
        }
      }
      t = (double)getTickCount() - t;
      printf("detection time = %g ms\n", t * 1000 / getTickFrequency());
      for (size_t i = 0; i < faces.size(); i++) {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = (colors[i % 8]);
        int radius;
        double aspect_ratio = (double)r.width / r.height;
        if (0.75 < aspect_ratio && aspect_ratio < 1.3) {
          center.x = cvRound((r.x + r.width * 0.5) * scale);
          center.y = cvRound((r.y + r.height * 0.5) * scale);
          radius = cvRound((r.width + r.height) * 0.25 * scale);
          circle(capper->dst, center, radius, color, 3, 8, 0);
        } else {
          rectangle(capper->dst,
                    cvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
                    cvPoint(cvRound((r.x + r.width - 1) * scale),
                            cvRound((r.y + r.height - 1) * scale)),
                    color, 3, 8, 0);
        }
        if (nestedCascade.empty())
          continue;
        smallImgROI = smallImg(r);
        nestedCascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2,
                                       0
                                           //|CASCADE_FIND_BIGGEST_OBJECT
                                           //|CASCADE_DO_ROUGH_SEARCH
                                           //|CASCADE_DO_CANNY_PRUNING
                                           | CASCADE_SCALE_IMAGE,
                                       Size(30, 30));
        for (size_t j = 0; j < nestedObjects.size(); j++) {
          Rect nr = nestedObjects[j];
          center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
          center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
          radius = cvRound((nr.width + nr.height) * 0.25 * scale);
          circle(capper->dst, center, radius, color, 3, 8, 0);
        }
      }

      //////////////////////////////////////////////////////////////////////////

      imshow(windowName, capper->dst);
      int key;
      key = waitKey(30);
    }
  }
};
