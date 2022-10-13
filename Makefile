CC=mpicc
CFLAGS=-std=c99 -O2 -Wall -Werror
LDFLAGS=-lm
TOPDIR=.
SRCDIR=$(TOPDIR)/src
INCLUDE= $(SRCDIR)/include/file.h $(SRCDIR)/include/input.h $(SRCDIR)/include/plain.h
BASE_C= $(SRCDIR)/file/interface.c $(SRCDIR)/file/file_binary.c $(SRCDIR)/file/file.c 
MAIN_C= $(SRCDIR)/main/main.c $(SRCDIR)/plain/plain.c $(BASE_C)
TEST_C= $(TOPDIR)/tests/main.c $(SRCDIR)/plain/plain.c $(BASE_C)
GEN_C= $(SRCDIR)/generator/main.c $(BASE_C)


all: gen.bin plain.bin test.bin

gen.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(GEN_C) $(LDFLAGS) -o gen.bin

plain.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(MAIN_C) $(LDFLAGS) -o plain.bin

test.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(TEST_C) $(LDFLAGS) -o test.bin

check: all
	time $(TOPDIR)/gen.bin -s 100 -l 100
	time $(TOPDIR)/test.bin 26214400 100 gen_0100mb_1.txt gen_0100mb_2.txt
	ls -lah $(TOPDIR)/result.txt

clean:
	rm -rf *.bin *.txt *.dSYM