#include "GstStreamCapper.h"
#include "GstHoughCircle.h"
#include "GstFaceDetect.h"
#include "GstEdgeDetect.h"
#include <time.h>

// #include "GstAbsFilter.h"


int main(int argc, char **argv) {

  GstStreamCapper gstr(5000);

	GstHoughCircle fCircle;
	// GstFaceDetect fFace;
  GstEdgeDetect fEdge;

	// gstr.addFilter(&fFace);
  gstr.addFilter(&fCircle);
  // gstr.addFilter(&fEdge);

	gstr.run();

	// getchar();
	// gstr.resetCap();
  cout << "Done!" << endl;
  return 0;
}
