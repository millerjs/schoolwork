DEPS = $(OBJS:.o=.d)
CC = g++
CFLAGS = -g3 -Wall -fpic -MMD -MP -DDEBUG -D_DEBUG -O3 -Werror
LDLIBS = -lm -lrt -pthread  -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE

UTILS = libutilcpp

##### Objects
eigP = ../eigP
OBJS = eigP.o\
	 $(UTILS)/matrix.o

all: $(eigP)

$(eigP): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)  $(LDLIBS) -o $(eigP)

%.d: %.cpp

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) $(BIN) sample *.d
