DEPS = $(OBJS:.o=.d)
CC = gcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 \
	 -MMD -MP -DDEBUG -O3  $(DEBUG)
LDLIBS = -lm -lrt -pthread  -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE

##### Objects

UTILS = ../../hash_utils
SRC = ../
BIN = ../libexperiments.a

OBJS =  \
	 experiment1.o\
	 experiment2.o\
	 experiment3.o\
	 experiment4.o\
	 experiment5.o\

all: $(BIN)

$(BIN): $(OBJS)
	ar  rcs $(BIN) $(OBJS)

%.d: %.c

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(BIN) *.d *.o
