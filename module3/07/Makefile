CC = gcc
CFLAGS = -Wall -Wextra -Werror -lrt

all: main

main: 
	$(CC) $(CFLAGS) ping.c -o ping
	$(CC) $(CFLAGS) pong.c -o pong

clean:
	rm -f ping pong