CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: chmod

chmod: chmod.o
	$(CC) chmod.o -o chmod

chmod.o: chmod.c
	$(CC) $(CFLAGS) -c chmod.c

clean:
	rm -f *.o chmod chmod.exe