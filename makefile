CXX=g++
CXXFLAGS = -Wall -g -std=c++11
OBJS = y86dis.o
DIR = ./Test/

# use this make target to build your own main.cpp for testing
y86dis $(FUNCTION):	$(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o y86dis	

		 

y86dis.o:   Tools.h y86dis.h

clean:
			rm -f y86dis.o y86distest y86dis
        