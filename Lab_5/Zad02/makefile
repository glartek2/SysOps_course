CC = gcc
CFLAGS = -Wall -Wextra -g

# Lista plików źródłowych
SRCS = sender.c catcher.c hope.c
# Lista plików obiektowych
OBJS = $(SRCS:.c=.o)
# Nazwa programu sender
SENDER = sender
# Nazwa programu catcher
CATCHER = catcher

.PHONY: all clean

# Domyślny cel
all: $(SENDER) $(CATCHER)

# Reguła kompilacji plików obiektowych
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Kompilacja programu sender
$(SENDER): sender.o hope.o
	$(CC) $(CFLAGS) $^ -o $@

# Kompilacja programu catcher
$(CATCHER): catcher.o hope.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(SENDER) $(CATCHER)
