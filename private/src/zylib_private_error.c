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
#include "zylib_private_error.h"
#include "zylib_private_dequeue.h"
#include <string.h>

typedef _Bool (*zylib_private_dequeue_push_t)(zylib_private_dequeue_t *, uint64_t, const void *);

struct zylib_private_error_box_s
{
    int64_t error_code;
    const char *file_name;
    uint64_t line_number;
    const char *function_name;
    uint64_t auxiliary_size;
};

struct zylib_private_error_s
{
    const zylib_private_allocator_t *allocator;
    zylib_private_dequeue_t *dequeue;
};

static _Bool zylib_private_error_push(zylib_private_error_t *obj, int64_t error_code, const char *file_name,
                                      uint64_t line_number, const char *function_name, uint64_t auxiliary_size,
                                      const void *auxiliary_data, zylib_private_dequeue_push_t push)
{
    _Bool r;
    unsigned char *ptr = NULL;
    const zylib_private_error_box_t error_box = {.error_code = error_code,
                                                 .file_name = file_name,
                                                 .line_number = line_number,
                                                 .function_name = function_name,
                                                 .auxiliary_size = auxiliary_size};

    r = zylib_private_allocator_malloc(obj->allocator, sizeof(zylib_private_error_box_t) + auxiliary_size,
                                       (void **)&ptr);
    if (!r)
    {
        goto error;
    }

    memcpy(ptr, &error_box, sizeof(zylib_private_error_box_t));
    memcpy(ptr + sizeof(zylib_private_error_box_t), auxiliary_data, auxiliary_size);

    r = push(obj->dequeue, sizeof(zylib_private_error_box_t) + auxiliary_size, ptr);
    if (!r)
    {
        goto error;
    }

error:
    if (ptr != NULL)
    {
        zylib_private_allocator_free(obj->allocator, (void **)&ptr);
    }
    return r;
}

_Bool zylib_private_error_construct(zylib_private_error_t **obj, const zylib_private_allocator_t *allocator)
{
    _Bool r;

    *obj = NULL;
    r = zylib_private_allocator_malloc(allocator, sizeof(zylib_private_error_t), (void **)obj);
    if (!r)
    {
        goto error;
    }

    r = zylib_private_dequeue_construct(&(*obj)->dequeue, allocator);
    if (!r)
    {
        goto error;
    }

    (*obj)->allocator = allocator;

    goto done;
error:
    zylib_private_error_destruct(obj);
done:
    return r;
}

void zylib_private_error_destruct(zylib_private_error_t **obj)
{
    if (*obj != NULL)
    {
        if ((*obj)->dequeue != NULL)
        {
            zylib_private_dequeue_destruct(&(*obj)->dequeue);
        }
        zylib_private_allocator_free((*obj)->allocator, (void **)obj);
    }
}

void zylib_private_error_clear(zylib_private_error_t *obj)
{
    zylib_private_dequeue_clear(obj->dequeue);
}

_Bool zylib_private_error_push_first(zylib_private_error_t *obj, int64_t error_code, const char *file_name,
                                     uint64_t line_number, const char *function_name, uint64_t auxiliary_size,
                                     const void *auxiliary_data)
{
    return zylib_private_error_push(obj, error_code, file_name, line_number, function_name, auxiliary_size,
                                    auxiliary_data, zylib_private_dequeue_push_first);
}

_Bool zylib_private_error_push_last(zylib_private_error_t *obj, int64_t error_code, const char *file_name,
                                    uint64_t line_number, const char *function_name, uint64_t auxiliary_size,
                                    const void *auxiliary_data)
{
    return zylib_private_error_push(obj, error_code, file_name, line_number, function_name, auxiliary_size,
                                    auxiliary_data, zylib_private_dequeue_push_last);
}

void zylib_private_error_discard_first(zylib_private_error_t *obj)
{
    zylib_private_dequeue_discard_first(obj->dequeue);
}

void zylib_private_error_discard_last(zylib_private_error_t *obj)
{
    zylib_private_dequeue_discard_last(obj->dequeue);
}

const zylib_private_error_box_t *zylib_private_error_peek_first(const zylib_private_error_t *obj)
{
    uint64_t size;
    const void *ptr;
    zylib_private_dequeue_peek_first(obj->dequeue, &size, &ptr);
    return ptr;
}

const zylib_private_error_box_t *zylib_private_error_peek_last(const zylib_private_error_t *obj)
{
    uint64_t size;
    const void *ptr;
    zylib_private_dequeue_peek_last(obj->dequeue, &size, &ptr);
    return ptr;
}

uint64_t zylib_private_error_size(const zylib_private_error_t *obj)
{
    return zylib_private_dequeue_size(obj->dequeue);
}

_Bool zylib_private_error_is_empty(const zylib_private_error_t *obj)
{
    return zylib_private_dequeue_is_empty(obj->dequeue);
}

int64_t zylib_private_error_box_peek_error_code(const zylib_private_error_box_t *obj)
{
    return obj->error_code;
}

const char *zylib_private_error_box_peek_file_name(const zylib_private_error_box_t *obj)
{
    return obj->file_name;
}

uint64_t zylib_private_error_box_peek_line_number(const zylib_private_error_box_t *obj)
{
    return obj->line_number;
}

const char *zylib_private_error_box_peek_function_name(const zylib_private_error_box_t *obj)
{
    return obj->function_name;
}

uint64_t zylib_private_error_box_peek_auxiliary_size(const zylib_private_error_box_t *obj)
{
    return obj->auxiliary_size;
}

const void *zylib_private_error_box_peek_auxiliary_data(const zylib_private_error_box_t *obj)
{
    return (const unsigned char *)obj + sizeof(zylib_private_error_box_t);
}
