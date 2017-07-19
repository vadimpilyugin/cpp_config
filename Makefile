.PHONY: all
all: build/config

build/config.o: src/config.cpp include/config.h
	g++ -std=c++11 -o build/config.o -c -I include/ src/config.cpp
build/main.o: src/main.cpp include/config.h
	g++ -std=c++11 -o build/main.o -c -I include/ src/main.cpp

build/config: clear build/config.o build/main.o
	g++ -std=c++11 -o build/config -I include/ build/config.o build/main.o

.PHONY: clear
clear:
	clear; clear; mkdir -p build;
.PHONY: clean
clean:
	rm -rf build/
.PHONY: run
run: all
	./build/config

.PHONY: test
test: all
	./build/config
