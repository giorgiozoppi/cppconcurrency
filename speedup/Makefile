# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -Wextra  
TARGET	 = demo
SRC		 = measure_speedup_demo.cpp

.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lpthread
	 
.PHONY: clean
clean:
	rm -f $(TARGET)
