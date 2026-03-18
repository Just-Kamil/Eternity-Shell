# Kamil Kasza, 18738- Operating Systems Project 1
# CSC1021/simpleshell

CC=gcc
CFLAGS= -Wall
OUTDIR=../bin

simpleshell: simpleshell.o utility.o manual
	$(CC) -o $(OUTDIR)/simpleshell simpleshell.o utility.o

%.o: %.c utility.h simpleshell.c utility.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTDIR)/man:
	@mkdir $(OUTDIR)/man

manual: $(OUTDIR)/man
	@cp -r ../manual/* $(OUTDIR)/man

clean:
	rm -r $(OUTDIR)/man
	rm *.o

