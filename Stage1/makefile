# Kamil Kasza, 18738- Operating Systems Project 1
# CSC1021/simpleshell

CC=gcc
CFLAGS= -Wall
SRC=./src
OUTDIR=./bin

simpleshell: $(SRC)/simpleshell.o $(SRC)/utility.o manual
	$(CC) -o $(OUTDIR)/simpleshell $(SRC)/simpleshell.o $(SRC)/utility.o

%.o: %.c utility.h simpleshell.c utility.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTDIR)/man:
	@mkdir $(OUTDIR)/man

manual: $(OUTDIR)/man
	@cp -r ./manual/* $(OUTDIR)/man

clean:
	rm -r $(OUTDIR)/man
	rm *.o

