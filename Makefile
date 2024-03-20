build:
	g++ -Wall -std=c++11 -o project2.out ./src/blend.cpp ./src/main.cpp ./src/rgb.cpp ./src/streamutils.cpp ./src/tga.cpp

.PHONY:
	clean

clean:
	rm output/*.tga project2.out