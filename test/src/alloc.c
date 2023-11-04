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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_SIZE (1U)
#define ITERATIONS (10U)

#define PRINT_INFO(format, ...) ZYLIB_LOG_INFO(log, format, ##__VA_ARGS__)
#define PRINT_WARN(format, ...) ZYLIB_LOG_WARN(log, format, ##__VA_ARGS__)
#define PRINT_ERROR(format, ...) ZYLIB_LOG_ERROR(log, format, ##__VA_ARGS__)

static zylib_log_t *log = NULL;

static zylib_return_t simulated_realloc_incremental_grow(const zylib_allocator_t *const alloc, size_t size, void **ptr)
{
    void *ptr_n = NULL;
    zylib_return_t r;
    if ((r = zylib_allocator_malloc(alloc, size, &ptr_n)) != ZYLIB_OK)
    {
        PRINT_ERROR("zylib_allocator_malloc");
        goto done;
    }
    memcpy(ptr_n, *ptr, size - 1);
    zylib_allocator_free(alloc, ptr);
    *ptr = ptr_n;
done:
    return r;
}

static _Bool test_loop(const zylib_allocator_t *const alloc,
                       zylib_return_t (*realloc)(const zylib_allocator_t *alloc, size_t size, void **ptr))
{
    _Bool r = false;

    void *ptr = NULL;

    for (unsigned int i = 0; i < ITERATIONS; ++i)
    {
        if (realloc(alloc, BASE_SIZE + i, &ptr) != ZYLIB_OK)
        {
            PRINT_ERROR("realloc(%u)", BASE_SIZE + i);
            goto done;
        }
    }

    r = true;
done:
    if (ptr != NULL)
    {
        zylib_allocator_free(alloc, &ptr);
    }
    return r;
}

int main()
{
    int r = EXIT_FAILURE;
    zylib_allocator_t *alloc = NULL;

    if (zylib_allocator_construct(&alloc, malloc, realloc, free) != ZYLIB_OK)
    {
        fprintf(stderr, "zylib_allocator_construct()\n");
        goto done;
    }

    if (zylib_log_construct(&log, alloc, "zylib-test-allocator-log.log", ZYLIB_INFO, ZYLIB_LOG_FORMAT_PLAIN) !=
        ZYLIB_OK)
    {
        fprintf(stderr, "zylib_log_construct()\n");
        goto done;
    }

    if (!test_loop(alloc, zylib_allocator_realloc))
    {
        PRINT_ERROR("test_loop: zylib_allocator_realloc");
        goto done;
    }

    if (!test_loop(alloc, simulated_realloc_incremental_grow))
    {
        PRINT_ERROR("test_loop: zylib_allocator_malloc");
        goto done;
    }

    r = EXIT_SUCCESS;
done:
    if (log != NULL)
    {
        zylib_log_destruct(&log);
    }
    if (alloc != NULL)
    {
        zylib_allocator_destruct(&alloc);
    }
    return r;
}
