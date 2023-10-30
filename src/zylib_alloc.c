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
#include "zylib_alloc.h"

/*
 * Types
 */

struct zylib_alloc_s
{
    zylib_malloc_t malloc;
    zylib_realloc_t realloc;
    zylib_free_t free;
};

/*
 * Functions
 */

zylib_return_t zylib_alloc_construct(zylib_alloc_t **alloc, zylib_malloc_t malloc, zylib_realloc_t realloc, zylib_free_t free)
{
    *alloc = malloc(sizeof(zylib_alloc_t));
    if (*alloc != nullptr)
    {
        (*alloc)->malloc = malloc;
        (*alloc)->realloc = realloc;
        (*alloc)->free = free;
        return ZYLIB_OK;
    }
    return ZYLIB_ERROR_OOM;
}

void zylib_alloc_destruct(zylib_alloc_t **alloc)
{
    if (*alloc != nullptr)
    {
        const zylib_free_t free = (*alloc)->free;
        free((void *)*alloc);
        *alloc = nullptr;
    }
}

zylib_return_t zylib_malloc(const zylib_alloc_t *alloc, size_t size, void **ptr)
{
    *ptr = alloc->malloc(size);
    if (*ptr != nullptr)
    {
        return ZYLIB_OK;
    }
    return ZYLIB_ERROR_OOM;
}

zylib_return_t zylib_realloc(const zylib_alloc_t *alloc, size_t size, void **ptr)
{
    void *x_ptr = alloc->realloc(*ptr, size);
    if (x_ptr != nullptr)
    {
        *ptr = x_ptr;
        return ZYLIB_OK;
    }
    return ZYLIB_ERROR_OOM;
}

void zylib_free(const zylib_alloc_t *alloc, void **ptr)
{
    alloc->free(*ptr);
    *ptr = nullptr;
}