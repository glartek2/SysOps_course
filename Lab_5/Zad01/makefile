CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = main
SRC = main.c

.PHONY: clear all

all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
