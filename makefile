all:
	g++ density.cpp -std=c++11 `pkg-config --cflags --libs opencv`
