/*
 * 2025 Bogdan Cvetanovski Pašalić, RA 26/2022
 *
 * Test program za biblioteku libminheap.
 *
 * Biblioteka libminheap sadrži implementaciju Min-Heap strukture podataka, koja
 * omogućava sledeće funkcionalnosti:
 *
 * - Čitanje najmanjeg elementa iz hrpe
 * - Dodavanje i brisanje elemenata
 * - Inicijalizovanje hrpe koristeći postojeći
 *   niz podataka
 * - Ispis hrpe (sadržaj i veličinu)
 *
 * Sama struktura podataka je predstavljena kao C struktura sa statički alociranim
 * nizom i promenljivom koja saopštava veličinu hrpe.
 *
 * U zasebnom delu je implementiran Heapsort algoritam koristeći ovu biblioteku, koji
 * menja redosled elemenata u strukturi tako da su sortirani u nerastućem redosledu.
 *
 * Testirano na arhitekturama x86_64 (GCC 15.2.1, Clang 21.1.4) i armv7l (GCC 14.1.1).
 *
 * Kod je preveden na sledeći način:
 *
 * gcc -std=c23 -pedantic -Wall -c minheap.c
 * ar -rs libminheap.a minheap.o
 * gcc -std=c23 -pedantic -Wall -static main.c heapsort.c -L. -lminheap -o bin_gcc.out
 *
 * Kod je napisan tako da je što više usklađen sa MISRA C (2004) standardom.
 *
 * Napomene kod prevođenja:
 *
 * - Ukoliko je NDEBUG definisan pri prevođenju za biblioteku, funkcionalnost za ispis
 *   na standardni izlaz neće biti uključen u kod. Pošto glavni program koristi
 *   ispis na standardni izlaz kao vid provere, onda se NDEBUG ne definiše zbog
 *   testiranja.
 * - Maksimalni kapacitet svake Min-Heap instance se može definisati preko makro
 *   definicije HEAP_MAXSIZE. Uobičajena vrednost je postavljena na 256. Gledajući da
 *   je promenljiva za čuvanje kapaciteta u strukturi tipa uint_least16_t, gornja
 *   granica kapaciteta je 65535.
 *
 * Napomene vezane za dizajn odluke kao i usklađenost sa MISRA standardom:
 *
 * - Odabran tip podataka za elemente hrpe je int_least32_t, i podrazumevana
 *   veličina hrpe je 256. Kao rezultat, veličina jedne instance hrpe će najčešće
 *   biti oko 1028 bajtova, ili tek malo iznad 1KiB. Da su upotrebljene brze
 *   varijante ovih tipova, upotrebljena radna memorija bi bila preko dva puta
 *   veća, što često nije prihvatljivo na uređajima koji imaju jako limitiranu
 *   memoriju.
 *
 * - Funkcije koje vraćaju potvrdnu vrednost (HeapAdd, HeapDelete) imaju int kao
 *   povratnu vrednost u duhu ostalih funkcija iz C standardne biblioteke koje
 *   vraćaju "bool" vrednosti preko int tipa, uprkos MISRA preporuci da se
 *   izbegavaju takvi tipovi (MISRA-C:2004 6.3/A).
 *
 * - Koriste se makro funkcije za dobijanje roditeljskog indeksa, kao i indeks za
 *   levo i desno dete, jer je primena dosta limitirana i jednostavna u okviru ovog
 *   koda. Ovo ide protiv preporuke (MISRA-C:2004 19.7/A).
 *
 * - Nije se primenila preporuka za upotrebu skroz jedinstvenih identifikatora za
 *   promenljive (MISRA-C:2004 5.7/A) jer su značenja imena tih promenljivih
 *   dovoljno jasna i ne koriste se za različite kontekste, kao što se u dokumentu
 *   implicira kao razlog za izbegavanje te prakse. Npr. `index` se uvek odnosi na
 *   indekse elemenata niza u strukturi.
 *
 * - Moguće je izazvati buffer overflow koristeći HeapInit funkciju, ukoliko se
 *   parametar za veličinu niza hrpe postavi da bude veći od veličine prosleđenog
 *   niza odakle se podaci kopiraju (ukoliko prosleđen parametar nije NULL).
 *
 * Za test program:
 *
 * - Instance strukture za Min-Heap se inicijalizuju koristeći HeapInit funkciju,
 *   tako što se prosleđuje adresa određene instance, ali da bi kod pratio MISRA C
 *   standard (MISRA-C:2004 9.1/R), potrebno je inicijalizovati strukturu na drugi
 *   način pre pozivanja funkcije HeapInit.
 *
 *   Kako projekat koristi C23, moguće je ispoštovati ovo pravilo koristeći
 *   praznu inicijalizaciju (empty initializer-list) za strukture.
 *   (https://en.cppreference.com/w/c/language/struct_initialization.html)
 *
 * - Kako test program nije namenjen za produkciju, uključuje se standardna biblioteka
 *   za ulaz i izlaz što narušava (MISRA-C:2004 20.9/R).
 */

#pragma CHECK_MISRA("none")
#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

#include "minheap.h"
#include "heapsort.h"
#pragma CHECK_MISRA("all")

int main(void)
{
    printf("Test case #1: Initialization:\n\n");

    const int_least32_t test1[] = { 16, 4, 10, 14, 7, 9, 3, 2, 8, 1 };
    const int_least32_t test2[] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
    const int_least32_t test3[] = { 90, 87, 34, 66, 81, 24, 20, 13, 49, 66, 0 };

    /*
     * C23 podržava praznu inicijalizaciju struktura na
     * ovaj način.
     */
    minHeap testCase1 = {};
    minHeap testCase2 = {};
    minHeap testCase3 = {};

    /*
     * Slučaj kada je Min-Heap prazan.
     */
    minHeap emptyTestCase = {};

    HeapInit(&testCase1, test1, 10U);
    HeapInit(&testCase3, test3, 10U);
    /*
     * Inicijalizujemo prazan Min-Heap veličine 2.
     */
    HeapInit(&testCase2, (int_least32_t*)NULL, 2U);

    /*
     * Ručna inicijalizacija hrpe.
     */
    uint_least16_t i;
    for (i = 0U; i < 10U; i++)
    {
        HeapAdd(&testCase2, test2[i]);
    }

    printf("Size in bytes: %zu\n", sizeof(testCase1));

    HeapPrint(&testCase1);
    HeapPrint(&testCase2);
    HeapPrint(&testCase3);
    printf("\n");
    printf("---------------------------------\n");

    printf("Test case #2: Adding more values to a min-heap:\n\n");

    HeapAdd(&testCase2, 5);
    /*
     * Ubacivanje negativnih elemenata.
     */
    HeapAdd(&testCase1, -1);

    HeapPrint(&testCase1);
    HeapPrint(&testCase2);
    HeapPrint(&testCase3);
    printf("\n");
    printf("---------------------------------\n");

    printf("Test case #3: Deleting values from a min-heap:\n\n");

    HeapDelete(&testCase2, 0);
    HeapDelete(&testCase1, 10);

    /*
     * Ovo treba da baci grešku.
     */
    if (!HeapDelete(&testCase3, 67))
    {
        printf("L%d: HeapDelete: Couldn't find value %d!\n", __LINE__ - 2, 67);
    }

    printf("\n");
    HeapPrint(&testCase1);
    HeapPrint(&testCase2);
    HeapPrint(&testCase3);
    printf("\n");
    printf("---------------------------------\n");

    printf("Test case #4: Heapsort and Heapify:\n\n");

    HeapSort(&testCase1);
    printf("\nSorted testCase1:\n");
    HeapPrint(&testCase1);

    HeapBuild(&testCase1);
    printf("\nRebuilt testCase1:\n");
    HeapPrint(&testCase1);

    /*
     * Ovo ne bi trebalo da ima efekta.
     */
    printf("\nTrying to sort an empty heap:\n");
    HeapSort(&emptyTestCase);
    HeapPrint(&emptyTestCase);


    printf("---------------------------------\n");

    printf("Test case #5: Getting minimum value from heap:\n\n");

    int_least32_t minimum1 = HeapGetMinimum(&testCase1);
    int_least32_t minimum2 = HeapGetMinimum(&testCase2);
    int_least32_t minimum3 = HeapGetMinimum(&testCase3);

    printf("testCase1: %"PRIdLEAST32"\n", minimum1);
    printf("testCase2: %"PRIdLEAST32"\n", minimum2);
    printf("testCase3: %"PRIdLEAST32"\n\n", minimum3);

    printf("\nDeleting minimum nodes...\n");
    HeapDelete(&testCase1, minimum1);
    HeapDelete(&testCase2, minimum2);
    HeapDelete(&testCase3, minimum3);

    printf("testCase1: %"PRIdLEAST32"\n", HeapGetMinimum(&testCase1));
    printf("testCase2: %"PRIdLEAST32"\n", HeapGetMinimum(&testCase2));
    printf("testCase3: %"PRIdLEAST32"\n", HeapGetMinimum(&testCase3));
    printf("---------------------------------\n");

    printf("Test case #6: Stress testing\n\n");

    printf("Emptying out testCase1...\n");
    while (testCase1.size > 0U)
    {
        minimum1 = HeapGetMinimum(&testCase1);
        HeapDelete(&testCase1, minimum1);
    }

    printf("Trying to delete the minimum from testCase1:\n");
    minimum1 = HeapGetMinimum(&testCase1);
    if (!HeapDelete(&testCase1, minimum1))
    {
        printf("L%d: HeapDelete: empty heap! Minimum value returned %"PRIdLEAST32"\n", __LINE__ - 2, minimum1);
    }

    printf("\nOverfilling testCase1:\n");
    for (i = HEAP_MAXSIZE + 10U; i > 0U; i--)
    {
        if (!HeapAdd(&testCase1, (int_least32_t)i))
        {
            printf("L%d: HeapAdd: couldn't add value %"PRIdLEAST32"!\n", __LINE__ - 2, i);
        }
    }

    HeapPrint(&testCase1);
    printf("\nSorting testCase1:\n");
    HeapSort(&testCase1);
    HeapPrint(&testCase1);

    printf("\nTesting buffer overflow\n");
    minHeap overflowTest = {};
    HeapInit(&overflowTest, test3, 30U);
    HeapPrint(&overflowTest);

    return 0;
}
