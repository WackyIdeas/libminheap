#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "minheap.h"

/*
 * HeapSort izvršava heapsort algoritam nad
 * prosleđenom Min-Heap strukturom.
 * Direktno modifikuje niz heap->data tako
 * da su elementi sortirani u nerastućem
 * redosledu.
 */
void HeapSort(minHeap* heap);

#endif
