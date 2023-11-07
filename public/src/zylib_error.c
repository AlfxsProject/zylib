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
#include "zylib_private_box.h"
#include "zylib_private_dequeue.h"

/*
 * Types
 */

struct zylib_error_s
{
    const zylib_allocator_t *allocator;
    zylib_private_dequeue_t *dequeue;
};

struct zylib_error_box_s
{
    size_t size;
    int64_t code;
    const char *file;
    uint64_t line;
    const char *function;
};

/*
 * Functions
 */

static _Bool zylib_error_push(zylib_error_t *error, int64_t code, const char *file, uint64_t line, const char *function,
                              size_t size, const void *data,
                              _Bool (*push)(zylib_private_dequeue_t *, size_t, const void *))
{
    _Bool r = 0;
    zylib_private_box_t *box = NULL;
    const zylib_error_box_t error_box = {.size = size, .code = code, .file = file, .line = line, .function = function};

    uint64_t i;
    const void *s;

    if (error == NULL || file == NULL || function == NULL)
    {
        goto error;
    }

    r = zylib_private_box_construct(&box, (const zylib_private_allocator_t *)error->allocator,
                                    sizeof(zylib_error_box_t), &error_box);
    if (!r)
    {
        goto error;
    }

    if (size > 0 && data != NULL)
    {
        r = zylib_private_box_append(&box, size, data);
        if (!r)
        {
            goto error;
        }
    }

    i = zylib_private_box_peek_size(box);
    s = zylib_private_box_peek_data(box);

    r = push(error->dequeue, i, s);
    if (!r)
    {
        goto error;
    }

error:
    if (box != NULL)
    {
        zylib_private_box_destruct(&box);
    }
    return r;
}

_Bool zylib_error_construct(zylib_error_t **obj, const zylib_allocator_t *allocator)
{
    _Bool r;

    *obj = NULL;
    r = zylib_allocator_malloc(allocator, sizeof(zylib_error_t), (void **)obj);
    if (!r)
    {
        goto error;
    }

    r = zylib_private_dequeue_construct(&(*obj)->dequeue, (const zylib_private_allocator_t *)allocator);
    if (!r)
    {
        goto error;
    }

    (*obj)->allocator = allocator;

    goto done;
error:
    if (*obj != NULL)
    {
        if ((*obj)->dequeue != NULL)
        {
            zylib_private_dequeue_destruct(&(*obj)->dequeue);
        }
        zylib_allocator_free(allocator, (void **)obj);
    }
done:
    return r;
}

void zylib_error_destruct(zylib_error_t **obj)
{
    if (*obj != NULL)
    {
        zylib_private_dequeue_destruct(&(*obj)->dequeue);
        zylib_allocator_free((*obj)->allocator, (void **)obj);
    }
}

void zylib_error_clear(zylib_error_t *obj)
{
    zylib_private_dequeue_clear(obj->dequeue);
}

_Bool zylib_error_push_first(zylib_error_t *obj, int64_t code, const char *file, uint64_t line, const char *function,
                             uint64_t size, const void *p_void)
{
    return zylib_error_push(obj, code, file, line, function, size, p_void, zylib_private_dequeue_push_first);
}

_Bool zylib_error_push_last(zylib_error_t *obj, int64_t code, const char *file, uint64_t line, const char *function,
                            uint64_t size, const void *p_void)
{
    return zylib_error_push(obj, code, file, line, function, size, p_void, zylib_private_dequeue_push_last);
}

void zylib_error_discard_first(zylib_error_t *obj)
{
    zylib_private_dequeue_discard_first(obj->dequeue);
}

void zylib_error_discard_last(zylib_error_t *obj)
{
    zylib_private_dequeue_discard_last(obj->dequeue);
}

const zylib_error_box_t *zylib_error_peek_first(const zylib_error_t *obj)
{
    uint64_t size;
    const void *data;
    if (!zylib_private_dequeue_peek_first(obj->dequeue, &size, &data))
    {
        return NULL;
    }
    return data;
}

const zylib_error_box_t *zylib_error_peek_last(const zylib_error_t *obj)
{
    uint64_t size;
    const void *data;
    if (!zylib_private_dequeue_peek_last(obj->dequeue, &size, &data))
    {
        return NULL;
    }
    return data;
}

uint64_t zylib_error_size(const zylib_error_t *obj)
{
    return zylib_private_dequeue_size(obj->dequeue);
}

_Bool zylib_error_is_empty(const zylib_error_t *obj)
{
    return zylib_private_dequeue_is_empty(obj->dequeue);
}

int64_t zylib_error_box_peek_code(const zylib_error_box_t *obj)
{
    return obj->code;
}

const char *zylib_error_box_peek_file(const zylib_error_box_t *obj)
{
    return obj->file;
}

uint64_t zylib_error_box_peek_line_number(const zylib_error_box_t *obj)
{
    return obj->line;
}

const char *zylib_error_box_peek_function(const zylib_error_box_t *obj)
{
    return obj->function;
}

const void *zylib_error_box_peek_auxiliary_data(const zylib_error_box_t *obj, uint64_t *size)
{
    if (obj->size != 0)
    {
        *size = obj->size;
        return &((const uint8_t *)obj)[sizeof(zylib_error_box_t)];
    }
    return NULL;
}
