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
#include "zylib_error.h"
#include <string.h>

/*
 * Types
 */

struct zylib_err_s
{
    const zylib_alloc_t *alloc;
    zylib_dequeue_t *dequeue;
};

struct zylib_err_bx_s
{
    int64_t code;
    size_t line;
    const char *file;
    const char *function;
    zylib_opaque_t opaque;
};

/*
 * Functions
 */

int zylib_err_construct(zylib_err_t **err, const zylib_alloc_t *alloc)
{
    int r = zylib_malloc(alloc, sizeof(zylib_err_t), (void **)err);

    if (r == ZYLIB_OK)
    {
        zylib_dequeue_t *dequeue;
        r = zylib_dequeue_construct(&dequeue, alloc);
        if (r == ZYLIB_OK)
        {
            (*err)->alloc = alloc;
            (*err)->dequeue = dequeue;
        }
        else
        {
            zylib_free(alloc, (void **)err);
        }
    }
    return r;
}

void zylib_err_destruct(zylib_err_t **err)
{
    if (*err != nullptr)
    {
        zylib_dequeue_destruct(&(*err)->dequeue);
        zylib_free((*err)->alloc, (void **)err);
    }
}

void zylib_err_clear(zylib_err_t *err)
{
    zylib_dequeue_clear(err->dequeue);
}

int zylib_err_push_first(zylib_err_t *err, int64_t code, const char *file, size_t line, const char *function,
                      const void *opaque, size_t opaque_size)
{
    void *data;
    int r = zylib_malloc(err->alloc, sizeof(zylib_opaque_t) + sizeof(zylib_err_bx_t) + opaque_size, &data);
    *(size_t *)(data + offsetof(zylib_opaque_t, size)) = sizeof(zylib_opaque_t) + sizeof(zylib_err_bx_t) + opaque_size;
    *(int64_t *)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, code)) = code;
    *(size_t *)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, line)) = line;
    *(const char **)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, file)) = file;
    *(const char **)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, function)) = function;
    *(size_t *)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, opaque) + offsetof(zylib_opaque_t, size)) =
        opaque_size;
    memcpy(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, opaque) + offsetof(zylib_opaque_t, data), opaque,
           opaque_size);
    zylib_dequeue_push_first(err->dequeue, (const zylib_opaque_t *)data);
    zylib_free(err->alloc, &data);
    return r;
}

int zylib_err_push_last(zylib_err_t *err, int64_t code, const char *file, size_t line, const char *function,
                     const void *opaque, size_t opaque_size)
{
    void *data;
    int r = zylib_malloc(err->alloc, sizeof(zylib_opaque_t) + sizeof(zylib_err_bx_t) + opaque_size, &data);
    *(size_t *)(data + offsetof(zylib_opaque_t, size)) = sizeof(zylib_opaque_t) + sizeof(zylib_err_bx_t) + opaque_size;
    *(int64_t *)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, code)) = code;
    *(size_t *)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, line)) = line;
    *(const char **)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, file)) = file;
    *(const char **)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, function)) = function;
    *(size_t *)(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, opaque) + offsetof(zylib_opaque_t, size)) =
        opaque_size;
    memcpy(data + offsetof(zylib_opaque_t, data) + offsetof(zylib_err_bx_t, opaque) + offsetof(zylib_opaque_t, data), opaque,
           opaque_size);
    zylib_dequeue_push_last(err->dequeue, (const zylib_opaque_t *)data);
    zylib_free(err->alloc, &data);
    return r;
}

void zylib_err_discard_first(zylib_err_t *err)
{
    zylib_dequeue_discard_first(err->dequeue);
}

void zylib_err_discard_last(zylib_err_t *err)
{
    zylib_dequeue_discard_last(err->dequeue);
}

zylib_err_bx_t *zylib_err_peek_first(const zylib_err_t *err)
{
    return (zylib_err_bx_t *)(&((zylib_opaque_t *)zylib_dequeue_peek_first(err->dequeue))->data);
}

zylib_err_bx_t *zylib_err_peek_last(const zylib_err_t *err)
{
    return (zylib_err_bx_t *)(&((zylib_opaque_t *)zylib_dequeue_peek_last(err->dequeue))->data);
}

size_t zylib_err_size(const zylib_err_t *err)
{
    return zylib_dequeue_size(err->dequeue);
}

bool zylib_err_is_empty(const zylib_err_t *err)
{
    return zylib_dequeue_is_empty(err->dequeue);
}

int64_t zylib_err_bx_code(const zylib_err_bx_t *bx)
{
    return bx->code;
}

const char *zylib_err_bx_file(const zylib_err_bx_t *bx)
{
    return bx->file;
}

size_t zylib_err_bx_line(const zylib_err_bx_t *bx)
{
    return bx->line;
}

const char *zylib_err_bx_function(const zylib_err_bx_t *bx)
{
    return bx->function;
}

const void *zylib_err_bx_opaque(const zylib_err_bx_t *bx, size_t *size)
{
    if (size != nullptr)
    {
        *size = bx->opaque.size;
    }
    return &bx->opaque.data;
}