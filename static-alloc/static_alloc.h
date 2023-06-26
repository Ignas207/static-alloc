#ifndef STATIC_ALLOC_H
#define STATIC_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

enum
{
	BUFF_SIZE = 20240000U,
	HEADER_SIZE = 4U,
	DATA_SECTION_END = 0xFFU
};

#define DATA_SECTION_START(headerSize) ((uint8_t)(headerSize)+2)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/**
 * @brief Gives us a pointer to the beginning of INFO section.
 *         Need to give a pointer at the start of USER section.
 * 
 */
#define GET_ALLOC_BUFFER_START(buffer)( \
    (uint8_t*)(buffer - *(buffer -1)) \
)

#define IS_ALLOC_MODE(buffer, mode)( (uint8_t)((*buff & mode) && 1))

#define GET_ALLOC_BUFFER_HEADER_LENGHT(buffer)( (uint8_t)(*(buffer -1)) )

#define GET_ALLOC_BUFFER_HEADER_LENGHT_START(buffer)( (uint8_t)(*(buffer -1)) )

#define GET_BUFFER_CHECKSUM(buff, count) ( \
    (uint8_t)(*(buff + count) & 1) \
)

#define GET_HEADER_CHECKSUM(checksum, type) ( \
    (uint8_t)(*(checksum) & (type << 1) ) \
)

typedef enum
{
    MODE_8BIT = 1U,
    MODE_16BIT = 4U,
    MODE_24BIT = 16U,
    MODE_32BIT = 64U,
} ALLOC_SIZE_TYPE;

uint8_t *memAlloc(uint32_t size);
uint8_t *memRealloc(uint8_t *memPtr, uint32_t size);
void memFree(uint8_t *memPtr);

static uint8_t GetBitAmount(uint32_t number);
static uint32_t getUserSize(const uint8_t *const buff);
static uint32_t getTotalSize(const uint8_t *const buff);
static uint32_t getSize(const uint8_t *const buff, const uint8_t getTotal);
static uint8_t *findEmptyLocation(const uint8_t *const buff, uint32_t size);
static uint32_t CheckIfFree(const uint8_t *const buff, const uint32_t end);
static uint8_t *SetBufferLenght(uint8_t *const memPtr, const uint32_t size);
static uint8_t *bufferAccess(void);

#ifdef __cplusplus
}
#endif

#endif