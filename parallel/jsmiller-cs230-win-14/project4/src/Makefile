UTILS = ../../utils
DEPS = $(OBJS:.o=.d)
CC = colorgcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 -MMD -MP -DDEBUG -D_DEBUG -O3 -Werror
BIN = project4
LDLIBS = -lm -lrt -pthread  -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE

##### Objects

OBJS = main.o\
	 util.o\
	 thread_pool.o\
	 $(UTILS)/generators.o\
	 $(UTILS)/stopwatch.o\
	 $(UTILS)/fingerprint.o\
	 $(UTILS)/packetsource.o\
	 $(UTILS)/crc32.o


all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)  $(LDLIBS) -o ../$(BIN)

%.d: %.c

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) $(BIN) *.d
	rm ../$(BIN)