/******************************************
 * OpenCV Tutorial: Ball Tracking using   *
 * Kalman Filter                          *
 ******************************************/

// Module "core"
#include <opencv2/core/core.hpp>

// Module "highgui"
#include <opencv2/highgui/highgui.hpp>

// Module "imgproc"
#include <opencv2/imgproc/imgproc.hpp>

// Module "video"
#include <opencv2/video/video.hpp>

// Output
#include <iostream>

// Vector
#include <vector>

#include <algorithm>

using namespace std;

// // >>>>> Color to be tracked
// #define MIN_HUE 100 //200
// #define MAX_HUE 200 //300
// #define MIN_VAL 30 //80
// #define MAX_VAL 45 //255
// // <<<<< Color to be tracked

// >>>>> Color to be tracked
// if(!open(default.hsv)) {
	//os::
// }
//////// MOUSE DRAG
bool calibrationMode = true;//used for showing debugging windows, trackbars etc.
bool mouseIsDragging = false;//used for showing a rectangle on screen as user clicks and drags mouse
bool mouseMove = false;
bool rectangleSelected = false;
cv::Point initialClickPoint, currentMousePoint; //keep track of initial point clicked and current position of mouse
cv::Rect rectangleROI; //this is the ROI that the user has selected
//////// MOUSE DRAG

string windowName = "Tracker";

///// funcs
void updateTrackBars(cv::Point clickPoint, cv::Mat* feed);
void clickAndDrag_Rectangle(int event, int x,
  int y, int flags, void* param);
void onTrackbar(int, void*);
int clamp(int clampee, int hbound=255, int lbound=0);
///// funcs

int MIN_HUE = 37; //200
int MAX_HUE = 71; //300
int MIN_SAT = 122;
int MAX_SAT = 255;
int MIN_VAL = 177; //80
int MAX_VAL = 255; //255

// <<<<< Color to be tracked
cv::Mat rangeRes;
cv::Mat frmHsv;
cv::Mat predTest;
cv::Mat frame;

int blurLevel = 300;

int stateSize = 6;

int measSize = 4;

int contrSize = 0;

unsigned int type = CV_32F;

cv::KalmanFilter kf(stateSize, measSize, contrSize, type);

cv::Mat state(stateSize, 1, type);  // [x,y,v_x,v_y,w,h]

cv::Mat meas(measSize, 1, type);    // [z_x,z_y,z_w,z_h]

int idx = 0;

cv::VideoCapture cap;

double ticks = 0;
bool found = false;

int notFoundCount = 0;

int main()
{

    // Camera frame

    // >>>> Kalman Filter
    //cv::Mat procNoise(stateSize, 1, type)
    // [E_x,E_y,E_v_x,E_v_y,E_w,E_h]

    // Transition State Matrix A
    // Note: set dT at each processing step!
    // [ 1 0 dT 0  0 0 ]
    // [ 0 1 0  dT 0 0 ]
    // [ 0 0 1  0  0 0 ]
    // [ 0 0 0  1  0 0 ]
    // [ 0 0 0  0  1 0 ]
    // [ 0 0 0  0  0 1 ]
    cv::setIdentity(kf.transitionMatrix);

    // Measure Matrix H
    // [ 1 0 0 0 0 0 ]
    // [ 0 1 0 0 0 0 ]
    // [ 0 0 0 0 1 0 ]
    // [ 0 0 0 0 0 1 ]
    kf.measurementMatrix = cv::Mat::zeros(measSize, stateSize, type);
    kf.measurementMatrix.at<float>(0) = 1.0f;
    kf.measurementMatrix.at<float>(7) = 1.0f;
    kf.measurementMatrix.at<float>(16) = 1.0f;
    kf.measurementMatrix.at<float>(23) = 1.0f;

    // Process Noise Covariance Matrix Q
    // [ Ex   0   0     0     0    0  ]
    // [ 0    Ey  0     0     0    0  ]
    // [ 0    0   Ev_x  0     0    0  ]
    // [ 0    0   0     Ev_y  0    0  ]
    // [ 0    0   0     0     Ew   0  ]
    // [ 0    0   0     0     0    Eh ]
    //cv::setIdentity(kf.processNoiseCov, cv::Scalar(1e-2));
    kf.processNoiseCov.at<float>(0) = 1e-2;
    kf.processNoiseCov.at<float>(7) = 1e-2;
    kf.processNoiseCov.at<float>(14) = 5.0f;
    kf.processNoiseCov.at<float>(21) = 5.0f;
    kf.processNoiseCov.at<float>(28) = 1e-2;
    kf.processNoiseCov.at<float>(35) = 1e-2;

    // Measures Noise Covariance Matrix R
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));
    // <<<< Kalman Filter

    // Camera Capture

		enum Codec { H264, VP8, RAW };        // row
		enum Element { DEPAY, DECODE, SYNC }; // col
		string codecStore[3][3] = {
	      {"rtph264depay", "avdec_h264 skip-frame=0", "false"}, // H264
	      {"rtpvp8depay", "avdec_vp8", "true"},                 // VP8
	      {"rtprawdepay", "", "true"}                           // RAW
	  };

	  string port = "5000";
		Codec codec = H264;
	  string _capFullString =
	        "udpsrc port=" + port + " caps=application/x-rtp ! " +
	        codecStore[codec][DEPAY] + " ! " + codecStore[codec][DECODE] + " ! " +
	        /// Comment out for no preview window ///
	        // "tee name=split ! "
	        // "queue ! "
	        // "videoconvert ! "
	        // "autovideosink sync=" +
	        // codecStore[codec][SYNC] + " split. ! "
	        // "queue ! "
	        /////////////////////////////////////////*/
	        "videoconvert ! "
	        "appsink ";
					// >>>>> Camera Settings
		if (!cap.open(_capFullString.c_str()))
    // if (!cap.open(idx))
    {
        cout << "Webcam not connected.\n" << "Please verify\n";
        return EXIT_FAILURE;
    }

    // cap.set(CV_CAP_PROP_FRAME_WIDTH, 740);
    // cap.set(CV_CAP_PROP_FRAME_HEIGHT, 680);

		frame.convertTo(frame, CV_32F);

		// <<<<< Camera Settings

    cout << "\nHit 'q' to exit...\n";


    char ch = 0;

    cv::namedWindow(windowName, 1);

		cv::setMouseCallback(windowName, clickAndDrag_Rectangle, &frmHsv);

    cv::createTrackbar( "MIN_HUE", windowName, &MIN_HUE, 179, onTrackbar );
    cv::createTrackbar( "MAX_HUE", windowName, &MAX_HUE, 179, onTrackbar );
		cv::createTrackbar( "MIN_SAT", windowName, &MIN_SAT, 255, onTrackbar );
		cv::createTrackbar( "MAX_SAT", windowName, &MAX_SAT, 255, onTrackbar );
		cv::createTrackbar( "MIN_VAL", windowName, &MIN_VAL, 255, onTrackbar );
    cv::createTrackbar( "MAX_VAL", windowName, &MAX_VAL, 255, onTrackbar );
		cv::createTrackbar( "blurLevel", windowName, &blurLevel, 1000, onTrackbar );

    // >>>>> Main loop
    while (ch != 'q' && ch != 'Q')
    {
      onTrackbar(0,0);

        // User key
        ch = cv::waitKey(1);
    }
    // <<<<< Main loop

    return EXIT_SUCCESS;
}

void onTrackbar(int, void*) {
  double precTick = ticks;
  ticks = (double) cv::getTickCount();

  double dT = (ticks - precTick) / cv::getTickFrequency(); //seconds

  // Frame acquisition
  cap >> frame;

  cv::Mat res;
  frame.copyTo( res );

  if (found)
  {
      // >>>> Matrix A
      kf.transitionMatrix.at<float>(2) = dT;
      kf.transitionMatrix.at<float>(9) = dT;
      // <<<< Matrix A

      // cout << "dT:" << dT << endl;

      state = kf.predict();
      // cout << "State post:" << endl << state << endl;

      cv::Rect predRect;
      predRect.width = state.at<float>(4);
      predRect.height = state.at<float>(5);
      predRect.x = state.at<float>(0) - predRect.width / 2;
      predRect.y = state.at<float>(1) - predRect.height / 2;

      cv::Point center;
      center.x = state.at<float>(0);
      center.y = state.at<float>(1);
      cv::circle(res, center, 2, CV_RGB(255,0,0), -1);

      cv::rectangle(res, predRect, CV_RGB(255,0,0), 2);
  }

  // >>>>> Noise smoothing
  cv::Mat blur;
  cv::GaussianBlur(frame, blur, cv::Size(5, 5), float(blurLevel/100), float(blurLevel/100));
	// cv::imshow("frame",frame);
	// cv::imshow("blur",blur);
  // <<<<< Noise smoothing

  // >>>>> HSV conversion
  cv::cvtColor(blur, frmHsv, CV_BGR2HSV);
  // <<<<< HSV conversion

  // >>>>> Color Thresholding
  // Note: change parameters for different colors
  rangeRes = cv::Mat::zeros(frame.size(), CV_8UC1);

  cv::inRange(frmHsv,
    cv::Scalar(MIN_HUE, MIN_SAT, MIN_VAL),
    cv::Scalar(MAX_HUE, MAX_SAT, MAX_VAL),
    rangeRes);
  // <<<<< Color Thresholding

  // >>>>> Improving the result
  cv::erode(rangeRes, rangeRes, cv::Mat(), cv::Point(-1, -1), 2);
  cv::dilate(rangeRes, rangeRes, cv::Mat(), cv::Point(-1, -1), 2);
  // <<<<< Improving the result

  // Thresholding viewing
  cv::imshow("Threshold", rangeRes);

  // >>>>> Contours detection
  vector<vector<cv::Point> > contours;
  cv::findContours(rangeRes, contours, CV_RETR_EXTERNAL,
                   CV_CHAIN_APPROX_NONE);
  // <<<<< Contours detection

  // >>>>> Filtering
  vector<vector<cv::Point> > balls;
  vector<cv::Rect> ballsBox;
  for (size_t i = 0; i < contours.size(); i++)
  {
      cv::Rect bBox;
      bBox = cv::boundingRect(contours[i]);

      float ratio = (float) bBox.width / (float) bBox.height;
      if (ratio > 1.0f)
          ratio = 1.0f / ratio;

      // Searching for a bBox almost square
      if (ratio > 0.75 && bBox.area() >= 400)
      {
          balls.push_back(contours[i]);
          ballsBox.push_back(bBox);
      }
  }
  // <<<<< Filtering

  // cout << "Balls found:" << ballsBox.size() << endl;

  // >>>>> Detection result
	predTest = cv::Mat(res.size(),res.type(), cv::Scalar::all(0));

  for (size_t i = 0; i < balls.size(); i++)
  {
      cv::drawContours(res, balls, i, CV_RGB(20,150,20), 1);
      cv::rectangle(res, ballsBox[i], CV_RGB(0,255,0), 2);

      cv::Point center;
      center.x = ballsBox[i].x + ballsBox[i].width / 2;
      center.y = ballsBox[i].y + ballsBox[i].height / 2;
      cv::circle(res, center, 2, CV_RGB(20,150,20), -1);

      stringstream sstr;
      sstr << "(" << center.x << "," << center.y << ")";
      cv::putText(res, sstr.str(),
                  cv::Point(center.x + 3, center.y - 3),
                  cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(20,150,20), 2);

			res(ballsBox[i]).copyTo(predTest(ballsBox[i]));

  }
  // <<<<< Detection result

  // >>>>> Kalman Update
  if (balls.size() == 0)
  {
      notFoundCount++;
      // cout << "notFoundCount:" << notFoundCount << endl;
      if( notFoundCount >= 100 )
      {
          found = false;
      }
      /*else
          kf.statePost = state;*/
  }
  else
  {
      notFoundCount = 0;

      meas.at<float>(0) = ballsBox[0].x + ballsBox[0].width / 2;
      meas.at<float>(1) = ballsBox[0].y + ballsBox[0].height / 2;
      meas.at<float>(2) = (float)ballsBox[0].width;
      meas.at<float>(3) = (float)ballsBox[0].height;

      if (!found) // First detection!
      {
          // >>>> Initialization
          kf.errorCovPre.at<float>(0) = 1; // px
          kf.errorCovPre.at<float>(7) = 1; // px
          kf.errorCovPre.at<float>(14) = 1;
          kf.errorCovPre.at<float>(21) = 1;
          kf.errorCovPre.at<float>(28) = 1; // px
          kf.errorCovPre.at<float>(35) = 1; // px

          state.at<float>(0) = meas.at<float>(0);
          state.at<float>(1) = meas.at<float>(1);
          state.at<float>(2) = 0;
          state.at<float>(3) = 0;
          state.at<float>(4) = meas.at<float>(2);
          state.at<float>(5) = meas.at<float>(3);
          // <<<< Initialization

          kf.statePost = state;

          found = true;
      }
      else
          kf.correct(meas); // Kalman Correction

      // cout << "Measure matrix:" << endl << meas << endl;
  }
  // <<<<< Kalman Update

  // Final result
  cv::imshow(windowName, res);
	cv::imshow("test", predTest);

}

///////////////////
/// Extra Funcs ///
///////////////////

void clickAndDrag_Rectangle(int event, int x, int y, int flags, void* param){
	//only if calibration mode is true will we use the mouse to change HSV values
  // cv::Mat* matParam = (cv::Mat*)param;

  if (calibrationMode == true){
		//get handle to video feed passed in as "param" and cast as Mat pointer
		cv::Mat* videoFeed = (cv::Mat*)param;

		if (event == cv::EVENT_LBUTTONDOWN && mouseIsDragging == false)
		{
			//keep track of initial point clicked
			initialClickPoint = cv::Point(x, y);
      updateTrackBars(initialClickPoint, videoFeed);
			//user has begun dragging the mouse
			mouseIsDragging = true;
		}
		/* user is dragging the mouse */
		if (event == cv::EVENT_MOUSEMOVE && mouseIsDragging == true)
		{
			//keep track of current mouse point
			currentMousePoint = cv::Point(x, y);
			//user has moved the mouse while clicking and dragging
			mouseMove = true;
		}
		/* user has released left button */
		if (event == cv::EVENT_LBUTTONUP && mouseIsDragging == true)
		{
			//set rectangle ROI to the rectangle that the user has selected
			rectangleROI = cv::Rect(initialClickPoint, currentMousePoint);

			//reset boolean variables
			mouseIsDragging = false;
			mouseMove = false;
			rectangleSelected = true;
		}

		if (event == cv::EVENT_RBUTTONDOWN){
			//user has clicked right mouse button
			//Reset HSV Values
			MIN_HUE = 0;
			MIN_SAT = 0;
			MIN_VAL = 0;
			MAX_HUE = 255;
			MAX_SAT = 255;
			MAX_VAL = 255;

		}
		if (event == cv::EVENT_MBUTTONDOWN){
			//user has clicked middle mouse button
			//enter code here if needed.
		}
	}
}

void updateTrackBars(cv::Point clickPoint, cv::Mat* feed) {
  cv::Vec3b hsvPix = feed->at<cv::Vec3b>(clickPoint);
  cout << hsvPix << endl;
  int h_dev = 7;
  int s_dev = 32;
  int v_dev = 80;
	int h_max = 179;
  cv::setTrackbarPos("MIN_HUE", windowName, clamp(hsvPix[0]-h_dev, h_max) );
  cv::setTrackbarPos("MIN_SAT", windowName, clamp(hsvPix[1]-s_dev, h_max) );
  cv::setTrackbarPos("MIN_VAL", windowName, clamp(hsvPix[2]-v_dev) );
  cv::setTrackbarPos("MAX_HUE", windowName, clamp(hsvPix[0]+h_dev) );
  cv::setTrackbarPos("MAX_SAT", windowName, clamp(hsvPix[1]+s_dev) );
  cv::setTrackbarPos("MAX_VAL", windowName, clamp(hsvPix[2]+v_dev) );

}

int clamp(int clampee, int hbound, int lbound){
	clampee = max(clampee, lbound);
	return min(clampee, hbound);
}
