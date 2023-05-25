#include "static_alloc.h"

// static uint8_t buffer[BUFF_SIZE] = {0};

uint8_t *memAlloc(uint32_t size)
{
    uint8_t *retBuffer = NULL;

    if (size == 0)
        return (uint8_t *)(0);

    if (size > BUFF_SIZE)
        return (uint8_t *)(0);

    retBuffer = findEmptyLocation(bufferAccess());
    if(retBuffer == NULL)
        return NULL;

    *retBuffer = size >> 7U;
    *(retBuffer +1) = size  - (size >> 7U);

    return (retBuffer + ALLOC_SIZE);
}

uint8_t *memRealloc(uint8_t *memPtr, uint32_t size)
{
    uint8_t *retPtr = findEmptyLocation(memPtr -ALLOC_SIZE);
}

static uint8_t *bufferAccess(void)
{
    static uint8_t buffer[BUFF_SIZE] = {0};
    return &buffer;
}

static uint8_t *findEmptyLocation(uint8_t *buff)
{
    uint16_t i = 0;
    uint16_t buff_size = 0;
    while (i < BUFF_SIZE)
    {
        buff_size = getSize((buff + i));
        if (buff_size == 0)
        {
            return (buff + i);
        }
        i += buff_size;
    }
    return NULL;
}

static uint16_t getSize(uint8_t *buff)
{
    return (*buff >> 8) + *(buff + 1);
}

void memFree(uint8_t *memPtr)
{
    memset(memPtr -ALLOC_SIZE, 0, getSize(memPtr -ALLOC_SIZE));
    memPtr = NULL;
}
