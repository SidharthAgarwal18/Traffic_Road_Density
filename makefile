a.o: density.cpp
	g++ density.cpp -o a.o -std=c++11 `pkg-config --cflags --libs opencv`
.PHONY: clean
clean:
	rm -f *.o
	rm -f out.txt
