
# This defines our compiler and linker, as we've seen before.
CXX = g++
# CXXFLAGS = -std=c++11 -c -O3 -Wall -Wextra -Werror -pedantic $(pkg-config --cflags opencv)
CXXFLAGS = -std=c++11 -O3 -Ofast `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
INCLUDE = -I.

# % = name of file to compile
# $@ =
# $< =
% : %.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(LIBS) $(INCLUDE)
