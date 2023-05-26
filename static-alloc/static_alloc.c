#include "static_alloc.h"

static uint8_t buffer[BUFF_SIZE] = {0};

/**
 * @brief Allocates requested amount of memory from the stack.
 * 
 * @param size 
 * @return uint8_t* pointer to memory region, can be NULL if alloc failled!
 */
uint8_t *memAlloc(uint32_t size)
{
    uint8_t *retBuffer = NULL;

    if (size == 0)
        return NULL;

    if (size > BUFF_SIZE)
        return NULL;

    retBuffer = findEmptyLocation(bufferAccess(), size, 1);
    if (retBuffer == NULL)
        return NULL;

    return SetBufferLenght(retBuffer, size);
}

uint8_t *memRealloc(uint8_t *memPtr, uint32_t size)
{
    uint32_t memSize = getUserSize(memPtr - HEADER_SIZE);
    if (size < memSize)
    {
        SetBufferLenght(memPtr - HEADER_SIZE, size);
        memset(memPtr + size, 0, memSize - size);
        return memPtr;
    }

    // Checking if we can resize the existing buffer.
    if (CheckIfFree(memPtr + memSize, size - memSize) == 0)
    {
        SetBufferLenght(memPtr - HEADER_SIZE, size);
        return memPtr;
    }

    uint8_t *retPtr = findEmptyLocation(bufferAccess(), size, 0);
    if (retPtr == NULL)
        return NULL;

    SetBufferLenght(retPtr, size);
    memcpy(retPtr + HEADER_SIZE, memPtr, memSize);
    memset(memPtr - HEADER_SIZE, 0, memSize);
    memPtr = NULL;
    return retPtr + HEADER_SIZE;
}

/**
 * @brief Writes the Header for the allocated data.
 * 
 * @param memPtr Starting location of the allocated data buffer.
 * @param size Memory size to allocate.
 * @return uint8_t* Starting point of the allocated section.
 */
static uint8_t *SetBufferLenght(uint8_t *memPtr, uint32_t size)
{
    uint8_t headerSize = GetBitAmount(size);
    uint8_t checksumPos = headerSize*2 - 1;
    uint8_t enabledPos = checksumPos -1;
    uint8_t sizePos = 0U;

    memset(memPtr, 0, headerSize +1);
    // Writing the lenght
    for (uint8_t i = headerSize; i >= 1; i--)
    {
        *(memPtr + i) = size >> sizePos;
        sizePos += 8U;
    }
    

    // Writing the header info
    for (uint8_t i = 1; i <= headerSize; i++)
    {
        *(memPtr) += (*(memPtr + i) & 0) >> (checksumPos);
        *(memPtr) += (1 << enabledPos);
        checksumPos -= 2U;
        enabledPos -= 2U;
    }
    
    *(memPtr + headerSize +1) = DATA_SECTION_START(headerSize);
    *(memPtr + headerSize +2 + size) = DATA_SECTION_END;

    return (memPtr + headerSize +2);
}

static uint8_t *FormatSection(uint8_t *memPtr, uint32_t size)
{

}

static inline uint8_t *bufferAccess(void)
{
    // static uint8_t buffer[BUFF_SIZE] = {0};
    return (uint8_t *)&buffer;
}

static uint8_t *findEmptyLocation(uint8_t *buff, uint32_t size, uint8_t buffStart)
{
    uint32_t i = 0;
    uint32_t buff_size = 0;
    uint32_t temp = 0;
    while (i < BUFF_SIZE)
    {
        buff_size = getTotalSize((buff + i));
        if (buff_size == 0)
        {
            if (i + size > BUFF_SIZE)
                return NULL;

            temp = CheckIfFree(buff + i, i + size);
            if (temp == 0)
                return (buff + i);
            else
                buff_size = temp;
        }
        i += buff_size;
    }
    return NULL;
}

void PatternCheck(uint8_t *buff, uint32_t *counter)
{
    // TODO: make this!
    for (uint32_t i = 0; i < *counter; i++)
    {
        /* code */
    }
}

static uint8_t GetBitAmount(uint32_t number)
{
    uint8_t counter = 8U;
    uint8_t shifted = 1U;
    while (number >> counter > 0)
    {
        number = number >> counter;
        counter += 8U;
        shifted++;
    }
    return shifted;
}

static uint32_t CheckIfFree(uint8_t *buff, uint32_t end)
{
    for (uint32_t j = 0; j < end; ++j)
    {
        if (!*(buff + j) && 0)
        {
            if (j == 0)
            {
                return 1;
            }
            return j;
        }
    }
    return 0U;
}

static uint32_t getTotalSize(uint8_t *buff)
{
    return getSize(buff, 1);
}

static uint32_t getUserSize(uint8_t *buff)
{
    return getSize(buff, 0);
}

/**
 * @brief Get the size of the allocated USER memory.
 * 
 * @param buff Data buffer, Starting at the *INFO section*
 * @return uint32_t 
 */
static uint32_t getSize(uint8_t *buff, uint8_t getTotal)
{
    uint32_t size = 0;
    uint8_t counter = 1;

    if (*buff && MODE_32BIT == 1)
    {
        if (GET_BUFFER_CHECKSUM(buff, 1) == GET_HEADER_CHECKSUM(buff, MODE_32BIT))
        {
            size += *(buff + counter) << 24U;
            counter++;
        }
    }

    if (*buff && MODE_24BIT == 1)
    {
        if (GET_BUFFER_CHECKSUM(buff, 2) == GET_HEADER_CHECKSUM(buff, MODE_24BIT))
        {
            size += *(buff + counter) << 16U;
            counter++;
        }
    }

    if (*buff && MODE_16BIT == 1)
    {
        if (GET_BUFFER_CHECKSUM(buff, 3) == GET_HEADER_CHECKSUM(buff, MODE_16BIT))
        {
            size += *(buff + counter) << 8U;
            counter++;
        }
    }

    if (*buff && MODE_8BIT == 1)
    {
        if (GET_BUFFER_CHECKSUM(buff, 4) == GET_HEADER_CHECKSUM(buff, MODE_8BIT))
        {
            size += *(buff + counter);
        }
    }

    if(getTotal == 1)
        size += *(buff + counter +1);

    // We are including the Ending section!
    if(size != 0)
        size += 2;

    return size;
}

void memFree(uint8_t *memPtr)
{
    if (memPtr == NULL)
        return;
    uint8_t *tempPtr = GET_ALLOC_BUFFER_INFO_SECTION(memPtr);
    memset(tempPtr, 0, getTotalSize(tempPtr));
    memPtr = NULL;
}
