GCC := gcc

SRC := $(wildcard src/*.c)
TEST := $(wildcard test/*.c)

runtest: ./build/test
	./build/test

.PHONY: test
test: ./build/test

.PHONY: build
build: ./build/test

./build/test: $(SRC) $(TEST) 
	mkdir -p build
	${GCC} -std=c99 test/main.test.c src/pack.c src/unpack.c -o build/test \
	-Iinclude

clean:
	rm -r build
