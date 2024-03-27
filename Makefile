build:
	g++ -Wall -O0 -std=c++11 -o project2.out ./src/*.cpp

.PHONY:
	clean

tasks:
	./project2.out output/part11.tga input/circles.tga multiply input/layer1.tga
	./project2.out output/part12.tga input/layer1.tga flip
	./project2.out output/part13.tga input/layer1.tga subtract input/layer2.tga

task14:
	./project2.out output/part14.tga input/car.tga flip flip flip flip

clean:
	rm output/*.tga project2.out