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
#include "zylib_private_dequeue.h"
#include "zylib_private_box.h"

/*
 * Type Definitions
 */

typedef struct zylib_private_dequeue_box_s
{
    struct zylib_private_dequeue_box_s *previous, *next;
    zylib_private_box_t *box;
} zylib_private_dequeue_box_t;

struct zylib_private_dequeue_s
{
    const zylib_private_allocator_t *allocator;
    zylib_private_dequeue_box_t *first, *last;
    size_t size;
};

/*
 * Static Function Definitions
 */

ZYLIB_NONNULL
static inline void zylib_private_dequeue_box_destruct(zylib_private_dequeue_box_t **const obj,
                                                      const zylib_private_allocator_t *const allocator)
{
    if (*obj != NULL)
    {
        if ((*obj)->box != NULL)
        {
            zylib_private_box_destruct(&(*obj)->box);
        }
        zylib_private_allocator_free(allocator, (void **)obj);
    }
}

ZYLIB_NONNULL
static _Bool zylib_private_dequeue_box_construct(zylib_private_dequeue_box_t **const obj,
                                                 const zylib_private_allocator_t *const allocator, size_t size,
                                                 const void *data)
{
    _Bool r;

    *obj = NULL;
    r = zylib_private_allocator_malloc(allocator, sizeof(zylib_private_dequeue_box_t), (void **)obj);
    if (!r)
    {
        goto error;
    }

    (*obj)->box = NULL;
    r = zylib_private_box_construct(&(*obj)->box, allocator, size, data);
    if (!r)
    {
        goto error;
    }

    (*obj)->previous = NULL;
    (*obj)->next = NULL;

    goto done;
error:
    zylib_private_dequeue_box_destruct(obj, allocator);
done:
    return r;
}

/*
 * Function Definitions
 */

_Bool zylib_private_dequeue_construct(zylib_private_dequeue_t **obj, const zylib_private_allocator_t *allocator)
{
    _Bool r;

    *obj = NULL;
    r = zylib_private_allocator_malloc(allocator, sizeof(zylib_private_dequeue_t), (void **)obj);
    if (!r)
    {
        goto error;
    }

    (*obj)->allocator = allocator;
    (*obj)->first = NULL;
    (*obj)->last = NULL;
    (*obj)->size = 0;

    goto done;
error:
    if (*obj != NULL)
    {
        zylib_private_allocator_free(allocator, (void **)obj);
    }
done:
    return r;
}

void zylib_private_dequeue_destruct(zylib_private_dequeue_t **obj)
{
    if (*obj != NULL)
    {
        zylib_private_dequeue_clear(*obj);
        zylib_private_allocator_free((*obj)->allocator, (void **)obj);
    }
}

void zylib_private_dequeue_clear(zylib_private_dequeue_t *obj)
{

    zylib_private_dequeue_box_t *box = obj->first;
    while (box != NULL)
    {
        zylib_private_dequeue_box_t *const next = box->next;
        zylib_private_dequeue_box_destruct(&box, obj->allocator);
        box = next;
    }
    obj->first = NULL;
    obj->last = NULL;
    obj->size = 0;
}

_Bool zylib_private_dequeue_push_first(zylib_private_dequeue_t *obj, uint64_t size, const void *data)
{
    _Bool r;
    zylib_private_dequeue_box_t *box = NULL;

    if (size <= 0)
    {
        return 0;
    }

    r = zylib_private_dequeue_box_construct(&box, obj->allocator, size, data);
    if (!r)
    {
        goto error;
    }

    if (!zylib_private_dequeue_is_empty(obj))
    {
        box->next = obj->first;
        obj->first->previous = box;
        obj->first = box;
    }
    else
    {
        obj->first = box;
        obj->last = box;
    }
    ++obj->size;

error:
    return r;
}

_Bool zylib_private_dequeue_push_last(zylib_private_dequeue_t *obj, uint64_t size, const void *data)
{
    _Bool r;
    zylib_private_dequeue_box_t *box;

    if (size <= 0)
    {
        return 0;
    }

    r = zylib_private_dequeue_box_construct(&box, obj->allocator, size, data);
    if (!r)
    {
        goto error;
    }

    if (!zylib_private_dequeue_is_empty(obj))
    {
        box->previous = obj->last;
        obj->last->next = box;
        obj->last = box;
    }
    else
    {
        obj->first = box;
        obj->last = box;
    }
    ++obj->size;

error:
    return r;
}

void zylib_private_dequeue_discard_first(zylib_private_dequeue_t *obj)
{
    if (!zylib_private_dequeue_is_empty(obj))
    {
        zylib_private_dequeue_box_t *box = obj->first;
        if (box->next != NULL)
        {
            box->next->previous = NULL;
            obj->first = box->next;
        }
        else
        {
            obj->first = NULL;
            obj->last = NULL;
        }
        zylib_private_dequeue_box_destruct(&box, obj->allocator);
        --obj->size;
    }
}

void zylib_private_dequeue_discard_last(zylib_private_dequeue_t *obj)
{
    if (!zylib_private_dequeue_is_empty(obj))
    {
        zylib_private_dequeue_box_t *box = obj->last;
        if (box->previous != NULL)
        {
            box->previous->next = NULL;
            obj->last = box->previous;
        }
        else
        {
            obj->first = NULL;
            obj->last = NULL;
        }
        zylib_private_dequeue_box_destruct(&box, obj->allocator);
        --obj->size;
    }
}

_Bool zylib_private_dequeue_peek_first(const zylib_private_dequeue_t *obj, uint64_t *size, const void **data)
{
    if (!zylib_private_dequeue_is_empty(obj))
    {
        *size = zylib_private_box_peek_size(obj->first->box);
        *data = zylib_private_box_peek_data(obj->first->box);
        return 1;
    }
    *size = 0;
    *data = NULL;
    return 0;
}

_Bool zylib_private_dequeue_peek_last(const zylib_private_dequeue_t *obj, uint64_t *size, const void **data)
{
    if (!zylib_private_dequeue_is_empty(obj))
    {
        *size = zylib_private_box_peek_size(obj->last->box);
        *data = zylib_private_box_peek_data(obj->last->box);
        return 1;
    }
    *size = 0;
    *data = NULL;
    return 0;
}

uint64_t zylib_private_dequeue_size(const zylib_private_dequeue_t *obj)
{
    return obj->size;
}

_Bool zylib_private_dequeue_is_empty(const zylib_private_dequeue_t *obj)
{
    return obj->size == 0;
}
