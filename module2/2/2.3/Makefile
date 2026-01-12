CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: ext_calc

ext_calc: ext_calc.o
	$(CC) ext_calc.o -o ext_calc

ext_calc.o: ext_calc.c
	$(CC) $(CFLAGS) -c ext_calc.c

clean:
	rm -f *.o ext_calc ext_calc.exe