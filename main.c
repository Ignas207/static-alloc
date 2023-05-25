#include "main.h"
#include "static_alloc.h"
#include <sys/time.h>

int main(void)
{
    printf("We are working!\n");
    // Test();

    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    Static_alloc_test();
    gettimeofday(&tv2, NULL);

    printf ("Total time = %f ms\n",
         ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000+
         (double) (tv2.tv_sec - tv1.tv_sec)) *1000);

    return 0;
}

void Static_alloc_test()
{
    uint8_t *testMem = NULL;
    testMem = memAlloc(40U);
    strcat(testMem, "We are writing to this memory!");
    printf("Our buffer: %s\n", testMem);

    uint8_t *test2 = memAlloc(20);
    strcat(test2, "Writing again...");
    printf("Our buffer: %s\n", test2);

    uint8_t *test3 = memAlloc(100);
    memset(test3, 3, 100);
    // memFree()
    

    memFree(testMem);
    memFree(test2);
    memFree(test3);
}