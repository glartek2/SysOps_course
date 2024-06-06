CC = gcc
CFLAGS = -Wall

all: first_program second_program

first_program: first_program.c
	$(CC) $(CFLAGS) first_program.c -o first_program

second_program: second_program.c
	$(CC) $(CFLAGS) second_program.c -o second_program

clean:
	rm -f first_program second_program
