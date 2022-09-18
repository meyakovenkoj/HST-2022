CC=gcc
CFLAGS=-std=c99 -O3 -Wall -Werror -Xclang -fopenmp
LDFLAGS=-lm -lomp
TOPDIR=.
SRCDIR=$(TOPDIR)/src

all: gen.bin plain.bin test.bin

gen.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(SRCDIR)/generator/main.c $(SRCDIR)/file/interface.c $(SRCDIR)/file/file_binary.c $(SRCDIR)/file/file.c $(LDFLAGS) -o gen.bin

plain.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(SRCDIR)/main/main.c $(SRCDIR)/plain/plain.c $(SRCDIR)/file/interface.c $(SRCDIR)/file/file_binary.c $(SRCDIR)/file/file.c $(LDFLAGS) -o plain.bin

test.bin:
	$(CC) $(CFLAGS) -DBINARY_ONLY -I$(TOPDIR)/include $(TOPDIR)/tests/main.c $(SRCDIR)/plain/plain.c $(SRCDIR)/file/interface.c $(SRCDIR)/file/file_binary.c $(LDFLAGS) -o test.bin

check: all
	time $(TOPDIR)/gen.bin -s 100 -l 100
	time $(TOPDIR)/test.bin 26214400 100 gen_0100mb_1.txt gen_0100mb_2.txt
	ls -lah $(TOPDIR)/result.txt

clean:
	rm -rf *.bin *.txt *.dSYM