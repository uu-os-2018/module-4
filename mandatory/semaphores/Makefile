TARGETS  := semaphores.o
CFLAGS   := -Wall -std=c99
LDLIBS   :=
PLATFORM := $(shell uname -s)
PREFIX   := UNDEFINED

ifeq ($(PLATFORM), Darwin)
	PREFIX := apple
endif

ifeq ($(PLATFORM), Linux)
	PREFIX := linux
	LDLIBS += -pthread
endif

SEMAPHORE := $(PREFIX)_semaphores

.PHONY: clean

all: $(TARGETS)

semaphores.o: $(SEMAPHORE).c
	gcc $(CFLAGS) $(LDLIBS) -c $< -o $@

%.o:%.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(TARGETS)
