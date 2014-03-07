DEPS = $(OBJS:.o=.d)
CC = g++
CFLAGS = -g3 -Wall -fpic -MMD -MP -DDEBUG -D_DEBUG -O3 -Werror
LDLIBS = -lm -lrt -pthread  -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE

UTILS = libutilcpp

##### Objects
OBJS = $(UTILS)/matrix.o
eigP = eigP
svdP = svdP

all: $(eigP) $(svdP)

$(eigP): $(OBJS) $(eigP).o
	$(CC) $(LDFLAGS) $(OBJS) $(eigP).o $(LDLIBS) -o ../$(eigP)

$(svdP): $(OBJS) $(svdP).o
	$(CC) $(LDFLAGS) $(OBJS) $(svdP).o $(LDLIBS) -o ../$(svdP)

%.d: %.cpp

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) ../$(eigP) ../$(svdP) sample *.d
