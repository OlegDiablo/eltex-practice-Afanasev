CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: calc

calc: calc.o
	$(CC) calc.o -o calc

calc.o: calc.c
	$(CC) $(CFLAGS) -c calc.c

clean:
	rm -f *.o calc calc.exe