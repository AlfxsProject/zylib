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
#include "zylib_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>

#define SIZE (512U)
#define ITERATIONS (10U)

#define PRINT_INFO(format, ...) ZYLIB_LOG_INFO(log, format, ##__VA_ARGS__)
#define PRINT_WARN(format, ...) ZYLIB_LOG_WARN(log, format, ##__VA_ARGS__)
#define PRINT_ERROR(format, ...) ZYLIB_LOG_ERROR(log, format, ##__VA_ARGS__)

static zylib_log_t *log = NULL;

static _Bool test_empty(const zylib_dequeue_t *dqe)
{
    _Bool r = false;

    if (zylib_dequeue_size(dqe) != 0)
    {
        PRINT_ERROR("zylib_dequeue_size()");
        goto done;
    }

    if (!zylib_dequeue_is_empty(dqe))
    {
        PRINT_ERROR("zylib_dequeue_is_empty()");
        goto done;
    }

    r = true;
done:
    return r;
}

static _Bool test_size_positive_n(const zylib_dequeue_t *dqe, size_t n)
{
    _Bool r = false;

    if (zylib_dequeue_size(dqe) != n)
    {
        PRINT_ERROR("zylib_dequeue_size()");
        goto done;
    }

    if (zylib_dequeue_is_empty(dqe))
    {
        PRINT_ERROR("zylib_dequeue_is_empty()");
        goto done;
    }

    r = true;
done:
    return r;
}

static _Bool test_loop(zylib_dequeue_t *dqe, _Bool (*push)(zylib_dequeue_t *, const zylib_box_t *),
                       const zylib_box_t *(*peek)(const zylib_dequeue_t *))
{
    _Bool r = false;

    if (!test_empty(dqe))
    {
        PRINT_ERROR("test_empty()");
        goto done;
    }

    for (unsigned int i = 0; i < ITERATIONS; ++i)
    {
        const zylib_box_t *first;
        char buf[SIZE] = {0};
        zylib_box_t *const box = (zylib_box_t *)buf;
        box->size = sizeof(buf) - sizeof(zylib_box_t);

        if (getrandom(box->data, box->size, 0) != (ssize_t)box->size)
        {
            PRINT_ERROR("getrandom()");
            goto done;
        }

        if (!push(dqe, box))
        {
            PRINT_ERROR("push()");
            goto done;
        }

        if (!test_size_positive_n(dqe, i + 1))
        {
            PRINT_ERROR("test_size_positive_n");
            goto done;
        }

        first = peek(dqe);

        if (box->size != first->size)
        {
            PRINT_ERROR("peek(): size");
            goto done;
        }

        if (memcmp(box->data, first->data, box->size) != 0)
        {
            PRINT_ERROR("peek(): data");
            goto done;
        }
    }

    zylib_dequeue_clear(dqe);

    if (!test_empty(dqe))
    {
        PRINT_ERROR("test_empty()");
        goto done;
    }

    r = true;
done:
    return r;
}

int main()
{
    int r = EXIT_FAILURE;

    zylib_allocator_t *alloc = NULL;
    zylib_dequeue_t *dqe = NULL;

    if (!zylib_allocator_construct(&alloc, malloc, realloc, free))
    {
        fprintf(stderr, "zylib_allocator_construct()\n");
        goto done;
    }

    if (!zylib_log_construct(&log, alloc, "zylib-test-dequeue-log.log", ZYLIB_INFO, ZYLIB_LOG_FORMAT_PLAIN))
    {
        fprintf(stderr, "zylib_log_construct()\n");
        goto done;
    }

    if (!zylib_dequeue_construct(&dqe, alloc))
    {
        PRINT_ERROR("zylib_dequeue_construct()");
        goto done;
    }

    if (!test_loop(dqe, zylib_dequeue_push_first, zylib_dequeue_peek_first))
    {
        PRINT_ERROR("test_loop(): first");
        goto done;
    }

    if (!test_loop(dqe, zylib_dequeue_push_last, zylib_dequeue_peek_last))
    {
        PRINT_ERROR("test_loop(): last");
        goto done;
    }

    r = EXIT_SUCCESS;
done:
    if (log != NULL)
    {
        zylib_log_destruct(&log);
    }
    if (dqe != NULL)
    {
        zylib_dequeue_destruct(&dqe);
    }
    if (alloc != NULL)
    {
        zylib_allocator_destruct(&alloc);
    }
    return r;
}
