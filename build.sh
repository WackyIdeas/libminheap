#!/bin/bash
gcc -std=c23 -pedantic -Wall -c minheap.c
ar -rs libminheap.a minheap.o

gcc -std=c23 -pedantic -Wall -static main.c heapsort.c -L. -lminheap -o bin_gcc.out
