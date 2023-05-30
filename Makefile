GCC := gcc

SRC := $(wildcard src/*.c)
TEST := $(wildcard test/*.c)
OBJ := $(SRC:src/%.c=build/obj/%.o)

build/libs/libmbd-mqtt-packer.a: $(OBJ)
	mkdir -p build/libs
	ar rcs $@ $(OBJ)

build/obj/%.o: src/%.c
	mkdir -p build/obj
	$(GCC) -c $< -o $@ -g -Wall -Iinclude -Ilibs/mbd-mqtt-packer/include

runtest: ./build/test
	./build/test

.PHONY: test
test: ./build/test

.PHONY: build
build: ./build/test

./build/test: $(SRC) $(TEST) 
	mkdir -p build
	${GCC} -std=c99 test/main.test.c build/libmbd-mqtt-client.a -o build/test \
	-Iinclude
.PHONY: clean
clean:
	rm -r build
