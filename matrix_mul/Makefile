CXX		 = /usr/bin/g++-11
# Matrix Multiply Challenge Makefile
CXXFLAGS = -Wall -std=c++2a -Wextra  
TARGET	 = matrix_multiply_challenge
SRC		 = matrix_multiply_challenge.cpp


.PHONY: all
all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) -lpthread
	 
.PHONY: clean

clean:
	rm -f $(TARGET)
