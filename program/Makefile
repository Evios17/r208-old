CC=g++

all: main

main: main.c++
	$(CC) $^ -o main -I pgnp/src -I pgnp/libs/chess-move-interface/src -L pgnp/build -lpgnp

run:
	LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:pgnp/build ./main

pgnp: download
	mkdir -p pgnp/build
	cd pgnp/build && cmake ../ && make

download:
	rm -rf pgnp
	git clone --recursive https://github.com/manzerbredes/pgnp.git

clean:
	rm -f main

mrproper: clean
	rm -rf pgnp

.PHONY: download run clean mrproper