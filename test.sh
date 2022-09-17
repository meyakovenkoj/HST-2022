#!/bin/sh

./gen.bin -s 100 -l 100
./gen.bin -s 200 -l 200
./gen.bin -s 400 -l 400
./gen.bin -s 800 -l 800
./gen.bin -s 1600 -l 1600


./test.bin 1 26214400 100 gen_0100mb_1.txt gen_0100mb_2.txt
./test.bin 1 52428800 200 gen_0200mb_1.txt gen_0200mb_2.txt
./test.bin 1 104857600 400 gen_0400mb_1.txt gen_0400mb_2.txt
./test.bin 1 209715200 800 gen_0800mb_1.txt gen_0800mb_2.txt
./test.bin 1 419430400 1600 gen_1600mb_1.txt gen_1600mb_2.txt
