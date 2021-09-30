# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a
TARGET	 = demo
SRC		 = detached_thread_demo.cpp

.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread

.PHONY: clean
clean:
	rm -f $(TARGET)
