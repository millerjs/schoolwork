UTILS = ../../utils
SRC = ../src
OBJS =   $(SRC)/firewall.o  $(SRC)/q.o  $(SRC)/threads.o $(UTILS)/generators.o $(UTILS)/stopwatch.o $(UTILS)/fingerprint.o $(UTILS)/packetsource.o $(UTILS)/crc32.o tests.o
DEPS = $(OBJS:.o=.d)
CC = gcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 -MMD -MP -DDEBUG -O3 -std=c99 -Werror
DFLAGS = 
BIN = test
LDLIBS = -lm -lrt -pthread 

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(DFLAGS) $(LDLIBS) -o ../$(BIN)

%.d: %.c

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) $(BIN) *.d
	rm ../$(BIN)
