all: trace_clean

trace_clean: trace_clean.o
	g++ -std=c++11 -o trace_clean trace_clean.o 

trace_clean.o: trace_clean.cpp
	g++ -std=c++11 -c trace_clean.cpp

clean:
	rm -rf *.o trace_clean

