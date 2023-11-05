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
#include "zylib_dequeue.h"
#include "zylib_box.h"

typedef struct zylib_dequeue_box_s
{
    struct zylib_dequeue_box_s *previous, *next;
    zylib_box_t *box;
} zylib_dequeue_box_t;

struct zylib_dequeue_s
{
    const zylib_allocator_t *allocator;
    zylib_dequeue_box_t *first, *last;
    size_t size;
};

static inline void zylib_dequeue_box_destruct(zylib_dequeue_box_t **p_dequeue_box, const zylib_allocator_t *allocator);

static _Bool zylib_dequeue_box_construct(zylib_dequeue_box_t **p_dequeue_box, const zylib_allocator_t *allocator,
                                         size_t size, const void *p_void);

_Bool zylib_dequeue_box_construct(zylib_dequeue_box_t **const p_dequeue_box, const zylib_allocator_t *const allocator,
                                  size_t size, const void *p_void)
{
    _Bool r;

    if (p_dequeue_box == NULL || allocator == NULL || p_void == NULL)
    {
        return 0;
    }

    *p_dequeue_box = NULL;
    r = zylib_allocator_malloc(allocator, sizeof(zylib_dequeue_box_t), (void **)p_dequeue_box);
    if (!r)
    {
        goto error;
    }

    (*p_dequeue_box)->box = NULL;
    r = zylib_box_construct(&(*p_dequeue_box)->box, allocator, size, p_void);
    if (!r)
    {
        goto error;
    }

    (*p_dequeue_box)->previous = NULL;
    (*p_dequeue_box)->next = NULL;

    goto done;
error:
    zylib_dequeue_box_destruct(p_dequeue_box, allocator);
done:
    return r;
}

void zylib_dequeue_box_destruct(zylib_dequeue_box_t **const p_dequeue_box, const zylib_allocator_t *const allocator)
{
    if (p_dequeue_box == NULL || *p_dequeue_box == NULL || allocator == NULL)
    {
        return;
    }

    if ((*p_dequeue_box)->box != NULL)
    {
        zylib_box_destruct(&(*p_dequeue_box)->box);
    }
    zylib_allocator_free(allocator, (void **)p_dequeue_box);
}

_Bool zylib_dequeue_construct(zylib_dequeue_t **p_dequeue, const zylib_allocator_t *allocator)
{
    _Bool r;

    if (p_dequeue == NULL || allocator == NULL)
    {
        return 0;
    }

    *p_dequeue = NULL;
    r = zylib_allocator_malloc(allocator, sizeof(zylib_dequeue_t), (void **)p_dequeue);
    if (!r)
    {
        goto error;
    }

    (*p_dequeue)->allocator = allocator;
    (*p_dequeue)->first = NULL;
    (*p_dequeue)->last = NULL;
    (*p_dequeue)->size = 0;

    goto done;
error:
    if (*p_dequeue != NULL)
    {
        zylib_allocator_free(allocator, (void **)p_dequeue);
    }
done:
    return r;
}

void zylib_dequeue_destruct(zylib_dequeue_t **p_dequeue)
{
    if (p_dequeue != NULL && *p_dequeue != NULL)
    {
        zylib_dequeue_clear(*p_dequeue);
        zylib_allocator_free((*p_dequeue)->allocator, (void **)p_dequeue);
    }
}

void zylib_dequeue_clear(zylib_dequeue_t *dequeue)
{
    if (dequeue != NULL)
    {
        zylib_dequeue_box_t *p_dequeue_box = dequeue->first;
        while (p_dequeue_box != NULL)
        {
            zylib_dequeue_box_t *const p_next = p_dequeue_box->next;
            zylib_dequeue_box_destruct(&p_dequeue_box, dequeue->allocator);
            p_dequeue_box = p_next;
        }
        dequeue->first = NULL;
        dequeue->last = NULL;
        dequeue->size = 0;
    }
}

_Bool zylib_dequeue_push_first(zylib_dequeue_t *dequeue, uint64_t size, const void *p_void)
{
    _Bool r;
    zylib_dequeue_box_t *p_dequeue_box = NULL;

    if (dequeue == NULL || size <= 0 || p_void == NULL)
    {
        return 0;
    }

    r = zylib_dequeue_box_construct(&p_dequeue_box, dequeue->allocator, size, p_void);
    if (!r)
    {
        goto error;
    }

    if (dequeue->size != 0)
    {
        p_dequeue_box->next = dequeue->first;
        dequeue->first->previous = p_dequeue_box;
        dequeue->first = p_dequeue_box;
    }
    else
    {
        dequeue->first = p_dequeue_box;
        dequeue->last = p_dequeue_box;
    }
    ++dequeue->size;

error:
    return r;
}

_Bool zylib_dequeue_push_last(zylib_dequeue_t *dequeue, uint64_t size, const void *p_void)
{
    _Bool r;
    zylib_dequeue_box_t *p_dequeue_box;

    if (dequeue == NULL || size <= 0 || p_void == NULL)
    {
        return 0;
    }

    r = zylib_dequeue_box_construct(&p_dequeue_box, dequeue->allocator, size, p_void);
    if (!r)
    {
        goto error;
    }

    if (dequeue->size != 0)
    {
        p_dequeue_box->previous = dequeue->last;
        dequeue->last->next = p_dequeue_box;
        dequeue->last = p_dequeue_box;
    }
    else
    {
        dequeue->first = p_dequeue_box;
        dequeue->last = p_dequeue_box;
    }
    ++dequeue->size;

error:
    return r;
}

void zylib_dequeue_discard_first(zylib_dequeue_t *dequeue)
{
    if (dequeue != NULL && dequeue->size != 0)
    {
        zylib_dequeue_box_t *p_dequeue_box = dequeue->first;
        if (p_dequeue_box->next != NULL)
        {
            p_dequeue_box->next->previous = NULL;
            dequeue->first = p_dequeue_box->next;
        }
        else
        {
            dequeue->first = NULL;
            dequeue->last = NULL;
        }
        zylib_dequeue_box_destruct(&p_dequeue_box, dequeue->allocator);
        --dequeue->size;
    }
}

void zylib_dequeue_discard_last(zylib_dequeue_t *dequeue)
{
    if (dequeue != NULL && dequeue->size != 0)
    {
        zylib_dequeue_box_t *p_dequeue_box = dequeue->last;
        if (p_dequeue_box->previous != NULL)
        {
            p_dequeue_box->previous->next = NULL;
            dequeue->last = p_dequeue_box->previous;
        }
        else
        {
            dequeue->first = NULL;
            dequeue->last = NULL;
        }
        zylib_dequeue_box_destruct(&p_dequeue_box, dequeue->allocator);
        --dequeue->size;
    }
}

const void *zylib_dequeue_peek_first(const zylib_dequeue_t *dequeue, uint64_t *p_size)
{
    if (dequeue != NULL)
    {
        const void *data;
        if (p_size != NULL)
        {
            zylib_box_peek_size(dequeue->first->box, p_size);
        }
        zylib_box_peek_data(dequeue->first->box, &data);
        return data;
    }
    return NULL;
}

const void *zylib_dequeue_peek_last(const zylib_dequeue_t *dequeue, uint64_t *p_size)
{
    if (dequeue != NULL)
    {
        const void *data;
        if (p_size != NULL)
        {
            zylib_box_peek_size(dequeue->last->box, p_size);
        }
        zylib_box_peek_data(dequeue->last->box, &data);
        return data;
    }
    return NULL;
}

uint64_t zylib_dequeue_size(const zylib_dequeue_t *dequeue)
{
    size_t size = SIZE_MAX;
    if (dequeue != NULL)
    {
        size = dequeue->size;
    }
    return size;
}

_Bool zylib_dequeue_is_empty(const zylib_dequeue_t *dequeue)
{
    return dequeue->size == 0;
}
