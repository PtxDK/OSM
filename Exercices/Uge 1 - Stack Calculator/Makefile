.PHONY: clean # 'clean' er ikke en fil, men bare en kommando.

# Flag til oversætteren for at give flere advarsler.
CFLAGS=-Wall -Wextra -pedantic
CFLAGS+=-g # Vi vil også gerne kunne køre gdb.

# For at oversætte calc-programmet, afhænger vi af calc.c og stack.o.
calc: calc.c stack.o
	# Byg calc.  '-o calc' gør at den bliver oversæt til 'calc'.
	gcc $(CFLAGS) -o calc calc.c stack.o

# ... men byg først stack.o, dvs. objektfilen for stack.c.  Den oversættes
# separat fra calc.
stack.o: stack.c stack.h
	# Flaget '-c' sørger for at lave en objektfil.
	gcc $(CFLAGS) -c stack.c

clean:
	rm stack.o calc # Fjern de genererede filer.
