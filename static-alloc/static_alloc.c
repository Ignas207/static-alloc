#include "static_alloc.h"

static uint8_t buffer[BUFF_SIZE] = {0};


uint8_t *memAlloc(uint32_t size)
{
    if(size == 0)
        return (uint8_t*)(0);

    
}

