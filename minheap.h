#ifndef HEAP_H
#define HEAP_H

#pragma CHECK_MISRA("none")
#include <inttypes.h>

#ifndef NDEBUG
#include <stdio.h>
#endif
#pragma CHECK_MISRA("all")

#ifndef HEAP_MAXSIZE
#define HEAP_MAXSIZE 256U
#endif

/*
 * Pomoćne makro funkcije koje računaju
 * indeks roditelja, levog i desnog deteta
 * za zadati indeks i.
 */
#define HEAP_PARENT(i) (((i)-1) / 2)
#define HEAP_LEFT(i) ((2U*(i)) + 1U)
#define HEAP_RIGHT(i) ((2U*(i)) + 2U)

/*
 * Struktura koja sadrži niz koji predstavlja
 * sam Min-Heap i njenu veličinu. Gornja
 * granica za veličinu je HEAP_MAXSIZE.
 */
struct mHeap
{
    int_least32_t data[HEAP_MAXSIZE];
    uint_least16_t size;
};

typedef struct mHeap minHeap;

/*
 * HeapInit se koristi za inicijalizaciju minHeap
 * strukture. Prosleđuju se dva argumenta:
 * - Pokazivač na niz brojeva koji će se kopirati
 *   u heap->data. Ukoliko se ovde prosledi NULL,
 *   prvih n elemenata niza heap->data se
 *   postavljaju na nulu.
 *
 * - Veličina pokazivača d. Ukoliko d nije NULL,
 *   gornja granica za n je veličina tog niza.
 *   Ako je pokazivač d NULL, onda se n tretira
 *   kao veličina niza heap->data. U oba slučaja,
 *   gornja granica za n je HEAP_MAXSIZE.
 *
 * Nakon inicijalizacije, ova funkcija poziva
 * HeapBuild funkciju koja preuređuje elemente
 * niza da poštuju svojstvo Min-Heap strukture.
 *
 * Treba voditi računa da n ne prekorači granice
 * prosleđenog niza d ukoliko nije NULL, da bi se
 * izbeglo čitanje van granica niza.
 *
 * Kako ova implementacija ne koristi dinamički
 * alociranu memoriju, nema potrebe za odgovarajućom
 * funkcijom za oslobađanje strukture.
 */
void HeapInit(minHeap* heap, const int_least32_t* d, uint_least16_t n);

/*
 * HeapGetMinimum vraća prvu vrednost niza heap->data.
 * Podrazumeva se da je heap pravilno inicijalizovan i
 * pravilno formiran Min-Heap. Ukoliko je heap prazan,
 * vraća se INT_LEAST32_MAX.
 */
int_least32_t HeapGetMinimum(const minHeap* heap);

/*
 * HeapFind vraća indeks prvog elementa čija vrednost je jednaka
 * prosleđenom argumentu value. Ukoliko takav element ne
 * postoji, funkcija vraća -1.
 */
int_least32_t HeapFind(const minHeap* heap, int_least32_t value);

/*
 * HeapMinHeapify vrši MinHeapify algoritam nad nizom heap->data
 * za prosleđen indeks i veličinu niza n.
 */
void HeapMinHeapify(minHeap* heap, uint_least16_t index, uint_least16_t n);

/*
 * HeapBuild preuređuje niz heap->data tako da bude pravilno
 * formiran Min-Heap.
 */
void HeapBuild(minHeap* heap);

/*
 * HeapAdd dodaje element value u strukturu tako da se održava
 * svojstvo Min-Heap strukture. Funkcija vraća 1 ukoliko je
 * unos uspešan, i vraća 0 za neuspešan unos.
 */
int HeapAdd(minHeap* heap, int_least32_t value);

/*
 * HeapDelete briše prvi element koji ima vrednost jednaka
 * prosleđenom argumentu value, tako da se održava svojstvo
 * Min-Heap strukture. Funkcija vraća 1 za uspešno brisanje,
 * u suprotnom vraća 0.
 */
int HeapDelete(minHeap* heap, int_least32_t value);

/*
 * Ukoiko radimo u Debug okruženju, implementira se i
 * funkcija HeapPrint za ispis sadržaja i veličine
 * heap strukture na standardni izlaz.
 */
#ifndef NDEBUG
void HeapPrint(const minHeap* heap);
#endif

#endif
