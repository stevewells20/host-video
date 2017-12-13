#include "GstStreamCapper.h"
#include "GstHoughCircle.h"
#include "GstFaceDetect.h"
#include "GstEdgeDetect.h"
#include "GstColorCorrect.h"
#include <time.h>

// #include "GstAbsFilter.h"


int main(int argc, char **argv) {

  GstStreamCapper gstr(5000);
	gstr.showLive = true;
	gstr.resetCap();

	// GstHoughCircle fCircle;
	// GstFaceDetect fFace;
  GstEdgeDetect fEdge;
	// GstColorCorrect fColor;

	// gstr.addFilter(&fFace);
  // gstr.addFilter(&fCircle);
  gstr.addFilter(&fEdge);
	// gstr.addFilter(&fColor);

	gstr.run();

	// getchar();
	// gstr.resetCap();
  cout << "Done!" << endl;
  return 0;
}
