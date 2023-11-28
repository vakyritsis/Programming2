CC = gcc
CCFLAGS = -Wall -g
OBJ =  project3.o objdb.o

project3: $(OBJ)
	$(CC) $(OBJ) -o project3

project3.o: project3.c objdb.h
	$(CC) $(CCFLAGS) -c project3.c

objdb.o: objdb.c objdb.h
	$(CC) $(CCFLAGS) -c objdb.c

.PHONY: clean
clean:
	rm -f *.o
