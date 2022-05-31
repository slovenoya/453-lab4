all : libDisk.o tinyFS.o demo.o
	gcc libDisk.o tinyFS.o demo.o -o tinyFSDemo

test : libDisk.o tinyFS.o test.o
	gcc libDisk.o tinyFS.o test.o -o tinyFSTest

demo.o : demo.c
	gcc -std=gnu99 -Wall -pedantic -c demo.c

test.o : test.c
	gcc -std=gnu99 -Wall -pedantic -c test.c

tinyFS.o : tinyFS.h
	gcc -std=gnu99 -Wall -pedantic -c tinyFS.c

libDisk.o : libDisk.h
	gcc -std=gnu99 -Wall -pedantic -c libDisk.c

clean :
	rm *.o