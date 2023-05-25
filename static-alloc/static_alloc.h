#ifndef STATIC_ALLOC_H
#define STATIC_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 1024U
#define ALLOC_SIZE 2U

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define GET_ALLOCD_BUFFER_SIZE(buffer) ( \
    (uint32_t)(*buffer >> 24 + *(buffer +1) >> 16 + *(buffer + 2) >> 8 + *(buffer + 3)) \
)

uint8_t *memAlloc(uint32_t size);
static uint16_t getSize(uint8_t *buff);
static uint8_t *findEmptyLocation(uint8_t *buff);
uint8_t *memRealloc(uint8_t *memPtr, uint32_t size);
void memFree(uint8_t *memPtr);
static uint8_t *bufferAccess(void);


#ifdef __cplusplus
}
#endif

#endif