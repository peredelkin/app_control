#include "parity.h"


bool calc_parity(uint32_t value, size_t bits_count, parity_type_t type)
{
    bool par = (bool) type;
    size_t bit;
    for(bit = 0; bit < bits_count; bit ++){
        par ^= ((value >> bit) & 0b1);
    }
    return par;
}
