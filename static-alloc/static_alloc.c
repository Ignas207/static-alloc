#include "static_alloc.h"

static uint8_t buffer[BUFF_SIZE] = {0};

/**
 * @brief Allocates requested amount of memory from the stack.
 * 
 * @param size memory size to allocate.
 * @return uint8_t* pointer to memory region, can be NULL if alloc failed!
 */
uint8_t *memAlloc(const uint32_t size)
{
    uint8_t *retBuffer = NULL;

    if (size == 0)
        return NULL;

    if (size > BUFF_SIZE)
        return NULL;

    retBuffer = findEmptyLocation(bufferAccess(), size);
    if (retBuffer == NULL)
        return NULL;

    return SetBufferLenght(retBuffer, size);
}

uint8_t *memRealloc(uint8_t *memPtr, const uint32_t size)
{
    uint8_t *tempPtr = GET_ALLOC_BUFFER_START(memPtr);
    const uint32_t memSize = getUserSize(tempPtr);
    if (size < memSize)
    {
        // TODO: possible data corruption if size blocks decrease!
        memset(memPtr + size, 0, memSize - size);
        return SetBufferLenght(tempPtr, size);
    }

    // Checking if we can resize the existing buffer.
    if (CheckIfFree(memPtr + memSize, size - memSize) == 0)
    {
        // TODO: possible data corruption if size blocks increase!
        SetBufferLenght(tempPtr, size);
        return memPtr;
    }

    uint8_t *allocdPtr = findEmptyLocation(bufferAccess(), size);
    if (allocdPtr == NULL)
        return NULL;

    uint8_t *respPtr = SetBufferLenght(allocdPtr, size);
    memcpy(respPtr, memPtr, memSize);
    memFree(memPtr);
    return respPtr;
}

/**
 * @brief Writes the Header for the allocated data.
 * 
 * @param memPtr Starting location of the allocated data buffer.
 * @param size Memory size to allocate.
 * @return uint8_t* Starting point of the allocated USER section.
 */
static uint8_t *SetBufferLenght(uint8_t *const memPtr, const uint32_t size)
{
	const uint8_t headerSize = GetBitAmount(size);
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
        *(memPtr) += (*(memPtr + i) & 1) >> (checksumPos);
        *(memPtr) += (1 << enabledPos);
        checksumPos -= 2U;
        enabledPos -= 2U;
    }
    
    *(memPtr + headerSize +1) = DATA_SECTION_START(headerSize);
    *(memPtr + headerSize +2 + size) = DATA_SECTION_END;

    return (memPtr + headerSize +2);
}

static inline uint8_t *bufferAccess(void)
{
    // static uint8_t buffer[BUFF_SIZE] = {0};
    return (uint8_t *)&buffer;
}

/**
 * @brief Finds an empty location in the provided buffer and returns its pointer.
 * 
 * @param buff 
 * @param size 
 * @return uint8_t* 
 */
static uint8_t *findEmptyLocation(const uint8_t *const buff, const uint32_t size)
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
            buff_size = temp;
        }
        i += buff_size;
    }
    return NULL;
}

static uint8_t GetBitAmount(uint32_t number)
{
    uint8_t shifted = 0;
    while (number > 0)
    {
        number = number >> 8U;
        shifted++;
    }
    return shifted;
}

/**
 * @brief Check if the memory region is free.
 * 
 * @param buff buffer pointer to start from.
 * @param end bytes to check.
 * @return uint32_t 1: next byte is occupied; 0: region is free; other result
 * indicates a memory region at this point.
 */
static uint32_t CheckIfFree(const uint8_t *const buff, const uint32_t end)
{
    for (uint32_t j = 0; j < end; ++j)
    {
        if ((*(buff + j) && 1))
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

/**
 * @brief Get the TOTAL Size of the allocated memory region.
 * 
 * @param buff 
 * @return uint32_t 
 */
static uint32_t getTotalSize(const uint8_t *const buff)
{
    return getSize(buff, 1);
}

/**
 * @brief Get the size of the ALLOCATED memory region.
 * 
 * @param buff 
 * @return uint32_t 
 */
static uint32_t getUserSize(const uint8_t *const buff)
{
    return getSize(buff, 0);
}

/**
 * @brief Get the size of the allocated memory.
 * 
 * @param buff Data buffer, needs to point at the beginning!
 * @param getTotal if 1, then will include the header lenght.
 * @return uint32_t size of allocated memory region.
 */
static uint32_t getSize(const uint8_t *const buff, const uint8_t getTotal)
{
    uint32_t size = 0;
    uint8_t counter = 1;

    if (IS_ALLOC_MODE(buff, MODE_32BIT))
    {
        if (GET_BUFFER_CHECKSUM(buff, counter) == GET_HEADER_CHECKSUM(buff, MODE_32BIT))
        {
            size += *(buff + counter) << 24U;
            counter++;
        }
    }

    if (IS_ALLOC_MODE(buff, MODE_24BIT))
    {
        if (GET_BUFFER_CHECKSUM(buff, counter) == GET_HEADER_CHECKSUM(buff, MODE_24BIT))
        {
            size += *(buff + counter) << 16U;
            counter++;
        }
    }

    if (IS_ALLOC_MODE(buff, MODE_16BIT))
    {
        if (GET_BUFFER_CHECKSUM(buff, counter) == GET_HEADER_CHECKSUM(buff, MODE_16BIT))
        {
            size += *(buff + counter) << 8U;
            counter++;
        }
    }

    if (IS_ALLOC_MODE(buff, MODE_8BIT))
    {
        if (GET_BUFFER_CHECKSUM(buff, counter) == GET_HEADER_CHECKSUM(buff, MODE_8BIT))
        {
            size += *(buff + counter);
        }
    }

    if(getTotal == 1)
        size += *(buff + counter +1);

    // We are including the Ending section!
    if(size != 0)
        size += 1;

    return size;
}

/**
 * @brief Frees the allocated memory region.
 * 
 * @param memPtr 
 */
void memFree(uint8_t *memPtr)
{
    if (memPtr == NULL)
        return;
    uint8_t *tempPtr = GET_ALLOC_BUFFER_START(memPtr);
    memset(tempPtr, 0, getTotalSize(tempPtr));
    memPtr = NULL;
}
