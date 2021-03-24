a0.o: reference.cpp
	g++ reference.cpp -o a0.o -std=c++11 `pkg-config --cflags --libs opencv`
a4.o: method4.cpp
	g++ method4.cpp -o a4.o -std=c++11 -lpthread `pkg-config --cflags --libs opencv`
run0: 
	./a0.o ./empty.jpg ./trafficvideo.mp4
run4: 
	./a4.o ./empty.jpg ./trafficvideo.mp4
.PHONY: clean
clean:
	rm -f *.o
#	rm -f out.txt
