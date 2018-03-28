
# This defines our compiler and linker, as we've seen before.
CXX = g++
PACKAGES = "opencv gstreamer-1.0"
OPTIMIZE = -fopenmp -D_GLIBCXX_PARALLEL -O3 -Ofast
CXXFLAGS = -march=native -std=c++11 
LIBS = `pkg-config --cflags --libs $(PACKAGES)`

# % = main
# $@ = main
# $< = main.cpp
% : %.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(OPTIMIZE) $(LIBS)


