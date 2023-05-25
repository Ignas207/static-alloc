#include "static_alloc.h"

// static uint8_t buffer[BUFF_SIZE] = {0};

uint8_t *memAlloc(uint32_t size)
{
    uint8_t *retBuffer = NULL;

    if (size == 0)
        return NULL;

    if (size > BUFF_SIZE)
        return NULL;

    retBuffer = findEmptyLocation(bufferAccess(), size);
    if(retBuffer == NULL)
        return NULL;

    SetBufferLenght(retBuffer, size);
    return (retBuffer + ALLOC_SIZE);
}

uint8_t *memRealloc(uint8_t *memPtr, uint32_t size)
{
    uint16_t memSize = getSize(memPtr - ALLOC_SIZE);
    if(size < memSize)
    {
        SetBufferLenght(memPtr - ALLOC_SIZE, size);
        memset(memPtr + size, 0, memSize - size);
        return memPtr;
    }

    // Checking if we can resize the existing buffer.
    if(CheckIfFree(memPtr + memSize, size - memSize) == 0)
    {
       SetBufferLenght(memPtr - ALLOC_SIZE, size);
       return memPtr;
    }

    uint8_t *retPtr = findEmptyLocation(bufferAccess(), size);
    if(retPtr == NULL)
        return NULL;
    
    SetBufferLenght(retPtr, size);
    memcpy(retPtr + ALLOC_SIZE, memPtr, memSize);
    memset(memPtr - ALLOC_SIZE, 0, memSize);
    memPtr = NULL;
    return retPtr + ALLOC_SIZE;
}

static void SetBufferLenght(uint8_t *memPtr, uint16_t size)
{
    *(memPtr) = size >> 7U;
    *(memPtr+1) = size - (size >> 7U);
}

static uint8_t *bufferAccess(void)
{
    static uint8_t buffer[BUFF_SIZE] = {0};
    return (uint8_t*)&buffer;
}

static uint8_t *findEmptyLocation(uint8_t *buff, uint32_t size)
{
    uint32_t i = 0;
    uint32_t buff_size = 0;
    uint32_t temp = 0;
    while (i < BUFF_SIZE)
    {
        buff_size = getSize((buff + i));
        if (buff_size == 0)
        {
            if(i + size > BUFF_SIZE)
                return NULL;
            
            temp = CheckIfFree(buff + i, i + size);
            if(temp == 0)
                return (buff + i);
            else
                buff_size = temp;
        }
        i += buff_size;
    }
    return NULL;
}

static uint32_t CheckIfFree(uint8_t *buff, uint32_t end)
{
    for (uint32_t j = 0; j < end; j++)
    {
        if(*(buff +j) != 0)
        {
            return j;
        }
    }
    return 0U;
}

static uint16_t getSize(uint8_t *buff)
{
    return (*buff >> 7) + *(buff + 1);
}

void memFree(uint8_t *memPtr)
{
    if(memPtr == NULL)
        return;
    memset(memPtr -ALLOC_SIZE, 0, getSize(memPtr -ALLOC_SIZE));
    memPtr = NULL;
}
