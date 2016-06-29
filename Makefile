NAME = lab1

CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -I src

SOURCES := $(shell find src -name '*.c')
OBJECTS := $(patsubst src/%.c,build/obj/%.o,$(SOURCES))
DEPENDENCIES := $(patsubst src/%.c,build/deps/%.d,$(SOURCES))

.PHONY: all clean clean-build build run

all: build

clean: clean-build
	-$(RM) -r bin

clean-build:
	-$(RM) -r build

run: build
	bin/$(NAME)

build: bin/$(NAME)

-include $(DEPENDENCIES)

bin/$(NAME): $(OBJECTS)
	@mkdir -p bin
	$(CC) -o bin/$(NAME) $(OBJECTS)

build/obj/%.o: src/%.c
	@mkdir -p build/deps build/obj
	$(CC) $(CFLAGS) -MMD -MP -MF build/deps/$*.d -o $@ -c $<
