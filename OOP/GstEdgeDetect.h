#ifndef GstEdgeDetect_H // avoid repeated expansion
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

RNG rng(12345);

class GstEdgeDetect : public GstAbsFilter {
private:
  Mat detected_edges;
  int edgeThresh = 1;
  int lowThreshold;
  int const max_lowThreshold = 100;
  int ratio = 3;
  int kernel_size = 3;
  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;
  vector<vector<Point>> hull;

public:
  bool on = true;
  bool apply_contuors = false;
  bool apply_hull = false;
	bool apply_lines = false;
	Mat cdstP;
	Mat cdst;

  enum trackType { MIN_THRESH, MAX_THRESH, LAST };

  GstEdgeDetect() {
    GstAbsFilter::numTrackbars = LAST;
    GstAbsFilter::trackbars = new TrackbarStruct[LAST];

    cout << "GstEdgeDetect created!" << endl;
    ///////
    // trackbars[x] = {string "Name_of_variable",
    //								int min_slider,
    //								int max_slider,
    //								int max_capacity
    //}
    trackbars[MIN_THRESH] = {"min Threshold", 15, 100, &onChange};
    trackbars[MAX_THRESH] = {"max Threshold", 40, 100, &onChange};
    // trackbars[DISPERSION] = {"Dispersion", 20, 200};
    //////
  }

  ~GstEdgeDetect() { cout << "GstEdgeDetect removed!" << endl; }

	static void onChange(int i, void* ptr) {
		cout << "onChange: " << i << "\n";
		// GstEdgeDetect *that = (GstEdgeDetect*)ptr;
 }

  void filter(const Mat &src, const Mat &src_gray, Mat &dst) {
    if (on) {
      ///////////////////////ImageManipulation//////////////////////////
      // cout << "GstEdgeDetect.filter()" << endl;
      // Reduce noise with a kernel 3x3
      blur(src_gray, detected_edges, Size(kernel_size, kernel_size));
      dst = Scalar::all(0);
      // Canny detector
      Canny(detected_edges, detected_edges, trackbars[MIN_THRESH].val,
            // trackbars[MIN_THRESH].val * ratio, //swapped in next line
            trackbars[MAX_THRESH].val * ratio, kernel_size);
      src.copyTo(dst, detected_edges);
      // Using Canny's output as a mask, we display our result

			if (apply_lines) {

				// Edge detection
		    // Canny(src, dst, 50, 200, 3);
				detected_edges.copyTo(dst);
		    // Copy edges to the images that will display the results in BGR
		    cvtColor(dst, cdst, COLOR_GRAY2BGR);
		    cdstP = cdst.clone();
		    // Standard Hough Line Transform
		    vector<Vec2f> lines; // will hold the results of the detection
		    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection
		    // Draw the lines
		    for( size_t i = 0; i < lines.size(); i++ )
		    {
		        float rho = lines[i][0], theta = lines[i][1];
		        Point pt1, pt2;
		        double a = cos(theta), b = sin(theta);
		        double x0 = a*rho, y0 = b*rho;
		        pt1.x = cvRound(x0 + 1000*(-b));
		        pt1.y = cvRound(y0 + 1000*(a));
		        pt2.x = cvRound(x0 - 1000*(-b));
		        pt2.y = cvRound(y0 - 1000*(a));
		        line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
		    }
		    // Probabilistic Line Transform
		    vector<Vec4i> linesP; // will hold the results of the detection
		    HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
		    // Draw the lines
		    for( size_t i = 0; i < linesP.size(); i++ )
		    {
		        Vec4i l = linesP[i];
		        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
		    }
				cdstP.copyTo(dst);
			}
      if (apply_contuors) {
        cout << "applied" << endl;
        findContours(detected_edges, contours, hierarchy, RETR_TREE,
                     CHAIN_APPROX_SIMPLE, Point(0, 0));
        // Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        dst = Scalar::all(0);
        Scalar color1 = Scalar(255, 0, 0);
        Scalar color2 = Scalar(0, 255, 0);

        vector<vector<Point>> hull(contours.size());
        for (size_t i = 0; i < contours.size(); i++) {
          convexHull(Mat(contours[i]), hull[i], false);
        }
        // Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
        for (size_t i = 0; i < contours.size(); i++) {
          Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
                                rng.uniform(0, 255));
          // drawContours(dst, contours, (int)i, color1, 1, 8, vector<Vec4i>(),
          //              0, Point());
          drawContours(dst, hull, (int)i, color2, 1, 8, vector<Vec4i>(), 0,
                       Point());
        }
      }
      ///////////////////////////////////////////////////////////////////
    }
  }


  /// Colored mask edge detection
  /*//////////////////////////ImageManipulation////////////////////////
		// cout << "GstEdgeDetect.filter()" << endl;
    // Reduce noise with a kernel 3x3
    blur(src_gray, detected_edges, Size(kernel_size, kernel_size));
    dst = Scalar::all(0);
    // Canny detector
    Canny(detected_edges, detected_edges,
          trackbars[MIN_THRESH].val,
          // trackbars[MIN_THRESH].val * ratio, //swapped in next line
					trackbars[MAX_THRESH].val * ratio,
          kernel_size);
		src.copyTo(dst, detected_edges);
    // Using Canny's output as a mask, we display our result
	/*/ //////////////////////////////////////////////////////////////

  /// Black and white edge detection
  /*//////////////////////////ImageManipulation/////////////////////////
			// cout << "GstEdgeDetect.filter()" << endl;
	    // Reduce noise with a kernel 3x3
	    blur(src_gray, detected_edges, Size(kernel_size, kernel_size));
	    // dst = Scalar::all(0);
	    // Canny detector
	    Canny(detected_edges, dst,
	          trackbars[MIN_THRESH].val,
	          // trackbars[MIN_THRESH].val * ratio, //swapped in next line
						trackbars[MAX_THRESH].val * ratio,
	          kernel_size);
	    // Using Canny's output as a mask, we display our result
		/*/ //////////////////////////////////////////////////////////////////
};

#endif
