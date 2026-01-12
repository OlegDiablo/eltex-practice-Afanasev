CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: ip_stat

ip_stat: ip_stat.o
	$(CC) ip_stat.o -o ip_stat

ip_stat.o: ip_stat.c
	$(CC) $(CFLAGS) -c ip_stat.c

clean:
	rm -f *.o ip_stat ip_stat.exe