all : libDisk.o tinyFS.o demo.o tinyFSErr.o
	gcc libDisk.o tinyFS.o demo.o tinyFSErr.o -o tinyFSDemo
	rm *.o

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
	rm tinyFSDemo tinyFSDisk