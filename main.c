#include "main.h"
#include "static_alloc.h"
#include <sys/time.h>
#include <stdlib.h>

struct Memblock
{
    uint32_t size;
    uint8_t *data;
};

int main(void)
{
    printf("We are working!\n");
    // Test();

    // volatile struct Memblock memblock = {0};
    // memblock.size = 5;
    // memblock.data = memAlloc(5);
    // memset(memblock.data, 55, 5);

    // volatile void *testcast = (void*)&memblock;

    struct timeval tv1, tv2;
    // gettimeofday(&tv1, NULL);
    Static_alloc_testings();
    // gettimeofday(&tv2, NULL);

    // uint32_t testedValues[] = {10, 100, 1000, 10000, 100000, 1000000};
    // for (size_t i = 0; i < 6; i++)
    // {
    //     Static_alloc_test(testedValues[i]);
    //     stdAlloc_test(testedValues[i]);
    // }
    

    // printf ("Total time = %f ms\n",
    //      ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000+
    //      (double) (tv2.tv_sec - tv1.tv_sec)) *1000);

    return 0;
}

void stdAlloc_test(uint32_t size)
{
    struct timeval tv1, tv2;
    void *test = NULL;
    printf("Stblib malloc test with %d bytes: ", size);
    gettimeofday(&tv1, NULL);
    test = malloc(size);
    gettimeofday(&tv2, NULL);
    memset(test, 0, size);
    // strcat(test, "We are writing to this memory!");
    PrintExecTime_us(tv1, tv2);
    // printf("Our buffer: %s\n", test);
    free(test);
}

void Static_alloc_test(uint32_t size)
{
    struct timeval tv1, tv2;
    void *test = NULL;
    printf("Static alloc test with %d bytes: ", size);
    gettimeofday(&tv1, NULL);
    test = memAlloc(size);
    gettimeofday(&tv2, NULL);
    memset(test, 0, size);
    // strcat(test, "We are writing to this memory!");
    PrintExecTime_us(tv1, tv2);
    // printf("Our buffer: %s\n", test);
    memFree(test);
}

void Static_alloc_testings()
{
    struct timeval tv1, tv2;
    uint8_t *testMem = NULL;

    gettimeofday(&tv1, NULL);
    testMem = memAlloc(40U);
    gettimeofday(&tv2, NULL);
    PrintExecTime_us(tv1, tv2);
 
    strcat(testMem, "We are writing to this memory!");
    printf("Our buffer: %s\n", testMem);

    gettimeofday(&tv1, NULL);
    uint8_t *test2 = memAlloc(20);
    gettimeofday(&tv2, NULL);
    PrintExecTime_us(tv1, tv2);
    strcat(test2, "Writing again...\0");
    printf("Our buffer: %s\n", test2);

    memFree(test2);

    gettimeofday(&tv1, NULL);
    uint8_t *test3 = memAlloc(100);
    gettimeofday(&tv2, NULL);
    memset(test3, '9', 100);
    PrintExecTime_us(tv1, tv2);

    gettimeofday(&tv1, NULL);
    uint8_t *test1_resize = memRealloc(testMem, 90);
    gettimeofday(&tv2, NULL);
    PrintExecTime_us(tv1, tv2);
    if(test1_resize != NULL)
    {
        printf("Resize success!\n");
        strcat(test1_resize, " And now we are writing more stuff to this resized one!");
        printf("Our buffer: %s\n", test1_resize);
    }

    memFree(test1_resize);

    printf("This is a large alloc 512B: ");
    uint8_t *largeTest = NULL;
    gettimeofday(&tv1, NULL);
    largeTest = memAlloc(512);
    gettimeofday(&tv2, NULL);
    memset(largeTest + 60, 55, 100);
    PrintExecTime_us(tv1, tv2);

    printf("This is a large realoc 600B: ");
    uint8_t *largeRealoc = NULL;
    gettimeofday(&tv1, NULL);
    largeRealoc = memRealloc(largeTest, 600);
    gettimeofday(&tv2, NULL);
    PrintExecTime_us(tv1, tv2);


    
    // memFree()


    // memFree(testMem);
    // memFree(test2);
    // memFree(test3);
}

void PrintExecTime_us(struct timeval tv1, struct timeval tv2)
{
    printf ("Total time = %.2f us\n",
         ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000+
         (double) (tv2.tv_sec - tv1.tv_sec)) *1000000);
}