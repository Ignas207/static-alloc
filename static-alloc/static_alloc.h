#ifndef STATIC_ALLOC_H
#define STATIC_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#define BUFF_SIZE 1024U

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

#define GET_ALLOCD_BUFFER_SIZE(buffer) ( \
    (uint32_t)(*buffer >> 24 + *(buffer +1) >> 16 + *(buffer + 2) >> 8 + *(buffer + 3)) \
)

uint8_t *memAlloc(uint32_t size);



#ifdef __cplusplus
}
#endif

#endif