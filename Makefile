build:
	g++ -Wall -O0 -std=c++11 -o project2.out ./src/*.cpp

.PHONY:
	clean

clean:
	rm output/*.tga project2.out