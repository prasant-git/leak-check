CC=gcc
INC=./inc
OBJ=./obj
CFLAGS=-I $(INC) -c -Wall -Werror

all: leak_check_data_structure.o leak_check.o

leak_check_data_structure.o: leak_check_data_structure.c $(INC)/leak_check_data_structure.h
	$(CC) $(CFLAGS) leak_check_data_structure.c
leak_check.o: leak_check.c $(INC)/leak_check.h
	$(CC) $(CFLAGS) leak_check.c

install:
	cp *.o $(OBJ)
clean:
	rm *.o

