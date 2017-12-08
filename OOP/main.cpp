<<<<<<< HEAD
#include "GstStreamCapper.h"
// #include "GstHoughCircle.h"
// #include "GstFaceDetect.h"
#include <time.h>

// #include "GstAbsFilter.h"

=======
// #include "GstStreamCapper.h"
#include "GstHoughCircle.h"
#include "GstFaceDetect.h"
#include <time.h>

>>>>>>> 1e5d002238e70f6c0d0e20f573d38a49519d8911
int main(int argc, char **argv) {

  GstStreamCapper gstr(5000);

<<<<<<< HEAD
	GstAbsFilter f_test;

	gstr.addFilter(&f_test);
	gstr.run();

	// GstHoughCircle hough;
	// hough.capper = &gstr;
	// hough.filterRun();
=======
	GstHoughCircle hough;
	hough.capper = &gstr;
	hough.filterRun();
>>>>>>> 1e5d002238e70f6c0d0e20f573d38a49519d8911
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
	getchar();
  cout << "Done!" << endl;
  return 0;
}
