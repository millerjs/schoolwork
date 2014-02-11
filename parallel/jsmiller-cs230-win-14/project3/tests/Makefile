UTILS = ../../utils
SRC = ../src
OBJS = test.o $(SRC)/locks.o $(SRC)/util.o $(SRC)/threads.o $(UTILS)/generators.o $(UTILS)/stopwatch.o $(UTILS)/fingerprint.o $(UTILS)/packetsource.o $(UTILS)/crc32.o
DEPS = $(OBJS:.o=.d)
CC = gcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 -MMD -MP -DDEBUG -O3 -std=c99
DFLAGS = 
BIN = test
LDLIBS = -lm -lrt -pthread 

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(DFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o ../$(BIN)

%.d: %.c

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) $(BIN) *.d
	rm ../$(BIN)