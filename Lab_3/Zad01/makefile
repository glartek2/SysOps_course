CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =

all: main main2

main: main.o
	$(CC) $(LDFLAGS) -o $@ $^

main2: main2.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o main main2
