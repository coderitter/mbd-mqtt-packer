CC := gcc

SRC := $(wildcard src/*.c)
TEST := test/main.test.c
OBJ := $(SRC:src/%.c=build/obj/%.o)

.PHONY: runtest
runtest: build/test
	./build/test

build/test: $(SRC) $(TEST) build/libmbd-mqtt-packer.a
	mkdir -p build
	${CC} -std=c99 $(TEST) -o build/test \
	-Iinclude \
	-lmbd-mqtt-packer -Lbuild

build/libmbd-mqtt-packer.a: $(OBJ)
	mkdir -p build
	ar rcs $@ $(OBJ)

build/obj/%.o: src/%.c
	mkdir -p build/obj
	$(CC) -c $< -o $@ -g -Wall -Iinclude

.PHONY: clean
clean:
	rm -r -f build
