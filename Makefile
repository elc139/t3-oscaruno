CXX = clang++ 
CXXFLAGS = -Wall -std=c++11 -fopenmp
LDFLAGS = -std=c++11 -pthread
TARGET = ThreadDemoABC OpenMPDemoABC OpenMPDemoABC2

all: $(TARGET)


ThreadDemoABC: ThreadDemoABC.cpp
	$(CXX) $(LDFLAGS) -o $@  $^
OpenMPDemoABC2: OpenMPDemoABC2.cpp
	$(CXX) $(CXXFLAGS) -o $@  $^
OpenMPDemoABC: OpenMPDemoABC.cpp
	$(CXX) $(CXXFLAGS) -o $@  $^

clean:
	rm -f *.o *~ $(TARGET)