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
#include <sys/random.h>

#define SIZE (512U)
#define ITERATIONS (10U)

#define PRINT_INFO(format, ...) ZYLIB_LOG_INFO(log, format, ##__VA_ARGS__)
#define PRINT_WARN(format, ...) ZYLIB_LOG_WARN(log, format, ##__VA_ARGS__)
#define PRINT_ERROR(format, ...) ZYLIB_LOG_ERROR(log, format, ##__VA_ARGS__)

static zylib_log_t *log = NULL;

static _Bool test_empty(const zylib_error_t *const error)
{
    _Bool r = false;

    if (zylib_error_size(error) != 0)
    {
        PRINT_ERROR("zylib_error_size(expect 0): fail");
        goto done;
    }

    PRINT_INFO("zylib_error_size(expect 0): pass");

    if (!zylib_error_is_empty(error))
    {
        PRINT_ERROR("zylib_error_is_empty(expect true): fail");
        goto done;
    }

    PRINT_INFO("zylib_error_is_empty(expect true): pass");

    r = true;
done:
    return r;
}

static _Bool test_size_positive_n(const zylib_error_t *error, uint64_t n)
{
    _Bool r = false;

    if (zylib_error_size(error) != n)
    {
        PRINT_ERROR("zylib_error_size(expect %lu): fail", n);
        goto done;
    }

    PRINT_INFO("zylib_error_size(expect %lu): pass", n);

    if (zylib_error_is_empty(error))
    {
        PRINT_ERROR("zylib_error_is_empty(expect false): fail");
        goto done;
    }

    PRINT_INFO("zylib_error_is_empty(expect false): pass");

    r = true;
done:
    return r;
}

static _Bool test_loop(zylib_error_t *error,
                       zylib_return_t (*push)(zylib_error_t *err, int64_t code, const char *file, uint64_t line,
                                              const char *function, const zylib_box_t *box),
                       const zylib_error_box_t *(*peek)(const zylib_error_t *err))
{
    _Bool r = false;

    if (!test_empty(error))
    {
        PRINT_ERROR("test_empty(): fail");
        goto done;
    }

    PRINT_INFO("test_empty(): pass");

    for (unsigned int i = 0; i < ITERATIONS; ++i)
    {
        const zylib_error_box_t *error_box;
        uint64_t size;
        const void *data;
        char buf[SIZE] = {0};
        struct ptr_s
        {
            int64_t code;
            const char *file;
            uint64_t line;
            const char *function;
            zylib_box_t box;
        } *const ptr = (void *)buf;

        if (getrandom(ptr, SIZE, 0) != SIZE)
        {
            PRINT_ERROR("getrandom(): fail");
            goto done;
        }

        PRINT_INFO("getrandom(): pass");

        ptr->box.size = sizeof(buf) - sizeof(struct ptr_s);
        ptr->file = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
        ptr->function = "a b c d e f g h i j k l m n o p q r s t u v w x y z";

        if (push(error, ptr->code, ptr->file, ptr->line, ptr->function, &ptr->box) != ZYLIB_OK)
        {
            PRINT_ERROR("push(): fail");
            goto done;
        }

        PRINT_INFO("push(): pass");

        if (!test_size_positive_n(error, i + 1))
        {
            PRINT_ERROR("test_size_positive_n(%u): fail", i + 1);
            goto done;
        }

        PRINT_INFO("test_size_positive_n(%u): pass", i + 1);

        error_box = peek(error);

        if (error_box == NULL)
        {
            PRINT_ERROR("peek(): fail");
            goto done;
        }

        PRINT_INFO("peek(): pass");

        if (ptr->code != zylib_error_box_peek_code(error_box))
        {
            PRINT_ERROR("zylib_error_box_peek_code(expect %ld): fail", ptr->code);
            goto done;
        }

        PRINT_INFO("zylib_error_box_peek_code(expect %ld): pass", ptr->code);

        if (strcmp(ptr->file, zylib_error_box_peek_file(error_box)) != 0)
        {
            PRINT_ERROR("zylib_error_box_peek_file(): fail");
            goto done;
        }

        PRINT_INFO("zylib_error_box_peek_file(): pass");

        if (ptr->line != zylib_error_box_peek_line_number(error_box))
        {
            PRINT_ERROR("zylib_error_box_peek_line_number(expect %lu): fail", ptr->line);
            goto done;
        }

        PRINT_INFO("zylib_error_box_peek_line_number(): pass");

        if (strcmp(ptr->function, zylib_error_box_peek_function(error_box)) != 0)
        {
            PRINT_ERROR("zylib_error_box_peek_function(): fail");
            goto done;
        }

        PRINT_INFO("zylib_error_box_peek_function(): pass");

        data = zylib_error_box_peek_opaque(error_box, &size);

        if (ptr->box.size != size)
        {
            PRINT_ERROR("zylib_error_box_peek_opaque(size): fail");
            goto done;
        }

        PRINT_INFO("zylib_error_box_peek_opaque(size): pass");

        if (memcmp(ptr->box.data, data, ptr->box.size) != 0)
        {
            PRINT_ERROR("zylib_error_box_peek_opaque(): data");
            goto done;
        }

        PRINT_INFO("zylib_error_box_peek_opaque(data): pass");
    }

    zylib_error_clear(error);

    if (!test_empty(error))
    {
        PRINT_ERROR("test_empty(): fail");
        goto done;
    }

    PRINT_INFO("test_empty(): pass");

    r = true;
done:
    return r;
}

int main()
{
    int r = EXIT_FAILURE;
    zylib_allocator_t *alloc = NULL;
    zylib_error_t *error = NULL;

    if (zylib_allocator_construct(&alloc, malloc, realloc, free) != ZYLIB_OK)
    {
        fprintf(stderr, "zylib_allocator_construct()\n");
        goto done;
    }

    if (zylib_log_construct(&log, alloc, "zylib-test-error-log.log", ZYLIB_INFO, ZYLIB_LOG_FORMAT_PLAIN) != ZYLIB_OK)
    {
        fprintf(stderr, "zylib_log_construct()\n");
        goto done;
    }

    if (zylib_error_construct(&error, alloc) != ZYLIB_OK)
    {
        PRINT_ERROR("zylib_error_construct(): fail");
        goto done;
    }

    PRINT_INFO("zylib_error_construct(): pass");

    if (!test_loop(error, zylib_error_push_first, zylib_error_peek_first))
    {
        PRINT_ERROR("test_loop(first(): fail");
        goto done;
    }

    PRINT_INFO("test_loop(first): pass");

    if (!test_loop(error, zylib_error_push_last, zylib_error_peek_last))
    {
        PRINT_ERROR("test_loop(last(): fail");
        goto done;
    }

    PRINT_INFO("test_loop(last): pass");

    r = EXIT_SUCCESS;
done:
    if (log != NULL)
    {
        zylib_log_destruct(&log);
    }
    if (error != NULL)
    {
        zylib_error_destruct(&error);
    }
    if (alloc != NULL)
    {
        zylib_allocator_destruct(&alloc);
    }
    return r;
}
