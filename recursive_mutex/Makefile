# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -fsanitize=thread
TARGET	 = demo
SRC		 = recursive_mutex_demo.cpp
SRC_FIXED = recursive_mutex_demo_fixed.cpp
TARGET_FIXED = demof
SRC_FIB = recursive_mutex_fib.cpp
TARGET_FIB = mutex_fib


.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread 
	$(CXX) $(CXXFLAGS) $(SRC_FIXED) -o $(TARGET_FIXED) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
	$(CXX) $(CXXFLAGS) $(SRC_FIB) -o $(TARGET_FIB) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
.PHONY: clean
clean:
	rm -f $(TARGET)
