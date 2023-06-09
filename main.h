#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <time.h>

typedef unsigned int uint32_t;

int main(void);
void Static_alloc_testings();
void PrintExecTime_us(struct timeval tv1, struct timeval tv2);
void stdAlloc_test(uint32_t size);
void Static_alloc_test(uint32_t size);
void AllocComparison(const uint32_t size);
void AllocTest(const uint32_t size, void *(*allocFunction)(uint32_t const *const func),
               void *(*freeFunction)(void const *const mem));

#endif