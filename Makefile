GCC := gcc

TEST := $(wildcard test/*.c)

runtest: build
	./build/test

test: build

build: $(TEST)
	mkdir -p build
	${GCC} -std=c99 test/main.test.c src/mqttCodec.c -o build/test

clean:
	rm -r build