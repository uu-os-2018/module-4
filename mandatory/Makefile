# Change to y to enable debugging support
DEBUG:=

CC=gcc
OS := $(shell uname)

CFLAGS=-std=c99 -D_XOPEN_SOURCE=600 -Wall -Wextra

ifeq ($(DEBUG), y)
	CFLAGS += -g
	LDFLAGS += -g
else
	CFLAGS += -O2
	LDFLAGS += -O2
endif

ifeq ($(OS), Linux)
	CFLAGS += -pthread
	LDLIBS += -pthread -lrt
endif

all: $(addprefix bin/, mutex rendezvous bounded_buffer)

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

bin/%: obj/%.o
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

bin/mutex: obj/mutex.o obj/timing.o
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

clean:
	$(RM) obj/*.o bin/*

.PHONY: all clean
