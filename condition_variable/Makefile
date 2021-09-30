# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -fsanitize=thread -g -ggdb -O0
TARGET	 = demo
SRC		 = condition_variable_demo.cpp
SRC_FIXED = condition_variable_fixed.cpp
TARGET_FIXED = demof


.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread 
	$(CXX) $(CXXFLAGS) $(SRC_FIXED) -o $(TARGET_FIXED) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET_FIXED)
