#ifndef PARITY_H
#define PARITY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum _Parity_Type {
    PARITY_EVEN = 0,
    PARITY_ODD = 1
} parity_type_t;

extern bool calc_parity(uint32_t value, size_t bits_count, parity_type_t type);

#endif  //PARITY_H
