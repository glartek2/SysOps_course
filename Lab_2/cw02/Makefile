#CXX = gcc
#CFLAGS = -g -Wall -Wextra -std=c11

.PHONY: all clean

all:
#utworzenie biblioteki z statycznym rozszerzeniem .a
	gcc -c bibl1.c
	ar rcs libbibl1.a bibl1.o
#utworzenie biblioteki dynamicznej z rozszerzeniem .so
	gcc -fPIC -c bibl1.c
	gcc -shared bibl1.o -o libbibl1.so
#utworzenie programów
	gcc main.c libbibl1.a -o main_stat
	gcc main.c -L. -lbibl1 -W -o main_wspoldz #dodatkowa ścieżka do bibl1
	gcc main2.c -o main_ladow_dynam -ldl


clean:
	rm -f all