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

enum SourceType {UDP, VID, CAM};
enum CodecType { H265, H264, VP8, RAW, NONE };        // row
enum Element { DEPAY, DECODE, SYNC }; // col

class GstStreamCapper {
private:
  // Future implementation of codec selection parsing, for quick changes
  GstAbsFilter *gstFilters[10];
  int currentFilterCount = 0;

  string codecStore[3][3] = {
      {"rtph264depay",  "avdec_h264", "false"}, // H264
      {"rtpvp8depay",   "avdec_vp8", "true"}, // VP8
      {"rtprawdepay", "", "true"} // RAW
  };

  // typedef void (*cvCallBack)(int, void *);  // Defines a function pointer
  // type pointing to a void function which doesn't take any parameter.
  // cvCallBack callback;  //  Actually defines a member variable of this type.


	// void _assemble_get_ROI() {
	// 	void* userData = nullptr;
 // 		setMouseCallback( windowName, get_ROI, userData );
	// }

public:
  CodecType codec = H264;
  Mat src, src_gray, dst;
  VideoWriter writer;
  VideoCapture cap;
	Rect2d ROI;

  string port = "5000";
  string _source = "";
  string _decode = "";
  string _live = "";
  string _sink = "";
  string _sync = "false";
  string _capFullString = "";

  SourceType sourceType = UDP; // options are UDP, VID, CAM
  string videoFileLocation = "";
  CodecType codecType = H264; // H264, VP8, RAW, NONE
  bool showLive = false;
	bool grabVid = false;
	string vidSaveLoc = "video.mp4";

  string windowName = "window1";


    void _make_pipe() {

      switch (sourceType) {
        case UDP: _source += "udpsrc port=" + port + " caps=application/x-rtp ! ";
                  break;
        case VID: _source += "filesrc location=" + videoFileLocation + " ! queue ! decodebin ! ";
                  codecType = NONE;
                  _sync="true";
                  break;
        case CAM: _source += "v4l2src ! ";
                  break;
        default:  cout << "Invalid source, setting to cam source...";
                  sourceType = CAM;
                  _source += "v4l2src ! ";
                  break;
      }

      switch (codecType) {
        case H265:  if (sourceType == UDP) _decode += "queue ! rtph265depay ! ";
                    _decode += "queue ! h265parse ! queue ! avdec_h265 ! ";
                    break;
        case H264:  if (sourceType == UDP) _decode += "queue ! rtph264depay ! ";
                    _decode += "queue ! h264parse ! queue ! avdec_h264 ! ";
                    break;
        case VP8:   if (sourceType == UDP) _decode += "queue ! rtpvp8depay ! ";
                    _decode += "queue ! vp8dec threads=4";
                    break;
        case RAW:   if (sourceType == UDP) _decode += "queue ! rtprawdepay ! ";
                    _decode += "queue ! rawvideoparse ! ";
                    break;
        case NONE:  _decode += "";
                    break;
        default:  cout << "Invalid decode, setting to none...";
                  codecType = NONE;
                  _decode = "";
                  break;
      }

      if (showLive) _live = "tee name=pSplit ! queue ! videoconvert ! autovideosink sync="+ _sync +" pSplit. ! ";

      _sink = "videoconvert ! appsink drop=true sync="+ _sync;
      cap.open((_source + _decode + _live + _sink).c_str());
    	if( cap.isOpened() )
  		cout << "Video " << //parser.get<string>("c") <<
  				": width=" << cap.get(CAP_PROP_FRAME_WIDTH) <<
  				", height=" << cap.get(CAP_PROP_FRAME_HEIGHT) << endl;
    }
    //
    // void _make_pipe() {
    //   cout << "._make_pipe" << endl;
    //   if (camSource) _capFullString +=
    //       "v4l2src ! ";
    //   else _capFullString +=
    //       "udpsrc port=" + port + " caps=application/x-rtp ! " +
    //       "h264parse !" +
    //       codecStore[codec][DEPAY] + " ! " +
  	// 			codecStore[codec][DECODE];
    //   if (showLive) _capFullString +=
  	// 	    "tee name=pSplit ! "
    //       "queue ! "
    //       "videoconvert ! "
    //       "autovideosink sync=" + codecStore[codec][SYNC] + " pSplit. ! "
    //       "queue ! ";
  	// 	// if (grabVid) _capFullString +=
  	// 	// 		"tee name=vSplit ! "
  	// 	// 		"queue ! "
  	// 	// 		// "h264parse ! "
  	// 	// 		"mpegtsmux ! "
  	// 	// 		"filesink location=" + vidSaveLoc + " vSplit. ! "
  	// 	// 		"queue";
    //   _capFullString +=
  	// 	    "videoconvert ! "
    //       "appsink drop=true sync=false";
  	// 	cout << _capFullString << endl;
    //   cap.open(_capFullString.c_str());
  	// 	if( cap.isOpened() )
  	// 	cout << "Video " << //parser.get<string>("c") <<
  	// 			": width=" << cap.get(CAP_PROP_FRAME_WIDTH) <<
  	// 			", height=" << cap.get(CAP_PROP_FRAME_HEIGHT) <<
  	// 			", nframes=" << cap.get(CAP_PROP_FRAME_COUNT) << endl;
    // }

  	void _assemble_trackbars() {
      for (int g = 0; g < currentFilterCount; g++) {
        for (int t = 0; t < gstFilters[g]->numTrackbars; t++) {
  				cout << "numTrackbars : " << gstFilters[g]->numTrackbars << endl;
  				cout << "Creating Trackbar: " << gstFilters[g]->trackbars[t].name << endl;
          createTrackbar(gstFilters[g]->trackbars[t].name, windowName,
                         &gstFilters[g]->trackbars[t].val,
                         gstFilters[g]->trackbars[t].cap,
                         gstFilters[g]->trackbars[t].onChange);
        }
      }
    }

	void get_ROI() {
		bool showCrosshair = true;
		bool fromCenter = false;
		ROI = selectROI(windowName, src, showCrosshair, fromCenter);
	}

  GstStreamCapper() {
    cout << "GstStreamReceiver created" << endl;
    //_make_pipe();

  }

	GstStreamCapper(int portNum) {
    cout << "GstStreamReceiver created" << endl;
    setPort(portNum);
    // _make_pipe();
    // if (!cap.isOpened()) {
    //   printf("=ERR= can't create video capture\n");
    // } else {
    //   cout << _capFullString << endl;
    // }
  }

	GstStreamCapper(int portNum, int showLiveFeed) {
	    cout << "GstStreamReceiver created" << endl;
	    setPort(portNum);
			showLive = showLiveFeed;
	    // _make_pipe();
	    // if (!cap.isOpened()) {
	    //   printf("=ERR= can't create video capture\n");
	    // } else {
	    //   cout << _capFullString << endl;
	    // }
	  }

  ~GstStreamCapper() { cout << "Cleaning up GstStreamCapper" << endl; }

  void addFilter(GstAbsFilter *filter) {
    if (currentFilterCount >= 10)
      throw "10 filters is currently the max count, "
            "please change it in GstStreamCapper.h for more";
    gstFilters[currentFilterCount] = filter;
    currentFilterCount++;
  }

	void removeFilter(GstAbsFilter *filter) {
		bool removed = false;
		for (int i = 0; i < currentFilterCount; i++) {
			if (gstFilters[i] == filter) {
				gstFilters[i] = nullptr;
				removed = true;
				currentFilterCount--;
				cout << "Filter removed!\n";
			}
			if (removed) gstFilters[i] = gstFilters[i+1];
			// cout << "Not working yet...\n";
		}
		gstFilters[currentFilterCount] = nullptr;
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
    _make_pipe();
    cout << ".gstPrimed" << endl;
    cap >> src;
    cvtColor(src, src_gray, CV_BGR2GRAY);
    src.copyTo(dst);
		ROI = Rect(); // x,y,cols,rows (x,y,w,h)
  }

  void run() {
    cout << ".run" << endl;
    namedWindow(windowName, WINDOW_NORMAL);

		cout << "gstPrime()";
		gstPrime();
		_assemble_trackbars();
		// for (int count = 0; count < framecount; count++) {
		char key = 0;
    while (key != 'q' && key != 'Q') {

      for (int i = 0; i < currentFilterCount; i++) {
        // cout << "Filter# : " << i << endl;
				// if (gstFilters[i] != nullptr)
      	gstFilters[i]->filter(src, src_gray, dst, ROI);
				if (dst.type() != src.type() ) {
					cvtColor(dst, src, CV_GRAY2RGB);
					src.copyTo(src_gray);
				} else {
					dst.copyTo(src);
					cvtColor(src, src_gray, CV_BGR2GRAY);
				}
        //dst.copyTo(src);
				// if (src.type() == CV_GR
        // cout << "End# : " << i << endl;
      }

      imshow(windowName, dst);

      cap >> src;
      cvtColor(src, src_gray, CV_BGR2GRAY);
      src.copyTo(dst);


			///// User interupts /////
			if (key == 's') { snapshot(); key = 0; }
			// if (key == 'v') { saveSrcVideo(); key = 0; }
			if (key == 'r') { get_ROI(); key = 0; }
      key = waitKey(20);
    }
  }
};

#endif
