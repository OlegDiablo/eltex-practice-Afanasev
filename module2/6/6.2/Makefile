CC = gcc
CFLAGS = -Wall -Werror -Wextra
PICFLAGS = -fPIC

all: contacts_book

contacts_book: main.o libcontacts.dll
	$(CC) main.o -L. -lcontacts -Wl,-rpath,. -o contacts_book

libcontacts.dll : contacts.o DL_List.o
	$(CC) -shared -o contacts.dll contacts.o DL_List.o \
	    -Wl,--out-implib,libcontacts.dll.a

main.o: main.c DL_List.h contacts.h
	$(CC) $(CFLAGS) -c main.c

DL_List.o: DL_List.c DL_List.h contacts.h
	$(CC) $(CFLAGS) $(PICFLAGS) -c DL_List.c

contacts.o: contacts.c contacts.h
	$(CC) $(CFLAGS) $(PICFLAGS) -c contacts.c

clean:
	rm -f *.o *.so contacts_book