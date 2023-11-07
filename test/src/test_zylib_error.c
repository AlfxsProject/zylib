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
#include "zylib_allocator.h"
#include "zylib_error.h"
#include "zylib_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Type Definitions
 */

typedef _Bool (*zylib_error_push_t)(zylib_error_t *, int64_t, const char *, uint64_t, const char *, uint64_t,
                                    const void *);

typedef const zylib_error_box_t *(*zylib_error_peek_t)(const zylib_error_t *);

typedef void (*zylib_error_discard_t)(zylib_error_t *);

/*
 * Macros
 */

#define PRINT_ERROR(format, ...) ZYLIB_LOG_ERROR(log, format, ##__VA_ARGS__)

/*
 * Global Variables
 */

static zylib_log_t *log = NULL;
static zylib_allocator_t *allocator = NULL;
static zylib_error_t *error = NULL;

/*
 * Static Function Declarations
 */

static inline _Bool test_push_peek(uint64_t before_size, zylib_error_push_t push, zylib_error_peek_t peek);

static inline _Bool test_push_peek_discard(zylib_error_push_t push, zylib_error_peek_t peek,
                                           zylib_error_discard_t discard);

static inline _Bool test_push_peek_discard_first();

static inline _Bool test_push_peek_discard_last();

static inline _Bool test_loop_push_peek_clear(zylib_error_push_t push, zylib_error_peek_t peek);

static inline _Bool test_loop_push_peek_clear_first();

static inline _Bool test_loop_push_peek_clear_last();

/*
 * Main
 */

int main()
{
    int r = EXIT_FAILURE;

    if (!zylib_allocator_construct(&allocator, malloc, realloc, free))
    {
        fprintf(stderr, "zylib_allocator_construct()\n");
        goto error;
    }

    if (!zylib_log_construct(&log, allocator, stderr, ZYLIB_INFO, ZYLIB_LOG_FORMAT_PLAIN))
    {
        fprintf(stderr, "zylib_log_construct()\n");
        goto error;
    }

    if (!zylib_error_construct(&error, allocator))
    {
        PRINT_ERROR("zylib_error_construct(): fail");
        goto error;
    }

    /*
     * TESTS
     */

    if (!zylib_error_is_empty(error))
    {
        PRINT_ERROR("zylib_dequeue_is_empty() failed");
        goto error;
    }

    if (!test_push_peek_discard_first())
    {
        PRINT_ERROR("test_push_peek_discard_first() failed");
        goto error;
    }

    if (!test_push_peek_discard_last())
    {
        PRINT_ERROR("test_push_peek_discard_last() failed");
        goto error;
    }

    if (!test_loop_push_peek_clear_first())
    {
        PRINT_ERROR("test_loop_push_peek_clear_first() failed");
        goto error;
    }

    if (!test_loop_push_peek_clear_last())
    {
        PRINT_ERROR("test_loop_push_peek_clear_last() failed");
        goto error;
    }

    if (!zylib_error_is_empty(error))
    {
        PRINT_ERROR("zylib_dequeue_is_empty() failed");
        goto error;
    }

    r = EXIT_SUCCESS;
error:
    if (log != NULL)
    {
        zylib_log_destruct(&log);
    }
    if (error != NULL)
    {
        zylib_error_destruct(&error);
    }
    if (allocator != NULL)
    {
        zylib_allocator_destruct(&allocator);
    }
    return r;
}

/*
 * Static Function Definitions
 */

_Bool test_push_peek(uint64_t before_size, zylib_error_push_t push, zylib_error_peek_t peek)
{
    _Bool r = 0;

    int64_t code;
    const char *file;
    uint64_t line;
    const char *function;

    void *ptr = NULL;
    size_t ptr_size = 1;

    const zylib_error_box_t *managed_box = NULL;
    uint64_t managed_ptr_size;
    const void *managed_ptr = NULL;

    if (!zylib_allocator_malloc(allocator, ptr_size, &ptr))
    {
        PRINT_ERROR("zylib_allocator_malloc() failed");
        goto error;
    }

    code = -1;
    file = __FILE__;
    line = __LINE__;
    function = __func__;
    memset(ptr, 0, ptr_size);

    if (!push(error, code, file, line, function, ptr_size, ptr))
    {
        PRINT_ERROR("zylib_dequeue_push_first() failed");
        goto error;
    }

    if (before_size + 1 != zylib_error_size(error))
    {
        PRINT_ERROR("zylib_dequeue_size() failed");
        goto error;
    }

    managed_box = peek(error);

    if (code != zylib_error_box_peek_error_code(managed_box))
    {
        PRINT_ERROR("zylib_error_box_peek_error_code() failed");
        goto error;
    }

    if (strcmp(file, zylib_error_box_peek_file_name(managed_box)) != 0)
    {
        PRINT_ERROR("zylib_error_box_peek_file_name() failed");
    }

    if (line != zylib_error_box_peek_line_number(managed_box))
    {
        PRINT_ERROR("zylib_error_box_peek_line_number() failed");
        goto error;
    }

    if (strcmp(function, zylib_error_box_peek_function_name(managed_box)) != 0)
    {
        PRINT_ERROR("zylib_error_box_peek_function_name() failed");
        goto error;
    }

    managed_ptr_size = zylib_error_box_peek_auxiliary_size(managed_box);
    managed_ptr = zylib_error_box_peek_auxiliary_data(managed_box);

    if (managed_ptr_size != ptr_size || memcmp(ptr, managed_ptr, ptr_size) != 0)
    {
        PRINT_ERROR("zylib_dequeue_peek_first() failed");
        goto error;
    }

    r = 1;
error:
    if (ptr != NULL)
    {
        zylib_allocator_free(allocator, &ptr);
    }
    return r;
}

static inline _Bool test_push_peek_discard(zylib_error_push_t push, zylib_error_peek_t peek,
                                           zylib_error_discard_t discard)
{
    size_t before_size = zylib_error_size(error);
    _Bool r = test_push_peek(before_size, push, peek);

    discard(error);

    if (before_size != zylib_error_size(error))
    {
        PRINT_ERROR("zylib_dequeue_size() failed");
        goto error;
    }
error:
    return r;
}

_Bool test_push_peek_discard_first()
{
    return test_push_peek_discard(zylib_error_push_first, zylib_error_peek_first, zylib_error_discard_first);
}

_Bool test_push_peek_discard_last()
{
    return test_push_peek_discard(zylib_error_push_last, zylib_error_peek_last, zylib_error_discard_last);
}

_Bool test_loop_push_peek_clear(zylib_error_push_t push, zylib_error_peek_t peek)
{
    _Bool r = 0;

    for (uint8_t i = 0; i < 100; ++i)
    {
        if (!test_push_peek(i, push, peek))
        {
            PRINT_ERROR("test_push_peek() failed");
            goto error;
        }
    }

    zylib_error_clear(error);

    if (!zylib_error_is_empty(error))
    {
        PRINT_ERROR("zylib_error_is_empty() failed");
        goto error;
    }

    r = 1;
error:
    return r;
}

_Bool test_loop_push_peek_clear_first()
{
    return test_loop_push_peek_clear(zylib_error_push_first, zylib_error_peek_first);
}

_Bool test_loop_push_peek_clear_last()
{
    return test_loop_push_peek_clear(zylib_error_push_first, zylib_error_peek_first);
}
