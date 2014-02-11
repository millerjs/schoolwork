OBJS = main.o floyd.o stopwatch.o util.o
DEPS = $(OBJS:.o=.d)
CC = gcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 -MMD -MP -DDEBUG -O3 -Werror
BIN = ../floyd
LDLIBS = -lrt -pthread

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $(BIN)

%.d: %.c

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	-rm -f $(OBJS) $(BIN) *.d
