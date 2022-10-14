CC=mpicc
NVCC=nvcc
CFLAGS=-std=c99 -O2 -Wall -Werror
CUFLAGS=
LDFLAGS=-lm
TOPDIR=.
SRCDIR=$(TOPDIR)/src
INCLUDE= $(SRCDIR)/include/file.h $(SRCDIR)/include/input.h $(SRCDIR)/include/plain.h
BASE_C= $(SRCDIR)/file/interface.c $(SRCDIR)/file/file_binary.c $(SRCDIR)/file/file.c 
MAIN_C= $(SRCDIR)/main/main.c $(SRCDIR)/plain/plain.c $(BASE_C)
TEST_C= $(TOPDIR)/tests/main.c $(SRCDIR)/plain/plain.c $(BASE_C)
GEN_C= $(SRCDIR)/generator/main.c $(BASE_C)
CUDA_C= $(SRCDIR)/cuda/main.c $(SRCDIR)/cuda/process.cu $(BASE_C)


all: gen.bin plain.bin test.bin

gen.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(GEN_C) $(LDFLAGS) -o gen.bin

plain.bin:
	$(CC) $(CFLAGS) -I$(TOPDIR)/include $(MAIN_C) $(LDFLAGS) -o plain.bin

interface.o: $(SRCDIR)/file/interface.c
	$(CC) $(CFLAGS) -I$(TOPDIR)/include -o $@ -c $<

file_binary.o: $(SRCDIR)/file/file_binary.c
	$(CC) $(CFLAGS) -I$(TOPDIR)/include -o $@ -c $<

file.o: $(SRCDIR)/file/file.c
	$(CC) $(CFLAGS) -I$(TOPDIR)/include -o $@ -c $<

test.o: $(TOPDIR)/tests/main.c
	$(CC) $(CFLAGS) -I$(TOPDIR)/include -o $@ -c $<

plain.o: $(SRCDIR)/plain/plain.c
	$(CC) $(CFLAGS) -I$(TOPDIR)/include -o $@ -c $<

cuda.o: $(SRCDIR)/cuda/process.cu
	$(NVCC) $(CUFLAGS) -c $<

test.bin: interface.o file.o file_binary.o test.o plain.o
	$(CC) -I$(TOPDIR)/include $^ $(LDFLAGS) -o test.bin

cuda.bin:
	nvcc -I$(TOPDIR)/include $(CUDA_C) $(LDFLAGS) -o cuda.bin

check: all
	time $(TOPDIR)/gen.bin -s 100 -l 100
	time $(TOPDIR)/test.bin -s 26214400 -l 100 gen_0100mb_1.txt gen_0100mb_2.txt
	ls -lah $(TOPDIR)/result.txt

clean:
	rm -rf *.bin *.txt *.dSYM *.o