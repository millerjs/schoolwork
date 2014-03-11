UTILS = ../hash_utils
DEPS = $(OBJS:.o=.d)
CC = gcc
CFLAGS = -I../../include -g3 -Wall -fpic -std=gnu99 \
	 -MMD -MP -DDEBUG -O3  $(DEBUG) -Werror
BIN = project4
LDLIBS = -lm -L./libhasht -L../hash_utils -lhash_utils -lrt -pthread \
	  -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE 

##### Objects

OBJS = \
	 main.o\
	 hasht_handler.o\
         libexperiments.a\
         libhasht/libhasht.a\
	 thread_pool.o\
	 $(UTILS)/hashgenerator.o\
	 cll.o\
	 util.o\

all: $(BIN)

$(BIN): $(OBJS)
	$(MAKE) -C experiments/
	$(CC) $(LDFLAGS) $(OBJS)  $(LDLIBS) -o ../$(BIN)

%.d: %.c

libexperiments.a:
	$(MAKE) -C experiments/

libhasht/libhasht.a:
	$(MAKE) -C libhasht/

$(UTILS)/libhash_utils.a:
	$(MAKE) -C $(UTILS)/	

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) ../$(BIN) *.d
	$(MAKE) clean -C experiments/
	$(MAKE) clean -C libhasht/ 