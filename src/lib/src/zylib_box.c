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
    uint64_t size;
    void *data;
};

_Bool zylib_box_construct(zylib_box_t **p_box, const zylib_allocator_t *allocator, uint64_t size, const void *p_void)
{
    _Bool r;

    if (p_box == NULL || allocator == NULL || size <= 0 || p_void == NULL)
    {
        return 0;
    }

    *p_box = NULL;
    r = zylib_allocator_malloc(allocator, sizeof(zylib_box_t), (void **)p_box);
    if (!r)
    {
        goto error;
    }

    (*p_box)->data = NULL;
    r = zylib_allocator_malloc(allocator, size, &(*p_box)->data);
    if (!r)
    {
        goto error;
    }

    (*p_box)->size = size;
    memcpy((*p_box)->data, p_void, size);

    goto done;
error:
    zylib_box_destruct(p_box, allocator);
done:
    return r;
}

_Bool zylib_box_append(zylib_box_t **dest, const zylib_allocator_t *allocator, uint64_t size, const void *p_void)
{
    _Bool r = 0;

    if (dest == NULL || *dest == NULL || allocator == NULL || p_void == NULL)
    {
        goto error;
    }

    r = zylib_allocator_realloc(allocator, sizeof(zylib_box_t) + (*dest)->size + size, &(*dest)->data);
    if (!r)
    {
        goto error;
    }

    memcpy((uint8_t *)(*dest)->data + (*dest)->size, p_void, size);
    (*dest)->size += size;

error:
    return r;
}

const void *zylib_box_split_latter(const zylib_box_t *src, uint64_t index, uint64_t *size)
{
    if (src == NULL || index >= src->size)
    {
        return NULL;
    }

    if (size != NULL)
    {
        *size = src->size - index;
    }

    return (uint8_t *)src->data + index;
}

void zylib_box_destruct(zylib_box_t **p_box, const zylib_allocator_t *allocator)
{
    if (p_box == NULL || *p_box == NULL || allocator == NULL)
    {
        return;
    }

    if ((*p_box)->data != NULL)
    {
        zylib_allocator_free(allocator, &(*p_box)->data);
    }
    zylib_allocator_free(allocator, (void **)p_box);
}

uint64_t zylib_box_peek_size(const zylib_box_t *box)
{
    return box->size;
}

const void *zylib_box_peek_data(const zylib_box_t *box)
{
    return box->data;
}
