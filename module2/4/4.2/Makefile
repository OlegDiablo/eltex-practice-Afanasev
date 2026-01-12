CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: queue

queue: queue.o
	$(CC) queue.o -o queue

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

clean:
	rm -f *.o queue queue.exe