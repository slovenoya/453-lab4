all : libDisk.o tinyFS.o demo.o tinyFSErr.o
	gcc libDisk.o tinyFS.o demo.o tinyFSErr.o -o tinyFSDemo
	rm *.o

test : libDisk.o tinyFS.o test.o tinyFSErr.o
	gcc libDisk.o tinyFS.o test.o tinyFSErr.o -o tinyFSTest
	rm *.o

dbg : libDisk.c tinyFS.c test.c
	gcc -g libDisk.c tinyFS.c test.c tinyFSErr.c -o debug

demo.o : demo.c
	gcc -std=gnu99 -Wall -pedantic -c demo.c

test.o : test.c
	gcc -std=gnu99 -Wall -pedantic -c test.c

tinyFS.o : tinyFS.h
	gcc -std=gnu99 -Wall -pedantic -c tinyFS.c

libDisk.o : libDisk.h
	gcc -std=gnu99 -Wall -pedantic -c libDisk.c

tinyFSErr.o : tinyFSErr.h
	gcc -std=gnu99 -Wall -pedantic -c tinyFSErr.c

clean: 
	rm tinyFSDemo tinyFSTest debug