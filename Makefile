CXX = g++
CXXFLAGS = -W -g

mytest: mytest.cpp satnet.cpp 
	$(CXX) $(CXXFLAGS) mytest.cpp satnet.cpp -o mytest

run:
	./mytest