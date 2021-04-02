all:ref.o m1.o m2.o m3.o m4.o

ref.o:reference.cpp
	g++ reference.cpp -o ref.o -std=c++11 `pkg-config --cflags --libs opencv`
m1.o:method1.cpp
	g++ method1.cpp -o m1.o -std=c++11 `pkg-config --cflags --libs opencv`
m2.o:method2.cpp
	g++ method2.cpp -o m2.o -std=c++11 `pkg-config --cflags --libs opencv`
m3.o:method3.cpp
	g++ method3.cpp -o m3.o -pthread -std=c++11 `pkg-config --cflags --libs opencv`
m4.o:method4.cpp
	g++ method4.cpp -o m4.o -pthread -std=c++11 `pkg-config --cflags --libs opencv`
.PHONY: clean
clean:
	rm -f *.o
	rm -f *.txt

