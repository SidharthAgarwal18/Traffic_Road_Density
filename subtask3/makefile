a0.o: reference.cpp
	g++ reference.cpp -o a0.o -std=c++11 `pkg-config --cflags --libs opencv`
a1.o: method1.cpp
	g++ method1.cpp -o a1.o -std=c++11 `pkg-config --cflags --libs opencv`
a2.o: method2.cpp
	g++ method2.cpp -o a2.o -std=c++11 `pkg-config --cflags --libs opencv`
a3.o: method3.2.cpp
	g++ method3.2.cpp -o a3.o -std=c++11 -pthread `pkg-config --cflags --libs opencv`
a4.o: method4.3.cpp
	g++ method4.3.cpp -o a4.o -std=c++11 -pthread `pkg-config --cflags --libs opencv`
ut.o: utility.cpp
	g++ utility.cpp -o ut.o
run0: 
	./a0.o ./empty.jpg ./trafficvideo.mp4
run1: 
	./a1.o ./empty.jpg ./trafficvideo.mp4 5000
run2:
	./a2.o ./empty.jpg ./trafficvideo.mp4 480 240
run3: 
	./a3.o ./empty.jpg ./trafficvideo.mp4 3
run4: 
	./a4.o ./empty.jpg ./trafficvideo.mp4 8
runu:
	./ut.o ./out3.txt
.PHONY: clean
clean:
	rm -f *.o
#	rm -f out.txt
