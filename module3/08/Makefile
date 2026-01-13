CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: main

main: 
	$(CC) $(CFLAGS) producer.c -o producer
	$(CC) $(CFLAGS) customer.c -o customer

clean:
	find . -mindepth 1 \( -name 'Makefile' -o -name 'customer.c' -o -name 'producer.c' \) -prune -o -exec rm -rf {} +