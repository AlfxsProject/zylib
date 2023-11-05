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
#include "zylib_lib_box.h"

/*
 * Types
 */

struct zylib_error_s
{
    const zylib_allocator_t *allocator;
    zylib_dequeue_t *dequeue;
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
                              size_t size, const void *data, _Bool (*push)(zylib_dequeue_t *, size_t, const void *))
{
    _Bool r = 0;
    zylib_lib_box_t *box = NULL;
    const zylib_error_box_t error_box = {.size = size, .code = code, .file = file, .line = line, .function = function};

    uint64_t i;
    const void *s;

    if (error == NULL || file == NULL || function == NULL)
    {
        goto error;
    }

    r = zylib_lib_box_construct(&box, error->allocator, sizeof(zylib_error_box_t), &error_box);
    if (!r)
    {
        goto error;
    }

    if (size > 0 && data != NULL)
    {
        r = zylib_lib_box_append(&box, size, data);
        if (!r)
        {
            goto error;
        }
    }

    r = zylib_lib_box_peek_size(box, &i);
    if (!r)
    {
        goto error;
    }

    r = zylib_lib_box_peek_data(box, &s);
    if (!r)
    {
        goto error;
    }

    r = push(error->dequeue, i, s);
    if (!r)
    {
        goto error;
    }

error:
    if (box != NULL)
    {
        zylib_lib_box_destruct(&box);
    }
    return r;
}

_Bool zylib_error_construct(zylib_error_t **p_error, const zylib_allocator_t *allocator)
{
    _Bool r;

    if (p_error == NULL || allocator == NULL)
    {
        return 0;
    }

    *p_error = NULL;
    r = zylib_allocator_malloc(allocator, sizeof(zylib_error_t), (void **)p_error);
    if (!r)
    {
        goto error;
    }

    r = zylib_dequeue_construct(&(*p_error)->dequeue, allocator);
    if (!r)
    {
        goto error;
    }

    (*p_error)->allocator = allocator;

    goto done;
error:
    if (*p_error != NULL)
    {
        if ((*p_error)->dequeue != NULL)
        {
            zylib_dequeue_destruct(&(*p_error)->dequeue);
        }
        zylib_allocator_free(allocator, (void **)p_error);
    }
done:
    return r;
}

void zylib_error_destruct(zylib_error_t **p_error)
{
    if (p_error == NULL || *p_error == NULL)
    {
        return;
    }

    zylib_dequeue_destruct(&(*p_error)->dequeue);
    zylib_allocator_free((*p_error)->allocator, (void **)p_error);
}

void zylib_error_clear(zylib_error_t *error)
{
    zylib_dequeue_clear(error->dequeue);
}

_Bool zylib_error_push_first(zylib_error_t *error, int64_t code, const char *file, uint64_t line, const char *function,
                             uint64_t size, const void *p_void)
{
    return zylib_error_push(error, code, file, line, function, size, p_void, zylib_dequeue_push_first);
}

_Bool zylib_error_push_last(zylib_error_t *error, int64_t code, const char *file, uint64_t line, const char *function,
                            uint64_t size, const void *p_void)
{
    return zylib_error_push(error, code, file, line, function, size, p_void, zylib_dequeue_push_last);
}

void zylib_error_discard_first(zylib_error_t *error)
{
    zylib_dequeue_discard_first(error->dequeue);
}

void zylib_error_discard_last(zylib_error_t *error)
{
    zylib_dequeue_discard_last(error->dequeue);
}

const zylib_error_box_t *zylib_error_peek_first(const zylib_error_t *error)
{
    return zylib_dequeue_peek_first(error->dequeue, NULL);
}

const zylib_error_box_t *zylib_error_peek_last(const zylib_error_t *error)
{
    return zylib_dequeue_peek_last(error->dequeue, NULL);
}

uint64_t zylib_error_size(const zylib_error_t *error)
{
    return zylib_dequeue_size(error->dequeue);
}

_Bool zylib_error_is_empty(const zylib_error_t *error)
{
    return zylib_dequeue_is_empty(error->dequeue);
}

int64_t zylib_error_box_peek_code(const zylib_error_box_t *error_box)
{
    return error_box->code;
}

const char *zylib_error_box_peek_file(const zylib_error_box_t *error_box)
{
    return error_box->file;
}

uint64_t zylib_error_box_peek_line_number(const zylib_error_box_t *error_box)
{
    return error_box->line;
}

const char *zylib_error_box_peek_function(const zylib_error_box_t *error_box)
{
    return error_box->function;
}

const void *zylib_error_box_peek_auxiliary_data(const zylib_error_box_t *error_box, uint64_t *p_size)
{
    if (error_box == NULL || error_box->size == 0)
    {
        return NULL;
    }

    if (p_size != NULL)
    {
        *p_size = error_box->size;
    }

    return &((const uint8_t *)error_box)[sizeof(zylib_error_box_t)];
}
