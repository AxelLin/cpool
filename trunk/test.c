#include <stdio.h>
#include <string.h>
#include "mem_pool.h"

const int SIZE = 16*1024;

extern size_t OBJ_NUM;

#define TEST_TRUE(a)    \
        if (!(a)) {       \
            printf("wrong [line:%lu]", __LINE__);  \
        }

void sequence_case1(void)
{
    mem_pool_t *mpl;
    void *new;
    int i;
    void *array[1000];
       
    mpl = create_mem_pool(SIZE);

    for (i = 0; i < 820; i++) {
        new = mem_pool_alloc(mpl);
        if (new) {
            memset(new, 'a', SIZE);
            array[i] = new;
        }
    }

    TEST_TRUE(mpl->slab_nr == 820/OBJ_NUM + 1)

    for (i = 0; i < 10; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }
    for (i = 10; i < 30; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }
    for (i = 30; i < 820; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }

    TEST_TRUE(mpl->slab_nr == 820/OBJ_NUM + 1)

    destroy_mem_pool(mpl);
}

void sequence_case2(void)
{
    mem_pool_t *mpl;
    void *new;
    int i;
    void *array[1000];
       
    mpl = create_mem_pool(SIZE);

    /* alloc 200 chunks */
    for (i = 0; i < 200; i++) {
        new = mem_pool_alloc(mpl);
        if (new) {
            memset(new, 'a', SIZE);
            array[i] = new;
        }
    }

    TEST_TRUE(mpl->slab_nr == 200/OBJ_NUM + 1)

    /* free the 80 chunks */
    for (i = 0; i < 80; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }

    TEST_TRUE(mpl->slab_nr == 200/OBJ_NUM + 1)

    /* alloc 80 chunks */
    for (i = 0; i < 80; i++) {
        new = mem_pool_alloc(mpl);
        if (new) {
            memset(new, 'a', SIZE);
            array[i] = new;
        }
    }

    TEST_TRUE(mpl->slab_nr == 200/OBJ_NUM + 1)

    /* free 200 chunks */
    for (i = 0; i < 200; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }

    TEST_TRUE(mpl->slab_nr == 200/OBJ_NUM + 1)

    destroy_mem_pool(mpl);
}

void sequence_case3(void)
{
    mem_pool_t *mpl;
    void *new;
    int i;
    void *array[1000];
       
    mpl = create_mem_pool(SIZE);

    /* alloc 128 chunks */
    for (i = 0; i < 128; i++) {
        new = mem_pool_alloc(mpl);
        if (new) {
            memset(new, 'a', SIZE);
            array[i] = new;
        }
    }

    TEST_TRUE(mpl->slab_nr == 1)

    /* free the 80 chunks */
    for (i = 0; i < 80; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }

    TEST_TRUE(mpl->slab_nr == 1)

    /* alloc 80 + 128 chunks */
    for (i = 0; i < 80; i++) {
        new = mem_pool_alloc(mpl);
        if (new) {
            memset(new, 'a', SIZE);
            array[i] = new;
        }
    }

    for (i = 128; i < 256; i++) {
        new = mem_pool_alloc(mpl);
        if (new) {
            memset(new, 'a', SIZE);
            array[i] = new;
        }
    }

    TEST_TRUE(mpl->slab_nr == 2)

    /* free 256 chunks */
    for (i = 0; i < 256; i++) {
        if (array[i]) {
            mem_pool_free(mpl, array[i]);
        }
    }

    TEST_TRUE(mpl->slab_nr == 2)

    TEST_TRUE(mpl->slab_nr == 200/OBJ_NUM + 1)

    destroy_mem_pool(mpl);
}

int main(int argc, char* argv[])
{
    sequence_case1();
    sequence_case2();
    sequence_case3();
    return 0;
}
