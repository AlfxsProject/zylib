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
#include "zylib_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Type Definitions
 */

typedef _Bool (*zylib_dequeue_push_t)(zylib_dequeue_t *, uint64_t, const void *);
typedef _Bool (*zylib_dequeue_peek_t)(const zylib_dequeue_t *, uint64_t *, const void **);
typedef void (*zylib_dequeue_discard_t)(zylib_dequeue_t *);

/*
 * Macros
 */

#define PRINT_ERROR(format, ...) ZYLIB_LOGGER_ERROR(log, format, ##__VA_ARGS__)

/*
 * Global Variables
 */

static zylib_logger_t *log = NULL;
static zylib_allocator_t *allocator = NULL;
static zylib_dequeue_t *dequeue = NULL;

/*
 * Static Function Declarations
 */

/* Push, Peek */
static inline _Bool test_push_peek(uint64_t before_size, zylib_dequeue_push_t push, zylib_dequeue_peek_t peek);

/* Push, Peek, Discard */
static inline _Bool test_push_peek_discard(zylib_dequeue_push_t push, zylib_dequeue_peek_t peek,
                                           zylib_dequeue_discard_t discard);

/* Push First, Peek First, Discard First */
static inline _Bool test_push_peek_discard_first();

/* Push Last, Peek Last, Discard Last */
static inline _Bool test_push_peek_discard_last();

/* Loop: Push, Peek; Clear */
static inline _Bool test_loop_push_peek_clear(zylib_dequeue_push_t push, zylib_dequeue_peek_t peek);

/* Loop: Push First, Peek First; Clear */
static inline _Bool test_loop_push_peek_clear_first();

/* Loop: Push Last, Peek Last; Clear */
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

    if (!zylib_logger_construct(&log, allocator, stderr, ZYLIB_INFO, ZYLIB_LOGGER_FORMAT_PLAINTEXT))
    {
        fprintf(stderr, "zylib_logger_construct()\n");
        goto error;
    }

    if (!zylib_dequeue_construct(&dequeue, allocator))
    {
        PRINT_ERROR("zylib_dequeue_construct()");
        goto error;
    }

    /*
     * TESTS
     */

    if (!zylib_dequeue_is_empty(dequeue))
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

    if (!zylib_dequeue_is_empty(dequeue))
    {
        PRINT_ERROR("zylib_dequeue_is_empty() failed");
        goto error;
    }

    r = EXIT_SUCCESS;
error:
    if (log != NULL)
    {
        zylib_logger_destruct(&log);
    }
    if (dequeue != NULL)
    {
        zylib_dequeue_destruct(&dequeue);
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

_Bool test_push_peek(uint64_t before_size, zylib_dequeue_push_t push, zylib_dequeue_peek_t peek)
{
    _Bool r = 0;

    void *ptr = NULL;
    uint64_t ptr_size = 1;

    const void *managed_ptr = NULL;
    uint64_t managed_ptr_size = 0;

    if (!zylib_allocator_malloc(allocator, ptr_size, &ptr))
    {
        PRINT_ERROR("zylib_allocator_malloc() failed");
        goto error;
    }

    memset(ptr, 0, ptr_size);

    if (!push(dequeue, ptr_size, ptr))
    {
        PRINT_ERROR("push() failed");
        goto error;
    }

    if (before_size + 1 != zylib_dequeue_size(dequeue))
    {
        PRINT_ERROR("zylib_dequeue_size() failed");
        goto error;
    }

    if (!peek(dequeue, &managed_ptr_size, &managed_ptr) || managed_ptr_size != ptr_size ||
        memcmp(ptr, managed_ptr, ptr_size) != 0)
    {
        PRINT_ERROR("peek() failed");
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

_Bool test_push_peek_discard(zylib_dequeue_push_t push, zylib_dequeue_peek_t peek, zylib_dequeue_discard_t discard)
{
    size_t before_size = zylib_dequeue_size(dequeue);
    _Bool r = test_push_peek(before_size, push, peek);

    if (!r)
    {
        PRINT_ERROR("test_push_peek() failed");
        goto error;
    }

    discard(dequeue);

    if (before_size != zylib_dequeue_size(dequeue))
    {
        PRINT_ERROR("zylib_dequeue_size() failed");
        goto error;
    }
error:
    return r;
}

_Bool test_push_peek_discard_first()
{
    return test_push_peek_discard(zylib_dequeue_push_first, zylib_dequeue_peek_first, zylib_dequeue_discard_first);
}

_Bool test_push_peek_discard_last()
{
    return test_push_peek_discard(zylib_dequeue_push_last, zylib_dequeue_peek_last, zylib_dequeue_discard_last);
}

_Bool test_loop_push_peek_clear(zylib_dequeue_push_t push, zylib_dequeue_peek_t peek)
{
    _Bool r = 0;

    for (uint64_t i = 0; i < 100; ++i)
    {
        if (!test_push_peek(i, push, peek))
        {
            PRINT_ERROR("test_push_peek() failed");
            goto error;
        }
    }

    zylib_dequeue_clear(dequeue);

    if (!zylib_dequeue_is_empty(dequeue))
    {
        PRINT_ERROR("zylib_dequeue_is_empty() failed");
        goto error;
    }

    r = 1;
error:
    return r;
}

_Bool test_loop_push_peek_clear_first()
{
    return test_loop_push_peek_clear(zylib_dequeue_push_first, zylib_dequeue_peek_first);
}

_Bool test_loop_push_peek_clear_last()
{
    return test_loop_push_peek_clear(zylib_dequeue_push_last, zylib_dequeue_peek_last);
}
