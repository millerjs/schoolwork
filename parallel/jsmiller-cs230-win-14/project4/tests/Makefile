UTILS = ../../utils
SRC = ../src
DEPS = $(OBJS:.o=.d)
CC = colorgcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99\
	 -MMD -MP -DDEBUG -O3 -std=gnu99 $(DEBUG) 
BIN = test
LDLIBS = -lm -lrt -pthread 
DEPS = $(OBJS:.o=.d)

OBJS = test.o\
	 $(SRC)/hasht.o\
	 $(SRC)/hasht_locking.o\
	 $(SRC)/hasht_lockfreec.o\
	 $(SRC)/hasht_linear.o\
	 $(SRC)/util.o\
	 $(SRC)/cll.o\
	 $(SRC)/thread_pool.o\
	 $(UTILS)/crc32.o\
	 $(UTILS)/generators.o\
	 $(UTILS)/hashgenerator.o\
	 $(UTILS)/stopwatch.o\
	 $(UTILS)/fingerprint.o\
	 $(UTILS)/packetsource.o


all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o ../$(BIN)

%.d: %.c

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) $(BIN) *.d
	rm ../$(BIN)