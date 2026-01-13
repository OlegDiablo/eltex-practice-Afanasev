CC = gcc
CFLAGS = -Wall -Werror -Wextra -c

all: contacts_book

contacts_book: main.o libcontacts.a
	$(CC) main.o -L. -lcontacts -o contacts_book	

libcontacts.a: contacts.o DL_List.o
	ar rcs libcontacts.a contacts.o DL_List.o

main.o: main.c DL_List.h contacts.h 
	$(CC) $(CFLAGS) main.c -o main.o

DL_List.o: DL_List.c DL_List.h contacts.h
	$(CC) $(CFLAGS) DL_List.c -o DL_List.o

contacts.o: contacts.c contacts.h
	$(CC) $(CFLAGS) contacts.c -o contacts.o

clean:
	rm -f *.o *.a contacts_book