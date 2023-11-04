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
#include <string.h>

typedef struct zylib_dequeue_bx_s
{
    const zylib_alloc_t *alloc;
    struct zylib_dequeue_bx_s *previous, *next;
    zylib_box_t box;
} zylib_dequeue_box_t;

struct zylib_dequeue_s
{
    const zylib_alloc_t *alloc;
    zylib_dequeue_box_t *first, *last;
    size_t size;
};

__attribute__((nonnull)) static zylib_return_t zylib_dequeue_bx_construct(zylib_dequeue_box_t **bx,
                                                                          const zylib_alloc_t *alloc,
                                                                          const zylib_box_t *box)
{
    zylib_return_t r = zylib_malloc(alloc, sizeof(zylib_dequeue_box_t) + box->size, (void **)bx);
    if (r == ZYLIB_OK)
    {
        (*bx)->alloc = alloc;
        (*bx)->previous = NULL;
        (*bx)->next = NULL;
        memcpy(&(*bx)->box, box, sizeof(zylib_box_t) + box->size);
    }
    return r;
}

__attribute__((nonnull)) static void zylib_dequeue_bx_destruct(zylib_dequeue_box_t **bx)
{
    if (*bx != NULL)
    {
        zylib_free((*bx)->alloc, (void **)bx);
    }
}

zylib_return_t zylib_dequeue_construct(zylib_dequeue_t **dqe, const zylib_alloc_t *alloc)
{
    zylib_return_t r = zylib_malloc(alloc, sizeof(zylib_dequeue_t), (void **)dqe);
    if (r == ZYLIB_OK)
    {
        (*dqe)->alloc = alloc;
        (*dqe)->first = NULL;
        (*dqe)->last = NULL;
        (*dqe)->size = 0;
    }
    return r;
}

void zylib_dequeue_destruct(zylib_dequeue_t **dqe)
{
    if (*dqe != NULL)
    {
        zylib_dequeue_clear(*dqe);
        zylib_free((*dqe)->alloc, (void **)dqe);
    }
}

void zylib_dequeue_clear(zylib_dequeue_t *dqe)
{
    zylib_dequeue_box_t *bx = dqe->first;
    while (bx != NULL)
    {
        zylib_dequeue_box_t *next = bx->next;
        zylib_dequeue_bx_destruct(&bx);
        bx = next;
    }
    dqe->first = NULL;
    dqe->last = NULL;
    dqe->size = 0;
}

zylib_return_t zylib_dequeue_push_first(zylib_dequeue_t *dqe, const zylib_box_t *box)
{
    zylib_dequeue_box_t *bx;
    zylib_return_t r = zylib_dequeue_bx_construct(&bx, dqe->alloc, box);
    if (r == ZYLIB_OK)
    {
        if (dqe->size != 0)
        {
            bx->next = dqe->first;
            dqe->first->previous = bx;
            dqe->first = bx;
        }
        else
        {
            dqe->first = bx;
            dqe->last = bx;
        }
        ++dqe->size;
    }
    return r;
}

zylib_return_t zylib_dequeue_push_last(zylib_dequeue_t *dqe, const zylib_box_t *box)
{
    zylib_dequeue_box_t *bx;
    zylib_return_t r = zylib_dequeue_bx_construct(&bx, dqe->alloc, box);
    if (r == ZYLIB_OK)
    {
        if (dqe->size != 0)
        {
            bx->previous = dqe->last;
            dqe->last->next = bx;
            dqe->last = bx;
        }
        else
        {
            dqe->first = bx;
            dqe->last = bx;
        }
        ++dqe->size;
    }
    return r;
}

void zylib_dequeue_discard_first(zylib_dequeue_t *dqe)
{
    if (dqe->size != 0)
    {
        zylib_dequeue_box_t *bx = dqe->first;
        if (bx->next != NULL)
        {
            bx->next->previous = NULL;
            dqe->first = bx->next;
        }
        else
        {
            dqe->first = NULL;
            dqe->last = NULL;
        }
        zylib_dequeue_bx_destruct(&bx);
        --dqe->size;
    }
}

void zylib_dequeue_discard_last(zylib_dequeue_t *dqe)
{
    if (dqe->size != 0)
    {
        zylib_dequeue_box_t *bx = dqe->last;
        if (bx->previous != NULL)
        {
            bx->previous->next = NULL;
            dqe->last = bx->previous;
        }
        else
        {
            dqe->first = NULL;
            dqe->last = NULL;
        }
        zylib_dequeue_bx_destruct(&bx);
        --dqe->size;
    }
}

const zylib_box_t *zylib_dequeue_peek_first(const zylib_dequeue_t *dqe)
{
    return (const zylib_box_t *)&dqe->first->box;
}

const zylib_box_t *zylib_dequeue_peek_last(const zylib_dequeue_t *dqe)
{
    return (const zylib_box_t *)&dqe->last->box;
}

size_t zylib_dequeue_size(const zylib_dequeue_t *dqe)
{
    return dqe->size;
}

_Bool zylib_dequeue_is_empty(const zylib_dequeue_t *dqe)
{
    return dqe->size == 0;
}
