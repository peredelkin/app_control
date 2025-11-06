#include <errno.h>
#include "fmc_sram.h"


/*
 sbrk - увеличить размер области данных, использутся для malloc.
 */
void* _sbrk(ptrdiff_t incr)
{
    extern char _sextsram[];
    extern char _eextsram[];

    static char *heap_end = NULL;
    char *prev_heap_end;

    if (heap_end == NULL)
    {
        heap_end = _eextsram;
    }
    prev_heap_end = heap_end;

    char * end_of_extsram = _sextsram + SRAM_WORDS_8;
    if ((ptrdiff_t)(heap_end + incr) > (ptrdiff_t)end_of_extsram)
    {
        errno = ENOMEM;
        return (void*) -1;
    }

    heap_end += incr;
    return (void*) prev_heap_end;

}
