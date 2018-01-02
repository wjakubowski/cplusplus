########################
ClassName=Uklad
########################


CXX=c++
CXXFLAGS= -g -Wall -std=c++11

main : main.o $(ClassName).o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(ClassName).o :   $(ClassName).cpp
	$(CXX) $(CXXFLAGS) $< -c

main.o : main.cpp
	$(CXX) $(CXXFLAGS) $< -c

clean:
	rm -rf *.o main

run: main
	./main

valgrind: main
	valgrind -v ./main

gdb : main
	gdb ./main 
