UTILS = ../hash_utils
SRC = ../src
DEPS = $(OBJS:.o=.d)
CC = gcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99\
	 -MMD -MP -DDEBUG  -std=gnu99 $(DEBUG)\
	 -O3

BIN = test
LDLIBS = -lm -lrt -pthread 
DEPS = $(OBJS:.o=.d)

OBJS = test.o\
	 $(SRC)/libhasht/libhasht.a\
	 $(SRC)/util.o\
	 $(SRC)/cll.o\
	 $(SRC)/thread_pool.o\

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o ../$(BIN)

%.d: %.c

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) $(BIN) *.d
	rm ../$(BIN)