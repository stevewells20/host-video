#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>

#include "GstAbsFilter.h"

#ifndef GSTSTREAMCAPPER_H // avoid repeated expansion
#define GSTSTREAMCAPPER_H

using namespace cv;
using namespace std;

enum Codec { H264, VP8, RAW };        // row
enum Element { DEPAY, DECODE, SYNC }; // col

class GstStreamCapper {
private:
  // Future implementation of codec selection parsing, for quick changes
  GstAbsFilter *gstFilters[10];
  int currentFilterCount = 0;

  string codecStore[3][3] = {
      {"rtph264depay", "avdec_h264 skip-frame=5", "false"}, // H264
      {"rtpvp8depay", "avdec_vp8", "true"},                 // VP8
      {"rtprawdepay", "", "true"}                           // RAW
  };

  string port = "5000";
  string _capFullString;

  // typedef void (*cvCallBack)(int, void *);  // Defines a function pointer
  // type pointing to a void function which doesn't take any parameter.
  // cvCallBack callback;  //  Actually defines a member variable of this type.

  void _make_pipe() {
    cout << "._make_pipe" << endl;
    _capFullString =
        "udpsrc port=" + port + " caps=application/x-rtp ! " +
        codecStore[codec][DEPAY] + " ! " +
				codecStore[codec][DECODE];
    if (showLive) _capFullString += " ! "
		    "tee name=pSplit ! "
        "queue ! "
        "videoconvert ! "
        "autovideosink sync=" + codecStore[codec][SYNC] + " pSplit. ! "
        "queue";
		// if (grabVid) _capFullString += " ! "
		// 		"tee name=vSplit ! "
		// 		"queue ! "
		// 		// "h264parse ! "
		// 		"mpegtsmux ! "
		// 		"filesink location=" + vidSaveLoc + " vSplit. ! "
		// 		"queue";
    _capFullString += " ! "
		    "videoconvert ! "
        "appsink drop=true sync=false";
		cout << _capFullString << endl;
    cap.open(_capFullString.c_str());
  }

  void _assemble_trackbars() {
    for (int g = 0; g < currentFilterCount; g++) {
      for (int t = 0; t < gstFilters[g]->numTrackbars; t++) {
        createTrackbar(gstFilters[g]->trackbars[t].name,
											windowName,
                       &gstFilters[g]->trackbars[t].val,
                       gstFilters[g]->trackbars[t].cap,
                       gstFilters[g]->onChange);
      }
    }
  }

public:
  Codec codec = H264;
  Mat src, src_gray, dst;
  VideoWriter writer;
  VideoCapture cap;
  string windowName = "window1";
	bool showLive = false;
	bool grabVid = false;
	string vidSaveLoc = "video.mp4";

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
      gstPrime();
      cout << _capFullString << endl;
    }
  }

  ~GstStreamCapper() { cout << "Cleaning up GstStreamCapper" << endl; }

  void addFilter(GstAbsFilter *filter) {
    if (currentFilterCount >= 10)
      throw "10 filters is currently the max count, "
            "please change it in GstStreamCapper.h for more";
    gstFilters[currentFilterCount] = filter;
    currentFilterCount++;
  }

  void setPort(int portNum) {
    cout << ".setPort" << endl;
    port = to_string(portNum);
  }

  void snapshot(string imgName = "snapshot.jpg") {
    cout << ".snapshot" << endl;
    imwrite(imgName, dst);
  }

	// void saveSrcVideo(string vidName = "video.mpg") {
	// 	string addedVidSave = _capFullString + " ! videoconvert !"
	// 													"autovideosink sync=false";
	// 	cout << addedVidSave << endl;
	// 	cap.release();
	// 	cap.open(addedVidSave.c_str());
	// }

  void resetCap() {
    cout << ".resetCap" << endl;
    cap.release();
    _make_pipe();
  }

  void gstPrime() {
    cout << ".gstPrimed" << endl;
    cap >> src;
    cvtColor(src, src_gray, CV_BGR2GRAY);
    src.copyTo(dst);
  }

  void run() {
    cout << ".run" << endl;
    namedWindow(windowName, WINDOW_NORMAL);
    _assemble_trackbars();
		// int framecount = 100;
		// for (int count = 0; count < framecount; count++) {
		char key = 0;
    while (key != 'q' && key != 'Q') {

      for (int i = 0; i < currentFilterCount; i++) {
        gstFilters[i]->filter(src, src_gray, dst);
      }

      imshow(windowName, dst);

      cap >> src;
      cvtColor(src, src_gray, CV_BGR2GRAY);
      src.copyTo(dst);
			if (key == 's') { snapshot(); key = 0; }
			// if (key == 'v') { saveSrcVideo(); key = 0; }
      key = waitKey(20);
    }
  }
};

#endif
