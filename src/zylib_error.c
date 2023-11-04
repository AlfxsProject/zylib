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
#include "zylib_error.h"
#include "zylib_dequeue.h"
#include <string.h>

/*
 * Types
 */

struct zylib_error_s
{
    const zylib_allocator_t *alloc;
    zylib_dequeue_t *dequeue;
};

struct zylib_error_box_s
{
    int64_t code;
    const char *file;
    uint64_t line;
    const char *function;
    zylib_box_t box;
};

/*
 * Functions
 */

static _Bool zylib_error_push(zylib_error_t *err, int64_t code, const char *file, uint64_t line, const char *function,
                              const zylib_box_t *box, _Bool (*push)(zylib_dequeue_t *dqe, const zylib_box_t *box))
{
    _Bool r = 0; // ZYLIB_ERROR_INPUT_VALUE;
    if (err != NULL && file != NULL && function != NULL)
    {
        struct buf_s
        {
            uint64_t size;
            zylib_error_box_t bx;
        } *buf;

        const uint64_t box_size = box == NULL ? 0 : box->size;
        const uint64_t total_size = sizeof(struct buf_s) + sizeof(zylib_box_t) + box_size;

        r = zylib_allocator_malloc(err->alloc, total_size, (void **)&buf);
        if (r) // ZYLIB_OK
        {
            buf->size = sizeof(zylib_error_box_t) + sizeof(*box) + box_size;
            buf->bx.code = code;
            buf->bx.file = file;
            buf->bx.line = line;
            buf->bx.function = function;
            buf->bx.box.size = box_size;
            memcpy(buf->bx.box.data, box->data, box_size);
            r = push(err->dequeue, (const zylib_box_t *)buf);
            zylib_allocator_free(err->alloc, (void **)&buf);
        }
    }
    return r;
}

_Bool zylib_error_construct(zylib_error_t **err, const zylib_allocator_t *alloc)
{
    _Bool r = 0; // ZYLIB_ERROR_INPUT_VALUE;
    if (err != NULL && alloc != NULL)
    {
        zylib_dequeue_t *dequeue = NULL;
        r = zylib_allocator_malloc(alloc, sizeof(zylib_error_t), (void **)err) &&
            zylib_dequeue_construct(&dequeue, alloc);
        if (r) // ZYLIB_OK
        {
            (*err)->alloc = alloc;
            (*err)->dequeue = dequeue;
        }
    }
    return r;
}

void zylib_error_destruct(zylib_error_t **err)
{
    if (err != NULL && *err != NULL)
    {
        zylib_dequeue_destruct(&(*err)->dequeue);
        zylib_allocator_free((*err)->alloc, (void **)err);
    }
}

void zylib_error_clear(zylib_error_t *err)
{
    zylib_dequeue_clear(err->dequeue);
}

_Bool zylib_error_push_first(zylib_error_t *err, int64_t code, const char *file, uint64_t line, const char *function,
                             const zylib_box_t *box)
{
    return zylib_error_push(err, code, file, line, function, box, zylib_dequeue_push_first);
}

_Bool zylib_error_push_last(zylib_error_t *err, int64_t code, const char *file, uint64_t line, const char *function,
                            const zylib_box_t *box)
{
    return zylib_error_push(err, code, file, line, function, box, zylib_dequeue_push_last);
}

void zylib_error_discard_first(zylib_error_t *err)
{
    zylib_dequeue_discard_first(err->dequeue);
}

void zylib_error_discard_last(zylib_error_t *err)
{
    zylib_dequeue_discard_last(err->dequeue);
}

const zylib_error_box_t *zylib_error_peek_first(const zylib_error_t *err)
{
    return (const zylib_error_box_t *)zylib_dequeue_peek_first(err->dequeue)->data;
}

const zylib_error_box_t *zylib_error_peek_last(const zylib_error_t *err)
{
    return (const zylib_error_box_t *)zylib_dequeue_peek_last(err->dequeue)->data;
}

uint64_t zylib_error_size(const zylib_error_t *err)
{
    return zylib_dequeue_size(err->dequeue);
}

_Bool zylib_error_is_empty(const zylib_error_t *err)
{
    return zylib_dequeue_is_empty(err->dequeue);
}

int64_t zylib_error_box_peek_code(const zylib_error_box_t *bx)
{
    return bx->code;
}

const char *zylib_error_box_peek_file(const zylib_error_box_t *bx)
{
    return bx->file;
}

uint64_t zylib_error_box_peek_line_number(const zylib_error_box_t *bx)
{
    return bx->line;
}

const char *zylib_error_box_peek_function(const zylib_error_box_t *bx)
{
    return bx->function;
}

const void *zylib_error_box_peek_opaque(const zylib_error_box_t *bx, uint64_t *size)
{
    if (size != NULL)
    {
        *size = bx->box.size;
    }
    return bx->box.data;
}
