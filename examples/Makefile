DEBUG=n

CC := gcc
OS := $(shell uname)

CFLAGS=-std=gnu99 -Werror -Wall -O2
LDFLAGS=

ifeq ($(DEBUG), y	)
	CFLAGS += -DDEBUG -g
endif

ifeq ($(OS), Linux)
	XCFLAGS := -pthread
	XLDLIBS := -lrt
endif


PTHREADS := balance hello_pthreads pthreads_and_fork pthreads_create_exit_null_join pthreads_unsynchronized_concurrency 
NORMAL	 := callbacks timer
UCONTEXT := contexts

PTHREAD_TARGETS  := $(addprefix bin/, $(PTHREADS))
NORMAL_TARGETS   := $(addprefix bin/, $(NORMAL))
UCONTEXT_TARGETS := $(addprefix bin/, $(UCONTEXT))

# Target specific variables

$(PTHREAD_TARGETS): CFLAGS += $(XCFLAGS)
$(PTHREAD_TARGETS): LDLIBS += $(XLDLIBS)

# On Mac OS (aka OS X) when compiling with gcc (clang) the -Wno-deprecated-declarations
# flag must also be used to suppress compiler warnings.
$(UCONTEXT_TARGETS): CFLAGS += -Wno-deprecated-declarations

# On Mac OS (aka OS X) optimizations of loops with no side effects will give
# strange results, see  https://stackoverflow.com/questions/48835657/why-does-empty-loop-cause-bus-error-when-compiling-c-program-with-clang-o2-on?noredirect=1#comment84676117_48835657
NO_OPT_TARGETS := $(UCONTEXT_TARGETS)  bin/timer
$(NO_OPT_TARGETS): CFLAGS := $(filter-out -O2, $(CFLAGS))

.PHONY: all clean 

all: $(NORMAL_TARGETS) $(PTHREAD_TARGETS) $(UCONTEXT_TARGETS)

bin/%: src/%.c
	$(CC) $(CFLAGS) $(LDLIBS) $< -o $@

clean:
	$(RM) *~ src/*~ src/#* obj/*.o bin/*
	$(RM) -rf bin/*.dSYM

