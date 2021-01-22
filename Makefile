CFLAGS=-O2 -I /usr/local
CC=gcc

TARGETS=sender receiver basic
UTILS=util.o 

all: $(TARGETS)

$(UTILS): %.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

%.o: %.c util.h
	$(CC) $(CFLAGS)  -c $< 


$(TARGETS): %:%.o util.o
	$(CC) $(CFLAGS) $^ -o $@ -lrt

.PHONY: clean

clean:
	rm -rf *.o $(UTILS) $(TARGETS) 
