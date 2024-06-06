CXX = gcc
CFLAGS = -g -Wall -Wextra -std=c11

TARGET = countdown

.PHONY: all clean

all: $(TARGET)

$(TARGET): countdown.c
	$(CXX) $(CFLAGS) -o $(TARGET) countdown.c

clean:
	rm -f $(TARGET)
