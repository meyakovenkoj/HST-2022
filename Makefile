CC=mpicc
NVCC=nvcc
CFLAGS=-std=c99 -g -O2 -Wall -Werror
CUFLAGS=-O2 -g --use_fast_math -m64
GENCODE_FLAGS=-gencode arch=compute_35,code=sm_35 -gencode arch=compute_37,code=sm_37 -gencode arch=compute_50,code=sm_50 -gencode arch=compute_52,code=sm_52 -gencode arch=compute_60,code=sm_60 -gencode arch=compute_61,code=sm_61 -gencode arch=compute_70,code=sm_70 -gencode arch=compute_75,code=sm_75 -gencode arch=compute_80,code=sm_80 -gencode arch=compute_86,code=sm_86 -gencode arch=compute_86,code=compute_86
LDFLAGS=-lm
CUDALDFLAGS=-L/usr/local/cuda/lib64 -lcudart
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
	$(NVCC) $(CUFLAGS) -I$(TOPDIR)/include $(GENCODE_FLAGS) -o $@ -c $<

test.bin: interface.o file.o file_binary.o test.o plain.o cuda.o
	$(CC) -I$(TOPDIR)/include $^ $(LDFLAGS) $(CUDALDFLAGS) -o test.bin

cuda.bin:
	$(NVCC) -I$(TOPDIR)/include $(CUDA_C) $(GENCODE_FLAGS) $(LDFLAGS) -o $@

check: all
	time $(TOPDIR)/gen.bin -s 100 -l 100
	time $(TOPDIR)/test.bin -s 26214400 -l 100 gen_0100mb_1.txt gen_0100mb_2.txt
	ls -lah $(TOPDIR)/result.txt

clean:
	rm -rf *.bin *.txt *.dSYM *.o
