#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>

using namespace cv;
using namespace std;

enum Codec {H264, VP8, RAW}; //row
enum Element {DEPAY, DECODE, SYNC}; //col
enum GstType {CAPTURE, WRITER};
class GstStreamer {
private:

// Future implementation of codec selection parsing, for quick changes

string codecStore[3][3] = {
	{"rtph264depay","avdec_h264 skip-frame=0", "false"},//H264
	{"rtpvp8depay", "avdec_vp8", "true"},//VP8
	{"rtprawdepay", "", "true"} //RAW
};

	string port = "5000";
	string _capFullString;
	string _writeFullString;

		// typedef void (*cvCallBack)(int, void *);  // Defines a function pointer type pointing to a void function which doesn't take any parameter.
		// cvCallBack callback;  //  Actually defines a member variable of this type.


void _make_cap_pipe() {
	_capFullString = "udpsrc port=" + port + " caps=application/x-rtp ! " +
	codecStore[codec][DEPAY] + " ! " +
	codecStore[codec][DECODE] + " ! " +
	"tee name=split ! "
	"queue ! "
	"videoconvert ! "
	"autovideosink sync=" + codecStore[codec][SYNC] + " split. ! "
	"queue ! "
	"videoconvert ! "
	"appsink ";

	cap.open(_capFullString.c_str());
}

void _make_write_pipe() {
	// _capFullString = "udpsrc port=" + port + " caps=application/x-rtp ! " +
	// codecStore[codec][DEPAY] + " ! " +
	// codecStore[codec][DECODE] + " ! " +
	// "tee name=split ! "
	// "queue ! "
	// "videoconvert ! "
	// "autovideosink sync=" + codecStore[codec][SYNC] + " split. ! "
	// "queue ! "
	// "videoconvert ! "
	// "appsink ";

	// cap.open(_capFullString.c_str());

	"v4l2src device=/dev/video0 ! "
          "video/x-raw, format=(string)I420, width=640,height=480, framerate=40/1 ! "
          "nvvidconv ! "
          "video/x-raw(memory:NVMM), format=(string)I420 ! "
          "omxh264enc low-latency=true ! "
          "rtph264pay config-interval=3 ! "
          "udpsink sync=false host=" + host + " port=" + port;

}

public:
	GstType gst_type = WRITER;
	Codec codec = H264;
  Mat src, src_gray, dst;
  VideoWriter writer;
  VideoCapture cap;

  GstStreamer() {
		cout << "GstStreamReceiver created" << endl;
		_make_cap_pipe();
		if (!cap.isOpened()) {
			printf("=ERR= can't create video capture\n");
		} else { cout << _capFullString << endl;}
	}

	GstStreamer(int portNum) {
		cout << "GstStreamReceiver created" << endl;
		setPort(portNum);
		_make_cap_pipe();
		if (!cap.isOpened()) {
			printf("=ERR= can't create video capture\n");
		} else { cout << _capFullString << endl;}
	}

  ~GstStreamer() { ; }

	void setPort(int portNum) {
		port = to_string(portNum);
	}

	void resetCap() {
		cap.release();
		_make_cap_pipe();

	}
};
