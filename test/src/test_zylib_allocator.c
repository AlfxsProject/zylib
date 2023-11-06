/*
 * Copyright 2023 Alexandre Fernandez <alex@fernandezfamily.email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "zylib_allocator.h"
#include "zylib_log.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT_ERROR(format, ...) ZYLIB_LOG_ERROR(log, format, ##__VA_ARGS__)

static zylib_allocator_t *allocator = NULL;
static zylib_log_t *log = NULL;

static inline _Bool test_region(void *ptr, size_t size, int chr);
static inline _Bool test_malloc_free(size_t m_size);
static inline _Bool test_malloc_realloc_free(size_t m_size, size_t r_size);

int main()
{
    int r = EXIT_FAILURE;

    if (!zylib_allocator_construct(&allocator, malloc, realloc, free))
    {
        fprintf(stderr, "zylib_allocator_construct() failed\n");
        goto error;
    }

    if (!zylib_log_construct(&log, allocator, stderr, ZYLIB_INFO, ZYLIB_LOG_FORMAT_PLAIN))
    {
        fprintf(stderr, "zylib_log_construct() failed\n");
        goto error;
    }

    /*
     * TESTS
     */

    if (!test_malloc_free((size_t)(rand() % 100 + 100)))
    {
        PRINT_ERROR("test_malloc_free() failed");
        goto error;
    }

    if (!test_malloc_realloc_free((size_t)(rand() % 100 + 100), (size_t)(rand() % 100 + 500)))
    {
        PRINT_ERROR("test_malloc_realloc_free() failed");
        goto error;
    }

    if (!test_malloc_realloc_free((size_t)(rand() % 100 + 500), (size_t)(rand() % 100 + 100)))
    {
        PRINT_ERROR("test_malloc_realloc_free() failed");
        goto error;
    }

    r = EXIT_SUCCESS;
error:
    if (log != NULL)
    {
        zylib_log_destruct(&log);
    }
    if (allocator != NULL)
    {
        zylib_allocator_destruct(&allocator);
    }
    return r;
}

_Bool test_malloc_free(size_t m_size)
{
    _Bool r = 0;
    void *ptr = NULL;

    if (!zylib_allocator_malloc(allocator, m_size, &ptr))
    {
        PRINT_ERROR("zylib_allocator_malloc() failed");
        goto error;
    }

    if (!test_region(ptr, m_size, rand() % 255))
    {
        PRINT_ERROR("test_region() failed");
        goto error;
    }

    r = 1;
error:
    if (ptr != NULL)
    {
        zylib_allocator_free(allocator, &ptr);
    }
    return r;
}

_Bool test_malloc_realloc_free(size_t m_size, size_t r_size)
{
    _Bool r = 0;
    void *ptr = NULL;

    if (!zylib_allocator_malloc(allocator, m_size, &ptr))
    {
        PRINT_ERROR("zylib_allocator_malloc() failed");
        goto error;
    }

    if (!test_region(ptr, m_size, rand() % 255))
    {
        PRINT_ERROR("test_region() failed");
        goto error;
    }

    if (!zylib_allocator_realloc(allocator, r_size, &ptr))
    {
        PRINT_ERROR("zylib_allocator_realloc() failed");
        goto error;
    }

    if (!test_region(ptr, r_size, rand() % 255))
    {
        PRINT_ERROR("test_region() failed");
        goto error;
    }

    r = 1;
error:
    if (ptr != NULL)
    {
        zylib_allocator_free(allocator, &ptr);
    }
    return r;
}

_Bool test_region(void *ptr, size_t size, int chr)
{
    _Bool r = 0;

    memset(ptr, chr, size);

    for (size_t i = 0; i < size; ++i)
    {
        if (((uint8_t *)ptr)[i] != chr)
        {
            PRINT_ERROR("memset() failed");
            goto error;
        }
    }

    r = 1;
error:
    return r;
}
