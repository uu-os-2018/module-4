DEBUG=y

CC=gcc
OS := $(shell uname)

CFLAGS=-std=gnu99 -Werror -Wall
LDFLAGS=

ifeq ($DEBUG), y)
	CFLAGS += -DDEBUG -g
else
	CFLAGS += -O2
endif

ifeq ($(OS), Linux)
	CFLAGS += -pthread
	LDLIBS += -pthread -lrt
endif


# http://stackoverflow.com/a/2394668/3834375

SOURCES := $(wildcard src/*.c)
TARGETS := $(patsubst src/%.c, bin/%, $(SOURCES))

all: $(TARGETS)

bin/%: src/%.c
	$(CC) $(CFLAGS) $(LDLIBS) $< -o $@

clean:
	$(RM) *~ src/*~ src/#* obj/*.o bin/*

#
.PHONY: all clean dist
