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

/*
 * Types
 */

struct zylib_allocator_s
{
    zylib_allocator_malloc_t malloc;
    zylib_allocator_realloc_t realloc;
    zylib_allocator_free_t free;
};

/*
 * Functions
 */

_Bool zylib_allocator_construct(zylib_allocator_t **allocator, zylib_allocator_malloc_t malloc,
                                zylib_allocator_realloc_t realloc, zylib_allocator_free_t free)
{
    _Bool r = 0;

    if (allocator == NULL || malloc == NULL || realloc == NULL || free == NULL)
    {
        return 0;
    }

    *allocator = malloc(sizeof(zylib_allocator_t));
    if (*allocator == NULL)
    {
        goto error;
    }

    (*allocator)->malloc = malloc;
    (*allocator)->realloc = realloc;
    (*allocator)->free = free;
    r = 1;
error:
    return r;
}

void zylib_allocator_destruct(zylib_allocator_t **allocator)
{
    if (allocator == NULL || *allocator == NULL)
    {
        return;
    }

    (*allocator)->free((void *)*allocator);
    *allocator = NULL;
}

_Bool zylib_allocator_malloc(const zylib_allocator_t *allocator, size_t size, void **p_void)
{
    _Bool r = 0;

    if (allocator == NULL || size <= 0 || p_void == NULL)
    {
        return 0;
    }

    *p_void = allocator->malloc(size);
    if (*p_void == NULL)
    {
        goto error;
    }

    r = 1;
error:
    return r;
}

_Bool zylib_allocator_realloc(const zylib_allocator_t *allocator, size_t size, void **p_void)
{
    _Bool r = 0;
    void *ptr = NULL;

    if (allocator == NULL || size <= 0 || p_void == NULL || *p_void == NULL)
    {
        return 0;
    }

    ptr = allocator->realloc(*p_void, size);
    if (ptr == NULL)
    {
        goto error;
    }
    *p_void = ptr;

    r = 1;
error:
    return r;
}

void zylib_allocator_free(const zylib_allocator_t *allocator, void **p_void)
{
    if (allocator == NULL || p_void == NULL || *p_void == NULL)
    {
        return;
    }

    allocator->free(*p_void);
    *p_void = NULL;
}
