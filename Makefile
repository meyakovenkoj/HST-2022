CC=gcc
CFLAGS=-std=c99 -g -Wall -Werror
LDFLAGS=-lm
TOPDIR=.
SRCDIR=$(TOPDIR)/src

all: gen.bin plain.bin test.bin

gen.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(SRCDIR)/generator/main.c $(SRCDIR)/file/interface.c $(SRCDIR)/file/file_binary.c $(SRCDIR)/file/file.c $(LDFLAGS) -o gen.bin

plain.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(SRCDIR)/main/main.c $(SRCDIR)/plain/plain.c $(SRCDIR)/file/interface.c $(SRCDIR)/file/file_binary.c $(SRCDIR)/file/file.c $(LDFLAGS) -o plain.bin

test.bin:
	$(CC) $(CFLAGS) -DBINARY_ONLY -I$(TOPDIR)/include $(TOPDIR)/tests/main.c $(SRCDIR)/plain/plain.c $(SRCDIR)/file/interface.c $(SRCDIR)/file/file_binary.c $(LDFLAGS) -o test.bin

clean:
	rm -rf *.bin *.txt *.dSYM