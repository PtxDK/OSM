.PHONY: all clean valgrind quickgrind debug

CC=gcc
CFLAGS=-Werror -Wextra -pedantic -std=c11 -g

all: bouncer

queue: queue.c
	$(CC) $(CFLAGS) -o queue queue.c

bouncer: bouncer.c queue.o
	$(CC) $(CFLAGS) -o bouncer bouncer.c queue.o

test: test.c
	$(CC) $(CFLAGS) -o test queue.o test.c

clean:
	rm -f *.o
	rm -f bouncer
	rm -f test

valgrind: bouncer
	valgrind --track-origins=yes --leak-check=full ./bouncer

quickgrind: bouncer
	cat input.txt | valgrind --track-origins=yes --leak-check=full ./bouncer

debug: input.txt gdb.script bouncer
	gdb -q -x gdb.script ./bouncer
