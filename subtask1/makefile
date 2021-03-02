myapp.o:anglecorrection.cpp
	g++ anglecorrection.cpp -o myapp.o -std=c++11 `pkg-config --cflags --libs opencv`
.PHONY clean:
	rm -f *.o
	rm -f *_*.jpg
runempty:   #These were just for accebility while coding
	./myapp.o empty.jpg
runtraffic:
	./myapp.o traffic.jpg
