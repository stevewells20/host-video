#include "GstStreamCapper.h"
#include "GstHoughCircle.h"
#include "GstFaceDetect.h"
#include "GstEdgeDetect.h"
#include "GstColorCorrect.h"
#include "GstLaplaceEdgeDetect.h"
#include "GstTest.h"
#include "GstBRISKDetect.h"

#include <time.h>

// #include "GstAbsFilter.h"


int main(int argc, char **argv) {

	cout << getBuildInformation() << endl << endl;

  GstStreamCapper gstr(5000,false);
	// gstr.showLive = false;

	// gstr.resetCap();

	// GstHoughCircle fCircle;
	// GstFaceDetect fFace;
  // GstEdgeDetect fEdge;	fEdge.apply_contuors = false; fEdge.apply_hull = false; fEdge.apply_lines = false;
	// GstColorCorrect fColor;
	// GstLaplaceEdgeDetect fLapEdge;
	// GstBRISKFeatures fBRISK;
	// GstBRISKDetect fBRISK;
	// GstTest fTest;

	// gstr.addFilter(&fCircle);
	// gstr.addFilter(&fEdge);
	// gstr.addFilter(&fFace);
	// gstr.addFilter(&fColor);
	// gstr.addFilter(&fLapEdge);
	// gstr.addFilter(&fBRISK);
	// gstr.addFilter(&fTest);

	gstr.run();
	// gstr.removeFilter(&fColor);
	// gstr.removeFilter(&fLapEdge);
	// gstr.removeFilter(&fEdge);
	// gstr.resetCap();
	// gstr.run();

  cout << "Done!" << endl;
  return 0;
}
