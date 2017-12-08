#include "GstStreamCapper.h"
#include "GstHoughCircle.h"
// #include "GstFaceDetect.h"
#include <time.h>

// #include "GstAbsFilter.h"


int main(int argc, char **argv) {

  GstStreamCapper gstr(5000);

	GstHoughCircle f_test;

	gstr.addFilter(&f_test);
	gstr.run();

	// GstHoughCircle hough;
	// hough.capper = &gstr;
	// hough.filterRun();

	// GstFaceDetect face;
	// face.capper = &gstr;
	// face.filterRun();

	// while (true) {
	// 	gstr.cap >> gstr.src;
	// 	imshow("test",gstr.src);
	// 	int key;
  //   key = waitKey(30);
	// 	}
	// getchar();
	// gstr.resetCap();
  cout << "Done!" << endl;
  return 0;
}
