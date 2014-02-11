OBJS =  tests.o ../src/floyd.o ../src/util.o 
# DEPS = $(OBJS:.o=.d)
CC = gcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 -MMD -MP -DDEBUG -O3
BIN = ../test
LDLIBS = -pthread

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $(BIN)

%.d: %.c

clean:
	-rm -f $(OBJS) $(BIN) *.d
