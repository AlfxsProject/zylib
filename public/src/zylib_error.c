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
#include "zylib_private_error.h"
#include <assert.h>

_Bool zylib_error_construct(zylib_error_t **obj, const zylib_allocator_t *allocator)
{
    assert(obj != NULL);
    assert(allocator != NULL);
    return zylib_private_error_construct((zylib_private_error_t **)obj, (const zylib_private_allocator_t *)allocator);
}

void zylib_error_destruct(zylib_error_t **obj)
{
    assert(obj != NULL);
    zylib_private_error_destruct((zylib_private_error_t **)obj);
}

void zylib_error_clear(zylib_error_t *obj)
{
    assert(obj != NULL);
    zylib_private_error_clear((zylib_private_error_t *)obj);
}

_Bool zylib_error_push_first(zylib_error_t *obj, int64_t error_code, const char *file_name, uint64_t line_number,
                             const char *function_name, uint64_t auxiliary_size, const void *auxiliary_data)
{
    assert(obj != NULL);
    assert(file_name != NULL);
    assert(function_name != NULL);
    return zylib_private_error_push_first((zylib_private_error_t *)obj, error_code, file_name, line_number,
                                          function_name, auxiliary_size, auxiliary_data);
}

_Bool zylib_error_push_last(zylib_error_t *obj, int64_t error_code, const char *file_name, uint64_t line_number,
                            const char *function_name, uint64_t auxiliary_size, const void *auxiliary_data)
{
    assert(obj != NULL);
    assert(file_name != NULL);
    assert(function_name != NULL);
    return zylib_private_error_push_last((zylib_private_error_t *)obj, error_code, file_name, line_number,
                                         function_name, auxiliary_size, auxiliary_data);
}

void zylib_error_discard_first(zylib_error_t *obj)
{
    assert(obj != NULL);
    zylib_private_error_discard_first((zylib_private_error_t *)obj);
}

void zylib_error_discard_last(zylib_error_t *obj)
{
    assert(obj != NULL);
    zylib_private_error_discard_last((zylib_private_error_t *)obj);
}

const zylib_error_box_t *zylib_error_peek_first(const zylib_error_t *obj)
{
    assert(obj != NULL);
    return (zylib_error_box_t const *)zylib_private_error_peek_first((const zylib_private_error_t *)obj);
}

const zylib_error_box_t *zylib_error_peek_last(const zylib_error_t *obj)
{
    assert(obj != NULL);
    return (zylib_error_box_t const *)zylib_private_error_peek_last((const zylib_private_error_t *)obj);
}

uint64_t zylib_error_size(const zylib_error_t *obj)
{
    assert(obj != NULL);
    return zylib_private_error_size((const zylib_private_error_t *)obj);
}

_Bool zylib_error_is_empty(const zylib_error_t *obj)
{
    assert(obj != NULL);
    return zylib_private_error_is_empty((const zylib_private_error_t *)obj);
}

int64_t zylib_error_box_peek_error_code(const zylib_error_box_t *obj)
{
    assert(obj != NULL);
    return zylib_private_error_box_peek_error_code((const zylib_private_error_box_t *)obj);
}

const char *zylib_error_box_peek_file_name(const zylib_error_box_t *obj)
{
    assert(obj != NULL);
    return zylib_private_error_box_peek_file_name((const zylib_private_error_box_t *)obj);
}

uint64_t zylib_error_box_peek_line_number(const zylib_error_box_t *obj)
{
    assert(obj != NULL);
    return zylib_private_error_box_peek_line_number((const zylib_private_error_box_t *)obj);
}

const char *zylib_error_box_peek_function_name(const zylib_error_box_t *obj)
{
    assert(obj != NULL);
    return zylib_private_error_box_peek_function_name((const zylib_private_error_box_t *)obj);
}

uint64_t zylib_error_box_peek_auxiliary_size(const zylib_error_box_t *obj)
{
    assert(obj != NULL);
    return zylib_private_error_box_peek_auxiliary_size((const zylib_private_error_box_t *)obj);
}

const void *zylib_error_box_peek_auxiliary_data(const zylib_error_box_t *obj)
{
    assert(obj != NULL);
    return zylib_private_error_box_peek_auxiliary_data((const zylib_private_error_box_t *)obj);
}
