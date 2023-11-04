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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <zylib_alloc.h>
#include <zylib_error.h>
#include <zylib_log.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#define SIZE (512U)
#define ITERATIONS (10U)

#define PRINT(format, ...) ZYLIB_LOG_ERROR(log, format, ##__VA_ARGS__)

static zylib_log_t *log = NULL;

static _Bool test_empty(const zylib_error_t *const error)
{
    _Bool r = false;

    if (zylib_error_size(error) != 0)
    {
        PRINT("zylib_error_size()");
        goto done;
    }

    if (!zylib_error_is_empty(error))
    {
        PRINT("zylib_error_is_empty()");
        goto done;
    }

    r = true;
done:
    return r;
}

static _Bool test_size_positive_n(const zylib_error_t *error, uint64_t n)
{
    _Bool r = false;

    if (zylib_error_size(error) != n)
    {
        PRINT("zylib_error_size()");
        goto done;
    }

    if (zylib_error_is_empty(error))
    {
        PRINT("zylib_error_is_empty()");
        goto done;
    }

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
        PRINT("test_empty()");
        goto done;
    }

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
            PRINT("getrandom()");
            goto done;
        }

        ptr->box.size = sizeof(buf) - sizeof(struct ptr_s);
        ptr->file = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
        ptr->function = "a b c d e f g h i j k l m n o p q r s t u v w x y z";

        if (push(error, ptr->code, ptr->file, ptr->line, ptr->function, &ptr->box) != ZYLIB_OK)
        {
            PRINT("push()");
            goto done;
        }

        if (!test_size_positive_n(error, i + 1))
        {
            PRINT("test_size_positive_n");
            goto done;
        }

        error_box = peek(error);

        if (error_box == NULL)
        {
            PRINT("peek");
            goto done;
        }

        if (ptr->code != zylib_error_box_peek_code(error_box))
        {
            PRINT("zylib_error_box_peek_code");
            goto done;
        }

        if (strcmp(ptr->file, zylib_error_box_peek_file(error_box)) != 0)
        {
            PRINT("zylib_error_box_peek_file");
            goto done;
        }

        if (ptr->line != zylib_error_box_peek_line_number(error_box))
        {
            PRINT("zylib_error_box_peek_line_number");
            goto done;
        }

        if (strcmp(ptr->function, zylib_error_box_peek_function(error_box)) != 0)
        {
            PRINT("zylib_error_box_peek_function");
            goto done;
        }

        data = zylib_error_box_peek_opaque(error_box, &size);

        if (ptr->box.size != size)
        {
            PRINT("zylib_error_box_peek_opaque(): size");
            goto done;
        }

        if (memcmp(ptr->box.data, data, ptr->box.size) != 0)
        {
            PRINT("zylib_error_box_peek_opaque(): data");
            goto done;
        }
    }

    zylib_error_clear(error);

    if (!test_empty(error))
    {
        PRINT("test_empty()");
        goto done;
    }

    r = true;
done:
    return r;
}

int main()
{
    int r = EXIT_FAILURE;
    zylib_alloc_t *alloc = NULL;
    zylib_error_t *error = NULL;

    if (zylib_alloc_construct(&alloc, malloc, realloc, free) != ZYLIB_OK)
    {
        fprintf(stderr, "zylib_alloc_construct()\n");
        goto done;
    }

    if (zylib_log_construct(&log, alloc, 2) != ZYLIB_OK)
    {
        fprintf(stderr, "zylib_log_construct()\n");
        goto done;
    }

    if (zylib_error_construct(&error, alloc) != ZYLIB_OK)
    {
        PRINT("zylib_error_construct()");
        goto done;
    }

    if (!test_loop(error, zylib_error_push_first, zylib_error_peek_first))
    {
        PRINT("test_loop(): first");
        goto done;
    }

    if (!test_loop(error, zylib_error_push_last, zylib_error_peek_last))
    {
        PRINT("test_loop(): last");
        goto done;
    }

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
        zylib_alloc_destruct(&alloc);
    }
    return r;
}
#pragma clang diagnostic pop
