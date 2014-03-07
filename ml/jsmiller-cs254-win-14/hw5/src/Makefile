DEPS = $(OBJS:.o=.d)
CC = g++
CFLAGS = -g3 -Wall -fpic -MMD -MP -DDEBUG -D_DEBUG -O3 -Werror
LDLIBS = -lm -lrt -pthread  -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE

UTILS = libutilcpp

##### Objects
OBJS = $(UTILS)/matrix.o
eigP = eigP
svdP = svdP
genA = genA

all: $(eigP) $(svdP) $(genA)

$(eigP): $(OBJS) $(eigP).o
	$(CC) $(LDFLAGS) $(OBJS) $(eigP).o $(LDLIBS) -o ../$(eigP)

$(svdP): $(OBJS) $(svdP).o
	$(CC) $(LDFLAGS) $(OBJS) $(svdP).o $(LDLIBS) -o ../$(svdP)

$(genA): $(OBJS) $(genA).o
	$(CC) $(LDFLAGS) $(OBJS) $(genA).o $(LDLIBS) -o ../$(genA)

%.d: %.cpp

check-syntax:
	$(CC) -o nul -S ${CHK_SOURCES}

clean:
	rm -f $(OBJS) ../$(eigP) ../$(svdP) sample *.d
