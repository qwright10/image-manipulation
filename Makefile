build:
	g++ -Wall -std=c++11 -o project2.out ./src/*.cpp

.PHONY:
	clean

clean:
	rm output/*.tga project2.out