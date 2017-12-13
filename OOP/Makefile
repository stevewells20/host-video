
# This defines our compiler and linker, as we've seen before.
CXX = g++
PACKAGES = opencv
OPTIMIZE = -fopenmp -D_GLIBCXX_PARALLEL -O3 -Ofast
# CXXFLAGS = -std=c++11 -c -O3 -Ofast -Wall -Wextra -Werror -pedantic $(pkg-config --cflags opencv)
CXXFLAGS = -std=c++11 $(OPTIMIZE) `pkg-config --cflags $(PACKAGES)`
LIBS = `pkg-config --libs $(PACKAGES)`
INCLUDE = -I.


# % = name of file to compile
# $@ =
# $< =
% : %.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(LIBS) $(INCLUDE)
