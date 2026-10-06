#include <assert.h>
#include <inttypes.h>

#include "heapsort.h"

void HeapSort(minHeap* heap)
{
    assert(heap != NULL);

    uint_least16_t n = heap->size;

    if(n > 1U)
    {
        HeapBuild(heap);
        int_least32_t i;
        for (i = (int_least32_t)(heap->size) - 1; i >= 0; i--)
        {
            int_least32_t temp = heap->data[0];
            heap->data[0] = heap->data[i];
            heap->data[i] = temp;
            n--;
            HeapMinHeapify(heap, 0U, n);
        }
    }
}
