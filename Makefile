export NAME = allocator-test

export CC = gcc
export STRIP = strip

export CFLAGS = -std=c11 -Wall -Wextra -I src
export RELEASE_CFLAGS = -O3
export DEBUG_CFLAGS = -g -DDEBUG

.PHONY: all release debug clean clean-build run run-debug

all: release debug

clean: clean-build
	-$(RM) -r bin

clean-build:
	-$(RM) -r build

run: bin/$(NAME)
	bin/$(NAME)

run-debug: build/debug/bin/$(NAME)
	build/debug/bin/$(NAME)

release: bin/$(NAME)

debug: build/debug/bin/$(NAME)

bin/$(NAME): build/release/bin/$(NAME)
	@mkdir -p bin
	cp build/release/bin/$(NAME) bin/$(NAME)
	$(STRIP) bin/$(NAME)

build/debug/bin/$(NAME): FORCE
	$(MAKE) -f Makefile.build MODE=debug ADDITIONAL_CFLAGS='$(DEBUG_CFLAGS)'

build/release/bin/$(NAME): FORCE
	$(MAKE) -f Makefile.build MODE=release ADDITIONAL_CFLAGS='$(RELEASE_CFLAGS)'

FORCE:
