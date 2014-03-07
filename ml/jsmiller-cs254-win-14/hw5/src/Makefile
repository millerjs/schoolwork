DEPS = $(OBJS:.o=.d)
CC = g++
CFLAGS = -g3 -Wall -fpic -std=gnu99 -MMD -MP -DDEBUG -D_DEBUG -O3 -Werror
LDLIBS = -lm -lrt -pthread  -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE

UTILS = libutilcpp

##### Objects
BIN = ../hw5
OBJS = main.o\
	 $(UTILS)/matrix.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)  $(LDLIBS) -o $(BIN)

%.d: %.cpp

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) $(BIN) sample *.d
