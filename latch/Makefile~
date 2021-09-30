# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -fsanitize=thread
TARGET	 = demo
SRC		 = barrier_demo.cpp
TARGET_FIXED	 = demof
SRC_FIXED		 = barrier_demo_fixed.cpp


.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
	$(CXX) $(CXXFLAGS) $(SRC_FIXED) -o $(TARGET_FIXED) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
	 
.PHONY: clean
clean:
	rm -f $(TARGET)
