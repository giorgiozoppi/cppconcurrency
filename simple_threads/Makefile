# Thread vs. Process Demo Makefile
CXX		 = /usr/bin/g++-11
CXXFLAGS = -Wall -std=c++2a -fsanitize=thread
TARGET	 = thread-1 thread-2 thread-3 thread-4 thread-5 thread-6 thread-7 thread-8 thread-9-bug
SRC		 = thread-1.cc thread-2.cc thread-3.cc thread-4.cc thread-5.cc thread-6.cc thread-7.cc thread-8.cc thread-9-bug.cc
objects = thread-1.o thread-2.o thread-3.o thread-4.o thread-5.o thread-6.o thread-7.o thread-8.o thread-9-bug.o
.PHONY: all
all: $(objects)
	$(CXX) -c $(CXXFLAGS)  -I/usr/local/include -L/usr/local/include  $< 
	$(CXX) -o thread-1 thread-1.o -lfmtd -lpthread
	$(CXX) -o thread-2 thread-2.o -lfmtd -lpthread
	$(CXX) -o thread-3 thread-3.o -lfmtd -lpthread
	$(CXX) -o thread-4 thread-4.o -lfmtd -lpthread
	$(CXX) -o thread-5 thread-5.o -lfmtd -lpthread
	$(CXX) -o thread-6 thread-6.o -lfmtd -lpthread
	$(CXX) -o thread-7 thread-7.o -lfmtd -lpthread
	$(CXX) -o thread-8 thread-8.o -lfmtd -lpthread
	$(CXX) -o thread-9-bug thread-9-bug.o -lfmtd -lpthread
	@rm -fr *.o
$(objects):	%.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET)
