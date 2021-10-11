# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -fsanitize=thread
TARGET	 = deadlock
SRC		 = deadlock_1.cc
TARGET_FIXED	 = deadlock_fixed
SRC_FIXED		 = deadlock_1-fixed.cc
TARGET_1	 = deadlock_2
SRC_1		 = deadlock_2.cc
TARGET_FIXED_1	 = deadlock_2_fixed
SRC_FIXED_1		 = deadlock_2-fixed.cc

.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
	$(CXX) $(CXXFLAGS) $(SRC_1) -o $(TARGET_1) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
	$(CXX) $(CXXFLAGS) $(SRC_FIXED) -o $(TARGET_FIXED) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread 
	$(CXX) $(CXXFLAGS) $(SRC_FIXED_1) -o $(TARGET_FIXED_1) -I/usr/local/include -L/usr/local/include -lfmtd -lpthread
	
.PHONY: clean
clean:
	rm -f $(TARGET) $(TARGET_FIXED)
