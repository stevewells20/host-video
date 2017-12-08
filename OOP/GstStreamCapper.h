#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>

<<<<<<< HEAD
#include "GstAbsFilter.h"

=======
>>>>>>> 1e5d002238e70f6c0d0e20f573d38a49519d8911
#ifndef GSTSTREAMCAPPER_H					// avoid repeated expansion
#define GSTSTREAMCAPPER_H

using namespace cv;
using namespace std;

enum Codec { H264, VP8, RAW };        // row
enum Element { DEPAY, DECODE, SYNC }; // col

class GstStreamCapper {
private:
  // Future implementation of codec selection parsing, for quick changes
<<<<<<< HEAD
	GstAbsFilter* gstFilters;
=======
>>>>>>> 1e5d002238e70f6c0d0e20f573d38a49519d8911

  string codecStore[3][3] = {
      {"rtph264depay", "avdec_h264 skip-frame=0", "false"}, // H264
      {"rtpvp8depay", "avdec_vp8", "true"},                 // VP8
      {"rtprawdepay", "", "true"}                           // RAW
  };

  string port = "5000";
  string _capFullString;

  // typedef void (*cvCallBack)(int, void *);  // Defines a function pointer
  // type pointing to a void function which doesn't take any parameter.
  // cvCallBack callback;  //  Actually defines a member variable of this type.

  void _make_pipe() {
<<<<<<< HEAD
		cout << "._make_pipe" <<endl;
    _capFullString =
        "udpsrc port=" + port + " caps=application/x-rtp ! " +
        codecStore[codec][DEPAY] + " ! " + codecStore[codec][DECODE] + " ! " +
        /* /// Comment out for no preview window ///
=======
    _capFullString =
        "udpsrc port=" + port + " caps=application/x-rtp ! " +
        codecStore[codec][DEPAY] + " ! " + codecStore[codec][DECODE] + " ! " +
        /// Comment out for no preview window ///
>>>>>>> 1e5d002238e70f6c0d0e20f573d38a49519d8911
        "tee name=split ! "
        "queue ! "
        "videoconvert ! "
        "autovideosink sync=" +
        codecStore[codec][SYNC] + " split. ! "
        "queue ! "
        /////////////////////////////////////////*/
        "videoconvert ! "
        "appsink ";

    cap.open(_capFullString.c_str());
  }

public:
  Codec codec = H264;
  Mat src, src_gray, dst;
  VideoWriter writer;
  VideoCapture cap;
<<<<<<< HEAD
	string windowName = "window1";
=======
>>>>>>> 1e5d002238e70f6c0d0e20f573d38a49519d8911

  GstStreamCapper() {
    cout << "GstStreamReceiver created" << endl;
    _make_pipe();
    if (!cap.isOpened()) {
      printf("=ERR= can't create video capture\n");
    } else {
      gstPrime();
      cout << _capFullString << endl;
    }
  }

  GstStreamCapper(int portNum) {
    cout << "GstStreamReceiver created" << endl;
    setPort(portNum);
    _make_pipe();
    if (!cap.isOpened()) {
      printf("=ERR= can't create video capture\n");
    } else {
<<<<<<< HEAD
			gstPrime();
=======
>>>>>>> 1e5d002238e70f6c0d0e20f573d38a49519d8911
      cout << _capFullString << endl;
    }
  }

<<<<<<< HEAD
  ~GstStreamCapper() {
		cout << "Cleaning up GstStreamCapper" << endl;
	}

	void addFilter(GstAbsFilter *filter) {
		gstFilters = filter;
	}

  void setPort(int portNum) {
		cout << ".setPort" <<endl;
		port = to_string(portNum); }

	void snapshot(string imgName="snapshot.jpg") {
		cout << ".snapshot" <<endl;
		imwrite(imgName, dst); }

  void resetCap() {
		cout << ".resetCap" <<endl;
    cap.release();
    _make_pipe();
  }

  void gstPrime() {
		cout << ".gstPrimed" <<endl;
		cap >> src;
    cvtColor(src, src_gray, CV_BGR2GRAY);
    src.copyTo(dst);
  }

	void run() {
		cout << ".run" <<endl;
		while (true) {

			gstFilters->filter(src, src_gray, dst);
			imshow(windowName, dst);

			cap >> src;
			cvtColor(src, src_gray, CV_BGR2GRAY);
			src.copyTo(dst);

			int key;
			key = waitKey(30);
		}
	}

=======
  ~GstStreamCapper() { ; }

  void setPort(int portNum) { port = to_string(portNum); }

  void resetCap() {
    cap.release();
    _make_pipe();
  }
  void gstPrime() {
		cap >> src;
    cvtColor(src, src_gray, CV_BGR2GRAY);
    dst.create(src.size(), src.type());
  }
>>>>>>> 1e5d002238e70f6c0d0e20f573d38a49519d8911
};

#endif
