# Compiler name
CC= gcc

# Flags
DFLAGS=-g -lpthread

# Linking flag
LINK=-I.

# Program name
PROG=server.out

# Sources
SRC= crip.c server.c
SRCOBJS=$(SRC:.c=.o)

.SUFFIXES:.c .o

all:$(PROG)

$(PROG):$(SRCOBJS)
	$(CC) $(DFLAGS) -o $@ $(SRCOBJS) $(LINK)

.c.o:
	$(CC) $(DFLAGS) -c  $< $(LINK)


# TEST
test: clean debug_compile

debug_compile: OPT = -DTEST
debug_compile: $(PROG)

# CLEAN
clean: 
	$(RM) *.o *~ $(PROG)
