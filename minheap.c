#include "minheap.h"

#pragma CHECK_MISRA("none")
#include <assert.h>
#include <string.h>
#pragma CHECK_MISRA("all")

/*
 * Pomoćna swap funkcija gde su a i b indeksi elemenata
 * niza heap->data.
 */
inline static void swap(minHeap* heap, uint_least16_t a, uint_least16_t b);

void HeapInit(minHeap* heap, const int_least32_t* d, uint_least16_t n)
{
    assert(heap != NULL);
    assert(n <= HEAP_MAXSIZE);

    heap->size = n;
    /*
     * Ako je d == NULL, inicijalizuj sa nulama.
     */
    if(!d)
    {
        memset(heap->data, 0, (size_t)n * sizeof(int_least32_t));
    }
    else
    {
        /*
         * U suprotnom, kopiraj sadržaj niza d u heap->data, i
         * preuredi niz.
         */
        memcpy(heap->data, d, (size_t)n * sizeof(int_least32_t));
        HeapBuild(heap);
    }
}
void HeapBuild(minHeap* heap)
{
    assert(heap != NULL);
    assert(heap->size <= HEAP_MAXSIZE);

    const uint_least16_t n = heap->size;
    if(n > 0U)
    {
        const int_least32_t midpoint = (((int_least32_t)(n) / 2) - 1);
        int_least32_t i;
        for (i = midpoint; i >= 0; i--)
        {
            HeapMinHeapify(heap, (uint_least16_t)i, n);
        }
    }
}
int_least32_t HeapGetMinimum(const minHeap* heap)
{
    assert(heap != NULL);

    return (heap->size == 0U) ? INT_LEAST32_MAX : heap->data[0];
}
void HeapMinHeapify(minHeap* heap, uint_least16_t index, uint_least16_t n)
{
    assert(heap != NULL);
    assert(heap->size <= HEAP_MAXSIZE);

    /*
     * Iterativna verzija MinHeapify algoritma. Uslov za
     * while petlju su tu za samu proveru validnosti
     * ulaznih argumenata. Ukoliko ulazni argumenti nisu
     * važeći, ova funkcija će se na "tih" način
     * neuspešno završiti.
     */
    while ((n <= heap->size) && (index < heap->size))
    {
        uint_least16_t left = HEAP_LEFT(index);
        uint_least16_t right = HEAP_RIGHT(index);
        uint_least16_t smallest = index;

        if ((left < n) && (heap->data[left] < heap->data[smallest]))
        {
            smallest = left;
        }

        if ((right < n) && (heap->data[right] < heap->data[smallest]))
        {
            smallest = right;
        }

        if (smallest != index)
        {
            swap(heap, smallest, index);
            index = smallest;
        }
        else
        {
            break;
        }
    }
}

int HeapAdd(minHeap* heap, int_least32_t value)
{
    assert(heap != NULL);

    int result = 0;

    if((heap->size + 1U) <= HEAP_MAXSIZE)
    {
        heap->size++;
        int_least32_t index = (int_least32_t)(heap->size) - 1;
        heap->data[index] = value;

        while ((index > 0) && (heap->data[HEAP_PARENT(index)] > heap->data[index]))
        {
            int_least32_t parent = HEAP_PARENT(index);
            swap(heap, (uint_least16_t)index, (uint_least16_t)parent);
            index = parent;
        }
        result = 1;
    }

    return result;
}
int_least32_t HeapFind(const minHeap* heap, int_least32_t value)
{
    assert(heap != NULL);
    assert(heap->size <= HEAP_MAXSIZE);

    int_least32_t result = -1;
    uint_least16_t i;

    for (i = 0U; i < heap->size; i++)
    {
        if (heap->data[i] == value)
        {
            result = (int_least32_t)i;
            break;
        }
    }

    return result;
}
int HeapDelete(minHeap* heap, int_least32_t value)
{
    int_least32_t index = HeapFind(heap, value);

    /*
     * Koristimo promenljivu result kako bi se
     * zadovoljilo MISRA C pravilo da funkcija
     * sme da sadrži samo jednu tačku povratka.
     */
    int_least32_t result = 0;
    if((index >= 0) && ((uint_least16_t)index < heap->size))
    {
        heap->data[index] = heap->data[heap->size - 1U];
        heap->size--;

        const uint_least16_t n = heap->size;
        HeapMinHeapify(heap, (uint_least16_t)index, n);
        result = 1;
    }
    return result;
}

inline static void swap(minHeap* heap, uint_least16_t a, uint_least16_t b)
{
    int_least32_t temp = heap->data[a];
    heap->data[a] = heap->data[b];
    heap->data[b] = temp;
}

#ifndef NDEBUG
void HeapPrint(const minHeap* heap)
{
    assert(heap != NULL);
    assert(heap->size <= HEAP_MAXSIZE);

    printf("Size: %"PRIuLEAST16"\nHeap: ", heap->size);

    uint_least16_t i;
    for (i = 0U; i < heap->size; i++)
    {
        printf("%"PRIdLEAST32" ", heap->data[i]);
    }
    printf("\n");
}
#endif
