# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -fsanitize=thread
TARGET	 = async_1
SRC		 = async_1.cpp
TARGET_1	 = async_2
SRC_1		 = async_2.cpp
TARGET_2	 = async_3
SRC_2		 = async_3.cpp


.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
	$(CXX) $(CXXFLAGS) $(SRC_1) -o $(TARGET_1) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
	$(CXX) $(CXXFLAGS) $(SRC_2) -o $(TARGET_2) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread

.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET_1) $(TARGET_2) $(TARGET_3)
