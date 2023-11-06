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
#include "zylib_private_allocator.h"

/*
 * Functions
 */

_Bool zylib_allocator_construct(zylib_allocator_t **allocator, zylib_allocator_malloc_t malloc,
                                zylib_allocator_realloc_t realloc, zylib_allocator_free_t free)
{
    return zylib_private_allocator_construct((zylib_private_allocator_t **)allocator, malloc, realloc, free);
}

void zylib_allocator_destruct(zylib_allocator_t **allocator)
{
    zylib_private_allocator_destruct((zylib_private_allocator_t **)allocator);
}

_Bool zylib_allocator_malloc(const zylib_allocator_t *allocator, size_t size, void **data)
{
    return zylib_private_allocator_malloc((const zylib_private_allocator_t *)allocator, size, data);
}

_Bool zylib_allocator_realloc(const zylib_allocator_t *allocator, size_t size, void **data)
{
    return zylib_private_allocator_realloc((const zylib_private_allocator_t *)allocator, size, data);
}

void zylib_allocator_free(const zylib_allocator_t *allocator, void **data)
{
    zylib_private_allocator_free((const zylib_private_allocator_t *)allocator, data);
}
