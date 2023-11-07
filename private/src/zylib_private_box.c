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
#include "zylib_private_box.h"
#include <string.h>

struct zylib_private_box_s
{
    const zylib_private_allocator_t *allocator;
    uint64_t size;
    void *data;
};

ZYLIB_NONNULL
static inline _Bool zylib_box_get_address_by_index_const(const zylib_private_box_t *box, uint64_t index, uint64_t *size,
                                                         const void **ptr);

ZYLIB_NONNULL
static inline _Bool zylib_box_get_address_by_index(const zylib_private_box_t *box, uint64_t index, uint64_t *size,
                                                   void **ptr);

_Bool zylib_private_box_construct(zylib_private_box_t **obj, const zylib_private_allocator_t *allocator, uint64_t size,
                                  const void *ptr)
{
    _Bool r;

    if (size <= 0)
    {
        return 0;
    }

    *obj = NULL;
    r = zylib_private_allocator_malloc(allocator, sizeof(zylib_private_box_t), (void **)obj);
    if (!r)
    {
        goto error;
    }

    (*obj)->allocator = allocator;
    (*obj)->data = NULL;
    r = zylib_private_allocator_malloc(allocator, size, &(*obj)->data);
    if (!r)
    {
        goto error;
    }

    (*obj)->size = size;
    memcpy((*obj)->data, ptr, size);

    goto done;
error:
    zylib_private_box_destruct(obj);
done:
    return r;
}

void zylib_private_box_destruct(zylib_private_box_t **obj)
{
    if (*obj != NULL)
    {
        if ((*obj)->data != NULL)
        {
            zylib_private_allocator_free((*obj)->allocator, &(*obj)->data);
        }
        zylib_private_allocator_free((*obj)->allocator, (void **)obj);
    }
}

_Bool zylib_private_box_append(zylib_private_box_t *obj, uint64_t size, const void *ptr)
{
    _Bool r;

    uint64_t offset;
    uint64_t index;
    void *address = NULL;

    r = zylib_private_allocator_realloc(obj->allocator, sizeof(zylib_private_box_t) + obj->size + size, &obj->data);
    if (!r)
    {
        goto error;
    }

    index = obj->size;
    obj->size += size;

    r = zylib_box_get_address_by_index(obj, index, &offset, &address);
    if (!r)
    {
        goto error;
    }

    memcpy(address, ptr, size);

error:
    return r;
}

_Bool zylib_private_box_split_by_index(const zylib_private_box_t *obj, uint64_t index, uint64_t *size, const void **ptr)
{
    return zylib_box_get_address_by_index_const(obj, index, size, ptr);
}

uint64_t zylib_private_box_peek_size(const zylib_private_box_t *obj)
{
    return obj->size;
}

const void *zylib_private_box_peek_data(const zylib_private_box_t *obj)
{
    return obj->data;
}

_Bool zylib_box_get_address_by_index(const zylib_private_box_t *box, uint64_t index, uint64_t *size, void **ptr)
{
    return zylib_box_get_address_by_index_const(box, index, size, (const void **)ptr);
}

_Bool zylib_box_get_address_by_index_const(const zylib_private_box_t *box, uint64_t index, uint64_t *size,
                                           const void **ptr)
{
    if (index >= box->size)
    {
        return 0;
    }

    *size = box->size - index;
    *ptr = &((const unsigned char *)box->data)[index];

    return 1;
}
