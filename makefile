all:
	g++ anglecorrection.cpp -o myapp -std=c++11 `pkg-config --cflags --libs opencv`
.PHONY clean:
	rm myapp
	rm traffic_*.jpg
	rm empty_*.jpg
runempty:
	./myapp empty.jpg
runtraffic:
	./myapp traffic.jpg
