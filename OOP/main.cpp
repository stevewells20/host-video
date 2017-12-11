#include "GstStreamCapper.h"
#include "GstHoughCircle.h"
#include "GstFaceDetect.h"
#include <time.h>

// #include "GstAbsFilter.h"


int main(int argc, char **argv) {

  GstStreamCapper gstr(5000);

	GstHoughCircle fCircle;
	// GstFaceDetect fFace;

	gstr.addFilter(&fCircle);
	// gstr.addFilter(&fFace);

	gstr.run();

	// getchar();
	// gstr.resetCap();
  cout << "Done!" << endl;
  return 0;
}
