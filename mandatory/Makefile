# Change to y to enable debugging support
DEBUG:=

CC=gcc
OS := $(shell uname)

CFLAGS=-std=c99 -D_XOPEN_SOURCE=600 -Wall -Wextra -I semaphores

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

all: $(addprefix bin/, mutex rendezvous bounded_buffer semaphores_test)

semaphores/semaphores.o:
	cd semaphores; make

obj/%.o: src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

bin/semaphores_test: obj/semaphores_test.o semaphores/semaphores.o
	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@


bin/%: obj/%.o
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

bin/mutex: obj/mutex.o obj/timing.o
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

clean:
	$(RM) obj/*.o bin/*
	cd semaphores; make clean

.PHONY: all clean
