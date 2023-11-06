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
#include "zylib_private_allocator.h"

/*
 * Types
 */

struct zylib_private_allocator_s
{
    zylib_private_allocator_malloc_t malloc;
    zylib_private_allocator_realloc_t realloc;
    zylib_private_allocator_free_t free;
};

/*
 * Functions
 */

_Bool zylib_private_allocator_construct(zylib_private_allocator_t **allocator, zylib_private_allocator_malloc_t malloc,
                                        zylib_private_allocator_realloc_t realloc, zylib_private_allocator_free_t free)
{
    _Bool r = 0;

    *allocator = malloc(sizeof(zylib_private_allocator_t));
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

void zylib_private_allocator_destruct(zylib_private_allocator_t **allocator)
{
    if (*allocator == NULL)
    {
        return;
    }

    (*allocator)->free((void *)*allocator);
    *allocator = NULL;
}

_Bool zylib_private_allocator_malloc(const zylib_private_allocator_t *allocator, size_t size, void **data)
{
    _Bool r = 0;

    if (size <= 0)
    {
        return 0;
    }

    *data = allocator->malloc(size);
    if (*data == NULL)
    {
        goto error;
    }

    r = 1;
error:
    return r;
}

_Bool zylib_private_allocator_realloc(const zylib_private_allocator_t *allocator, size_t size, void **data)
{
    _Bool r = 0;
    void *ptr = NULL;

    if (size <= 0 || *data == NULL)
    {
        return 0;
    }

    ptr = allocator->realloc(*data, size);
    if (ptr == NULL)
    {
        goto error;
    }
    *data = ptr;

    r = 1;
error:
    return r;
}

void zylib_private_allocator_free(const zylib_private_allocator_t *allocator, void **data)
{
    if (*data == NULL)
    {
        return;
    }

    allocator->free(*data);
    *data = NULL;
}
