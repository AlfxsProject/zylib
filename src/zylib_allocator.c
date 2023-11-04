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
    if (allocator != NULL)
    {
        *allocator = malloc(sizeof(zylib_allocator_t));
        if (*allocator != NULL)
        {
            (*allocator)->malloc = malloc;
            (*allocator)->realloc = realloc;
            (*allocator)->free = free;
            return 1; // ZYLIB_OK
        }
        return 0; // ZYLIB_ERROR_OOM;
    }
    return 0; // ZYLIB_ERROR_INPUT_VALUE;
}

void zylib_allocator_destruct(zylib_allocator_t **allocator)
{
    if (allocator != NULL && *allocator != NULL)
    {
        const zylib_allocator_free_t free = (*allocator)->free;
        free((void *)*allocator);
        *allocator = NULL;
    }
}

_Bool zylib_allocator_malloc(const zylib_allocator_t *allocator, size_t size, void **p_void)
{
    if (allocator != NULL && p_void != NULL)
    {
        *p_void = allocator->malloc(size);
        if (*p_void != NULL)
        {
            return 1; // ZYLIB_OK;
        }
        return 0; // ZYLIB_ERROR_OOM;
    }
    return 0; // ZYLIB_ERROR_INPUT_VALUE;
}

_Bool zylib_allocator_realloc(const zylib_allocator_t *allocator, size_t size, void **p_void)
{
    if (allocator != NULL && p_void != NULL)
    {
        void *x_ptr = allocator->realloc(*p_void, size);
        if (x_ptr != NULL)
        {
            *p_void = x_ptr;
            return 1; // ZYLIB_OK;
        }
        return 0; // ZYLIB_ERROR_OOM;
    }
    return 0; // ZYLIB_ERROR_INPUT_VALUE;
}

void zylib_allocator_free(const zylib_allocator_t *allocator, void **p_void)
{
    if (allocator != NULL && p_void != NULL)
    {
        allocator->free(*p_void);
        *p_void = NULL;
    }
}
