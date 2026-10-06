#!/bin/bash
clang -std=c23 -pedantic -Wall -c minheap.c
ar -rs libminheap.a minheap.o

clang -std=c23 -pedantic -Wall -static main.c heapsort.c -L. -lminheap -o bin_clang.out
