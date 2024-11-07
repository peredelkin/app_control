#include <errno.h>
#include "fmc_sram.h"


/*
 sbrk - увеличить размер области данных, использутся для malloc.
 */
void* _sbrk(ptrdiff_t incr)
{
    extern char _sextsram;
    extern char _eextsram;

    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0)
    {
        heap_end = &_eextsram;
    }
    prev_heap_end = heap_end;

    char * end_of_extsram = (char*) &_sextsram + SRAM_WORDS_8;
    if (heap_end + incr > end_of_extsram)
    {
        errno = ENOMEM;
        return (void*) -1;
    }

    heap_end += incr;
    return (void*) prev_heap_end;

}
