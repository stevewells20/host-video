#ifndef GstColorCorrect_H // avoid repeated expansion
#define GstColorCorrect_H

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

class GstColorCorrect : public GstAbsFilter {
private:
  Mat temp;

public:
  bool on = true;
	double clip_max=40;
	Size grid_size;

  enum trackType { CLIP_LIMIT, LAST };

  GstColorCorrect() {
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstColorCorrect created!" << endl;
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

  ~GstColorCorrect() { cout << "GstColorCorrect removed!" << endl; }

	static void onChange(int i, void* ptr) {
		cout << "onChange: " << i << "\n";
		// GstEdgeDetect *that = (GstEdgeDetect*)ptr;
 }
 
  void filter(const Mat &src, const Mat &src_gray, Mat &dst) {
    if (on) {
      /////////////////////////ImageManipulation/////////////////////////
      // cout << "GstColorCorrect.filter()" << endl;

      // READ RGB color image and convert it to Lab
			cvtColor(src, temp, CV_BGR2Lab);

      // Extract the L channel
      vector<Mat> lab_planes(3);
      split(temp, lab_planes); // now we have the L image in lab_planes[0]

      // apply the CLAHE algorithm to the L channel
      Ptr<CLAHE> clahe = createCLAHE(clip_max, grid_size);
      clahe->setClipLimit(trackbars[CLIP_LIMIT].val);
      clahe->apply(lab_planes[0], dst);

      // Merge the the color planes back into an Lab image
      dst.copyTo(lab_planes[0]);

      merge(lab_planes, temp);

      // convert back to RGB
      // cv::Mat image_clahe;
      cvtColor(temp, dst, CV_Lab2BGR);

      // display the results  (you might also want to see lab_planes[0] before
      // and after).
      //  cv::imshow("image original", src);
      //  cv::imshow("image CLAHE", image_clahe);
      //  cv::waitKey();
      /////////////////////////////////////////////////////////////////////
    }
  }
};

#endif
