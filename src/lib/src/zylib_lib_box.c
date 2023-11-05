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
#include "zylib_lib_box.h"
#include <string.h>

struct zylib_lib_box_s
{
    const zylib_allocator_t *allocator;
    uint64_t size;
    void *data;
};

static inline _Bool zylib_box_get_address_by_index_const(const zylib_lib_box_t *box, uint64_t index, uint64_t *size,
                                                         const void **data);

static inline _Bool zylib_box_get_address_by_index(const zylib_lib_box_t *box, uint64_t index, uint64_t *size,
                                                   void **data);

_Bool zylib_lib_box_construct(zylib_lib_box_t **box, const zylib_allocator_t *allocator, uint64_t size,
                              const void *data)
{
    _Bool r;

    if (box == NULL || allocator == NULL || size <= 0 || data == NULL)
    {
        return 0;
    }

    *box = NULL;
    r = zylib_allocator_malloc(allocator, sizeof(zylib_lib_box_t), (void **)box);
    if (!r)
    {
        goto error;
    }

    (*box)->allocator = allocator;
    (*box)->data = NULL;
    r = zylib_allocator_malloc(allocator, size, &(*box)->data);
    if (!r)
    {
        goto error;
    }

    (*box)->size = size;
    memcpy((*box)->data, data, size);

    goto done;
error:
    zylib_lib_box_destruct(box);
done:
    return r;
}

void zylib_lib_box_destruct(zylib_lib_box_t **box)
{
    if (box == NULL || *box == NULL)
    {
        return;
    }
    if ((*box)->data != NULL)
    {
        zylib_allocator_free((*box)->allocator, &(*box)->data);
    }
    zylib_allocator_free((*box)->allocator, (void **)box);
}

_Bool zylib_lib_box_append(zylib_lib_box_t **box, uint64_t size, const void *data)
{
    _Bool r = 0;

    uint64_t index;
    void *address = NULL;

    if (box == NULL || *box == NULL || data == NULL)
    {
        goto error;
    }

    r = zylib_allocator_realloc((*box)->allocator, sizeof(zylib_lib_box_t) + (*box)->size + size, &(*box)->data);
    if (!r)
    {
        goto error;
    }

    index = (*box)->size;
    (*box)->size += size;

    r = zylib_box_get_address_by_index(*box, index, NULL, &address);
    if (!r)
    {
        goto error;
    }

    memcpy(address, data, size);

error:
    return r;
}

_Bool zylib_lib_box_split_latter(const zylib_lib_box_t *box, uint64_t index, uint64_t *size, const void **data)
{
    return zylib_box_get_address_by_index_const(box, index, size, data);
}

_Bool zylib_lib_box_peek_size(const zylib_lib_box_t *box, uint64_t *size)
{
    if (box == NULL || size == NULL)
    {
        return 0;
    }
    *size = box->size;
    return 1;
}

_Bool zylib_lib_box_peek_data(const zylib_lib_box_t *box, const void **data)
{
    if (box == NULL || data == NULL)
    {
        return 0;
    }
    *data = box->data;
    return 1;
}

_Bool zylib_box_get_address_by_index(const zylib_lib_box_t *box, uint64_t index, uint64_t *size, void **data)
{
    if (box == NULL || index >= box->size || data == NULL)
    {
        return 0;
    }

    if (size != NULL)
    {
        *size = box->size - index;
    }

    *data = &((uint8_t *)box->data)[index];

    return 1;
}

_Bool zylib_box_get_address_by_index_const(const zylib_lib_box_t *box, uint64_t index, uint64_t *size,
                                           const void **data)
{
    if (box == NULL || index >= box->size || data == NULL)
    {
        return 0;
    }

    if (size != NULL)
    {
        *size = box->size - index;
    }

    *data = &((const uint8_t *)box->data)[index];

    return 1;
}
