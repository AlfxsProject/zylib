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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zylib_alloc.h>
#include <zylib_log.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#define BASE_SIZE (1U)
#define ITERATIONS (10U)

#define PRINT(format, ...) ZYLIB_LOG_ERROR(log, format, ##__VA_ARGS__)

static zylib_log_t *log = NULL;

static zylib_return_t simulated_realloc_incremental_grow(const zylib_alloc_t *const alloc, size_t size, void **ptr)
{
    void *ptr_n = NULL;
    zylib_return_t r;
    if ((r = zylib_malloc(alloc, size, &ptr_n)) != ZYLIB_OK)
    {
        PRINT("zylib_malloc");
        goto done;
    }
    memcpy(ptr_n, *ptr, size - 1);
    zylib_free(alloc, ptr);
    *ptr = ptr_n;
done:
    return r;
}

static _Bool test_loop(const zylib_alloc_t *const alloc,
                       zylib_return_t (*realloc)(const zylib_alloc_t *alloc, size_t size, void **ptr))
{
    _Bool r = false;

    void *ptr = NULL;

    for (unsigned int i = 0; i < ITERATIONS; ++i)
    {
        if (realloc(alloc, BASE_SIZE + i, &ptr) != ZYLIB_OK)
        {
            PRINT("realloc(%u)", BASE_SIZE + i);
            goto done;
        }
    }

    r = true;
done:
    if (ptr != NULL)
    {
        zylib_free(alloc, &ptr);
    }
    return r;
}

int main()
{
    int r = EXIT_FAILURE;
    zylib_alloc_t *alloc = NULL;

    if (zylib_alloc_construct(&alloc, malloc, realloc, free) != ZYLIB_OK)
    {
        fprintf(stderr, "zylib_alloc_construct()\n");
        goto done;
    }

    if (zylib_log_construct(&log, alloc, "zylib-test-alloc-log.log", (zylib_log_severity_t)ZYLIB_LOG_OUTPUT_FORMAT_MAX,
                            ZYLIB_FORMAT_PLAIN) != ZYLIB_OK)
    {
        fprintf(stderr, "zylib_log_construct()\n");
        goto done;
    }

    if (!test_loop(alloc, zylib_realloc))
    {
        PRINT("test_loop: zylib_realloc");
        goto done;
    }

    if (!test_loop(alloc, simulated_realloc_incremental_grow))
    {
        PRINT("test_loop: zylib_malloc");
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
        zylib_alloc_destruct(&alloc);
    }
    return r;
}

#pragma clang diagnostic pop
