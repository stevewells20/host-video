// Compile with: $ g++ opencv_gst.cpp -o opencv_gst `pkg-config --cflags --libs
// opencv`

#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>

using namespace cv;
using namespace std;

//![canny.variables]
Mat detected_edges;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
const char *window_name = "Edge Map";

//![circles.variables]
int minRad = 83;
int maxRad = 104;
int dispersionDist = 50;

//![hog.variables]
HOGDescriptor hog;
int hogScale = 100;

//![face.variables]
bool tryflip;
CascadeClassifier cascade("/home/steven/JetsonCodes/vidstream/src/opencv/data/"
                          "haarcascades/haarcascade_frontalface_alt.xml");
CascadeClassifier
    nestedCascade("/home/steven/JetsonCodes/vidstream/src/opencv/data/"
                  "haarcascades/haarcascade_eye_tree_eyeglasses.xml");
double scale = 1.0;
double t = 0;
vector<Rect> faces, faces2;
const static Scalar colors[] = {Scalar(255, 0, 0),   Scalar(255, 128, 0),
                                Scalar(255, 255, 0), Scalar(0, 255, 0),
                                Scalar(0, 128, 255), Scalar(0, 255, 255),
                                Scalar(0, 0, 255),   Scalar(255, 0, 255)};

//![global.variables]
Mat src, src_gray, dst;
VideoWriter writer;
VideoCapture cap;

static void HOGDetect(int, void *) {
  while (true) {
    cap >> src;
    if (src.empty())
      return;
    src.copyTo(dst);
    vector<Rect> found, found_filtered;
    double t = (double)getTickCount();
    // Run the detector with default parameters. to get a higher hit-rate
    // (and more false alarms, respectively), decrease the hitThreshold and
    // groupThreshold (set groupThreshold to 0 to turn off the grouping
    // completely).
    hog.detectMultiScale(src, found, 0, Size(8, 8), Size(32, 32),
                         double(hogScale / 100.0), 2);
    cout << double(hogScale / 100.0) << endl;
    t = (double)getTickCount() - t;
    cout << "detection time = " << (t * 1000. / cv::getTickFrequency()) << " ms"
         << endl;
    for (size_t i = 0; i < found.size(); i++) {
      Rect r = found[i];
      size_t j;
      // Do not add small detections inside a bigger detection.
      for (j = 0; j < found.size(); j++)
        if (j != i && (r & found[j]) == r)
          break;
      if (j == found.size())
        found_filtered.push_back(r);
    }
    for (size_t i = 0; i < found_filtered.size(); i++) {
      Rect r = found_filtered[i];
      // The HOG detector returns slightly larger rectangles than the real
      // objects,
      // so we slightly shrink the rectangles to get a nicer output.
      r.x += cvRound(r.width * 0.1);
      r.width = cvRound(r.width * 0.8);
      r.y += cvRound(r.height * 0.07);
      r.height = cvRound(r.height * 0.8);
      rectangle(dst, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
    }
    // writer << src;

    imshow(window_name, dst);
    int key;
    key = waitKey(30);
  }
}

void CannyThreshold(int, void *) {
  while (true) {

    cap >> src;
    if (src.empty())
      return;

    // Convert the image to grayscale
    cvtColor(src, src_gray, CV_BGR2GRAY);
    // Reduce noise with a kernel 3x3
    blur(src_gray, detected_edges, Size(kernel_size, kernel_size));
    // Canny detector
    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio,
          kernel_size);
    // Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);

    src.copyTo(dst, detected_edges);
    // writer << src;

    imshow(window_name, dst);
    int key;
    key = waitKey(30);
  }
}

void CircleHoughs(int, void *) {
  while (true) {
    cap >> src;
    if (src.empty())
      return;
    src.copyTo(dst);

    // Convert the image to grayscale
    cvtColor(src, src_gray, CV_BGR2GRAY);

    medianBlur(src_gray, src_gray, 5);
    vector<Vec3f> circles;
    HoughCircles(src_gray, circles, HOUGH_GRADIENT, 1,
                 src_gray.rows / dispersionDist, // change this value to detect
                                                 // circles with differen$
                 100, 30, minRad,
                 maxRad // change the last two parameters (1, 30)
                 // (min_radius & max_radius) to detect larger circles
                 );
    for (size_t i = 0; i < circles.size(); i++) {
      Vec3i c = circles[i];
      Point center = Point(c[0], c[1]);
      // circle center
      circle(dst, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
      // circle outline
      int radius = c[2];
      circle(dst, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
    }
    // writer << src;

    imshow(window_name, dst);
    int key;
    key = waitKey(30);
  }
}

void FaceDetect(int, void *) {
  while (true) {
    cap >> src;
    if (src.empty())
      return;
    src.copyTo(dst);
    // Convert the image to grayscale
    cvtColor(src, src_gray, CV_BGR2GRAY);
    ////////////
    Mat smallImg;
    double fx = 1 / scale;
    resize(src_gray, smallImg, Size(), fx, fx, INTER_LINEAR);
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
      Scalar color = colors[i % 8];
      int radius;
      double aspect_ratio = (double)r.width / r.height;
      if (0.75 < aspect_ratio && aspect_ratio < 1.3) {
        center.x = cvRound((r.x + r.width * 0.5) * scale);
        center.y = cvRound((r.y + r.height * 0.5) * scale);
        radius = cvRound((r.width + r.height) * 0.25 * scale);
        circle(dst, center, radius, color, 3, 8, 0);
      } else
        rectangle(dst, cvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
                  cvPoint(cvRound((r.x + r.width - 1) * scale),
                          cvRound((r.y + r.height - 1) * scale)),
                  color, 3, 8, 0);
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
        circle(dst, center, radius, color, 3, 8, 0);
      }
    }

    ///////////
    // writer << src;

    imshow(window_name, dst);
    int key;
    key = waitKey(30);
  }
}

void p_hog() {
  hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
  /// Create a Trackbar for user to enter threshold
  createTrackbar("HOG Scale:", window_name, &hogScale, 200, HOGDetect);
  /// Show the image
  HOGDetect(0, 0);
}

void h_circles() {
  /// Create a Trackbar
  createTrackbar("Min Radius:", window_name, &minRad, 200, CircleHoughs);
  createTrackbar("Max Radius:", window_name, &maxRad, 200, CircleHoughs);
  createTrackbar("Min Dispersion:", window_name, &dispersionDist, 200,
                 CircleHoughs);
  /// Show the image
  CircleHoughs(0, 0);
}

void canny() {
  /// Create a Trackbar for user to enter threshold
  createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold,
                 CannyThreshold);
  /// Show the image
  CannyThreshold(0, 0);
}

void face_d() {
  /// Create a Trackbar for user to enter threshold
  createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold,
                 FaceDetect);
  /// Show the image
  FaceDetect(0, 0);
}

int main(int argc, char **argv) {
  string port = "5000";
  if (argc > 1) {
    port = argv[1];
  }

  // first part of sender pipeline
  cap.open(
      "udpsrc port=" + port +
      " caps=application/x-rtp ! "
      "rtph264depay ! "
      "avdec_h264 skip-frame=0 ! "
      //"video/x-raw, format=BGR, width=640,height=480 ! "
      //"videoconvert ! "
      //"video/x-raw, framerate=40/1, width=640, height=480, format=I420 ! "
      //"videoconvert ! "
      //"video/x-raw, framerate=40/1, width=640, height=480, format=RGB ! "
      //"videoconvert ! "
      "tee name=split ! "
      "queue ! "
      "videoconvert ! "
      "autovideosink sync=false split. ! "
      "queue ! "
      "videoconvert ! "
      "appsink ");
  if (!cap.isOpened()) {
    printf("=ERR= can't create video capture\n");
    return -1;
  }

  // second part of sender pipeline
  //    writer.open("appsrc caps=video/x-raw, framerate=40/1, width=640,
  //    height=480, format=RGB ! "
  //		"videoconvert ! "
  //		"autovideosink sync=false"
  //                , 0, (double)30, Size(640, 480), true);
  //    if (!writer.isOpened()) {
  //        printf("=ERR= can't create video writer\n");
  //        return -1;
  //    }

  cap >> src;

  /// Create a matrix of the same type and size as src (for dst)
  dst.create(src.size(), src.type());

  /// Convert the image to grayscale
  cvtColor(src, src_gray, CV_BGR2GRAY);

  /// Create a window
  namedWindow(window_name, CV_WINDOW_NORMAL);

  ////////////////FILTERS/////////////////
  //// Uncomment one of these to have it run! (Multiple filters wont work
  ///correctly... for now... ////
  //// Remember to save afterwards with CTRL-o, then Enter ////
  // canny();
  // h_circles();
   p_hog();
  // face_d();
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  /// Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
}
