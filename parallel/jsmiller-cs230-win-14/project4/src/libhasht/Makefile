DEPS = $(OBJS:.o=.d)
CC = colorgcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 \
	 -MMD -MP -DDEBUG -O3  $(DEBUG)
LDLIBS = -lm -lrt -pthread  -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE

##### Objects

BIN = libhasht.a

OBJS =  \
	 hasht.o\
	 hasht_locking.o\
	 hasht_lockfreec.o\
	 hasht_linear.o\
	 hasht_awesome.o\

all: $(BIN)

$(BIN): $(OBJS)
	ar  rcs $(BIN) $(OBJS)

%.d: %.c

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(BIN) *.d *.o
