CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: sniffer

chmod: main
	$(CC) $(CFLAGS) sniffer.c -o sniffer

clean:
	rm -f sniffer