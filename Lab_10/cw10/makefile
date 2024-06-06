TARGET = main

CC = gcc
CFLAGS = -pthread -Wall -Wextra -O2

SRC = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: clean
