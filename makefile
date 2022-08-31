CC=g++
compile:
	mkdir build
	${CC} rain.cpp -lX11 -o build/rain
