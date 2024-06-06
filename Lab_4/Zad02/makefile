CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

TARGET = main

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f $(TARGET)