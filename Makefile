CC = gcc
CFLAGS = -Wall -Werror -g
LIBS = -lcurl -lxml2 -lpcre

all: crawler

crawler: crawler.o bag.o hashtable.o pagedir.o url.o curl.o set.o webpage.o
	$(CC) $(CFLAGS) -o crawler crawler.o bag.o hashtable.o pagedir.o url.o curl.o set.o webpage.o $(LIBS)

crawler.o: crawler.c crawler.h
	$(CC) $(CFLAGS) -c crawler.c

bag.o: bag.c bag.h
	$(CC) $(CFLAGS) -c bag.c

hashtable.o: hashtable.c hashtable.h set.h
	$(CC) $(CFLAGS) -c hashtable.c

pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c pagedir.c

url.o: url.c url.h
	$(CC) $(CFLAGS) -c url.c

curl.o: curl.c curl.h
	$(CC) $(CFLAGS) -c curl.c

set.o: set.c set.h
	$(CC) $(CFLAGS) -c set.c

webpage.o: webpage.c webpage.h
	$(CC) $(CFLAGS) -c webpage.c -lcurl

clean:
	rm -f crawler *.o
