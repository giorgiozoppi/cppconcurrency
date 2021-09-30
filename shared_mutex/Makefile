# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -fsanitize=thread
TARGET	 = demo
SRC		 = shared_mutex_demo.cpp


.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread 
.PHONY: clean
clean:
	rm -f $(TARGET)
