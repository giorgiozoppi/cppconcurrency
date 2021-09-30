# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -fsanitize=thread
TARGET	 = demo
SRC		 = try_lock_demo.cpp
SRC_FIB = try_lock_restroom.cpp
TARGET_FIB = demor


.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread 
	$(CXX) $(CXXFLAGS) $(SRC_FIB) -o $(TARGET_FIB) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
.PHONY: clean
clean:
	rm -f $(TARGET)
