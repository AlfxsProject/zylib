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

_Bool zylib_private_allocator_construct(zylib_private_allocator_t **obj, zylib_private_allocator_malloc_t malloc,
                                        zylib_private_allocator_realloc_t realloc, zylib_private_allocator_free_t free)
{
    _Bool r = 0;

    *obj = malloc(sizeof(zylib_private_allocator_t));
    if (*obj == NULL)
    {
        goto error;
    }

    (*obj)->malloc = malloc;
    (*obj)->realloc = realloc;
    (*obj)->free = free;
    r = 1;
error:
    return r;
}

void zylib_private_allocator_destruct(zylib_private_allocator_t **obj)
{
    if (*obj == NULL)
    {
        return;
    }

    (*obj)->free((void *)*obj);
    *obj = NULL;
}

_Bool zylib_private_allocator_malloc(const zylib_private_allocator_t *obj, size_t size, void **ptr)
{
    _Bool r = 0;

    if (size <= 0)
    {
        return 0;
    }

    *ptr = obj->malloc(size);
    if (*ptr == NULL)
    {
        goto error;
    }

    r = 1;
error:
    return r;
}

_Bool zylib_private_allocator_realloc(const zylib_private_allocator_t *obj, size_t size, void **ptr)
{
    _Bool r = 0;
    void *x_ptr = NULL;

    if (size <= 0 || *ptr == NULL)
    {
        return 0;
    }

    x_ptr = obj->realloc(*ptr, size);
    if (x_ptr == NULL)
    {
        goto error;
    }
    *ptr = x_ptr;

    r = 1;
error:
    return r;
}

void zylib_private_allocator_free(const zylib_private_allocator_t *obj, void **ptr)
{
    if (*ptr != NULL)
    {
        obj->free(*ptr);
        *ptr = NULL;
    }
}
