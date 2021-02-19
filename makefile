all:
	g++ anglecorrection.cpp -o myapp -std=c++11 `pkg-config --cflags --libs opencv`
clean:
	rm myapp
runempty:
	./myapp empty.jpg
runtraffic:
	./myapp traffic.jpg
