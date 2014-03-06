UTILS = ../../utils
OBJS = vector.o matrix.o hw3.o 
DEPS = $(OBJS:.o=.d)
BIN = hw3

DFLAGS =

CC = g++

CFLAGS = -I../../include -g3 -Wall -fpic  -MMD -MP -DDEBUG -O3 -std=c++0x
LDLIBS = -lm -lrt -pthread 

all: hw3 

hw3: $(OBJS) 
	$(CC) $(DFLAGS) $(OBJS) $(LDLIBS) -o $(BIN)

%.d: %.cpp

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) $(BIN) *.d
