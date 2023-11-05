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
#include "zylib_box.h"
#include <string.h>

struct zylib_box_s
{
    const zylib_allocator_t *allocator;
    uint64_t size;
    void *data;
};

static inline _Bool zylib_box_get_address_by_index_const(const zylib_box_t *box, uint64_t index, uint64_t *p_size,
                                                         const void **p_void);

static inline _Bool zylib_box_get_address_by_index(const zylib_box_t *box, uint64_t index, uint64_t *p_size,
                                                   void **p_void);

_Bool zylib_box_construct(zylib_box_t **p_box, const zylib_allocator_t *allocator, uint64_t size, const void *p_data)
{
    _Bool r;

    if (p_box == NULL || allocator == NULL || size <= 0 || p_data == NULL)
    {
        return 0;
    }

    *p_box = NULL;
    r = zylib_allocator_malloc(allocator, sizeof(zylib_box_t), (void **)p_box);
    if (!r)
    {
        goto error;
    }

    (*p_box)->allocator = allocator;
    (*p_box)->data = NULL;
    r = zylib_allocator_malloc(allocator, size, &(*p_box)->data);
    if (!r)
    {
        goto error;
    }

    (*p_box)->size = size;
    memcpy((*p_box)->data, p_data, size);

    goto done;
error:
    zylib_box_destruct(p_box);
done:
    return r;
}

void zylib_box_destruct(zylib_box_t **p_box)
{
    if (p_box == NULL || *p_box == NULL)
    {
        return;
    }
    if ((*p_box)->data != NULL)
    {
        zylib_allocator_free((*p_box)->allocator, &(*p_box)->data);
    }
    zylib_allocator_free((*p_box)->allocator, (void **)p_box);
}

_Bool zylib_box_append(zylib_box_t **dest, uint64_t size, const void *p_data)
{
    _Bool r = 0;

    uint64_t index;
    void *address = NULL;

    if (dest == NULL || *dest == NULL || p_data == NULL)
    {
        goto error;
    }

    r = zylib_allocator_realloc((*dest)->allocator, sizeof(zylib_box_t) + (*dest)->size + size, &(*dest)->data);
    if (!r)
    {
        goto error;
    }

    index = (*dest)->size;
    (*dest)->size += size;

    r = zylib_box_get_address_by_index(*dest, index, NULL, &address);
    if (!r)
    {
        goto error;
    }

    memcpy(address, p_data, size);

error:
    return r;
}

_Bool zylib_box_split_latter(const zylib_box_t *box, uint64_t index, uint64_t *p_size, const void **p_data)
{
    return zylib_box_get_address_by_index_const(box, index, p_size, p_data);
}

_Bool zylib_box_peek_size(const zylib_box_t *box, uint64_t *p_size)
{
    if (box == NULL || p_size == NULL)
    {
        return 0;
    }
    *p_size = box->size;
    return 1;
}

_Bool zylib_box_peek_data(const zylib_box_t *box, const void **p_data)
{
    if (box == NULL || p_data == NULL)
    {
        return 0;
    }
    *p_data = box->data;
    return 1;
}

_Bool zylib_box_get_address_by_index(const zylib_box_t *box, uint64_t index, uint64_t *p_size, void **p_void)
{
    if (box == NULL || index >= box->size || p_void == NULL)
    {
        return 0;
    }

    if (p_size != NULL)
    {
        *p_size = box->size - index;
    }

    *p_void = &((uint8_t *)box->data)[index];

    return 1;
}

_Bool zylib_box_get_address_by_index_const(const zylib_box_t *box, uint64_t index, uint64_t *p_size,
                                           const void **p_void)
{
    if (box == NULL || index >= box->size || p_void == NULL)
    {
        return 0;
    }

    if (p_size != NULL)
    {
        *p_size = box->size - index;
    }

    *p_void = &((const uint8_t *)box->data)[index];

    return 1;
}
