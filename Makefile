CC := gcc
CFLAGS := -g -Wall -Wextra

SRC := $(wildcard src/*.c)
INCLUDE := $(wildcard include/*.h)
TEST := test/main.test.c
OBJ := $(SRC:src/%.c=build/obj/%.o)

.PHONY: runtest
runtest: build/test
	./build/test

build/test: $(SRC) $(INCLUDE) $(TEST) build/libmbd-mqtt-packer.a
	mkdir -p build
	$(CC) -std=c99 $(TEST) -o build/test \
	$(CFLAGS) \
	-Iinclude \
	-lmbd-mqtt-packer -Lbuild

build/libmbd-mqtt-packer.a: $(OBJ)
	mkdir -p build
	ar rcs $@ $(OBJ)

build/obj/%.o: src/%.c
	mkdir -p build/obj
	$(CC) -c $< -o $@ $(CFLAGS) -Iinclude

.PHONY: clean
clean:
	rm -r -f build
