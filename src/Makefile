CC=gcc
CFLAGS=-g -Wall -ansi -pedantic -O3
EXEC=jeu
FILES=main.o piece.o partie.o

all: $(FILES)
	$(CC) $(FILES) -o ../$(EXEC) $(CFLAGS)

piece.o: piece.c
	$(CC) -c piece.c $(CFLAGS)

partie.o: partie.c
	$(CC) -c partie.c $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)
