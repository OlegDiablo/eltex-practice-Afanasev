CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: main

main: 
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

clean:
	rm -f server client