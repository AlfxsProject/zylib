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
#include "zylib_logger.h"
#include "zylib_private_box.h"
#include <stdlib.h>
#include <string.h>

#define PRINT_ERROR(format) ZYLIB_LOGGER_ERROR(log, format)

static zylib_allocator_t *allocator = NULL;
static zylib_logger_t *log = NULL;

static inline _Bool test_box();

int main()
{
    int r = EXIT_FAILURE;

    if (!zylib_allocator_construct(&allocator, malloc, realloc, free))
    {
        fprintf(stderr, "zylib_allocator_construct() failed\n");
        goto error;
    }

    if (!zylib_logger_construct(&log, allocator, stderr, ZYLIB_INFO, ZYLIB_LOGGER_FORMAT_PLAINTEXT))
    {
        fprintf(stderr, "zylib_logger_construct() failed\n");
        goto error;
    }

    /*
     * TESTS
     */

    if (!test_box())
    {
        PRINT_ERROR("test_box() failed");
        goto error;
    }

    r = EXIT_SUCCESS;
error:
    if (log != NULL)
    {
        zylib_logger_destruct(&log);
    }
    if (allocator != NULL)
    {
        zylib_allocator_destruct(&allocator);
    }
    return r;
}

_Bool test_box()
{
    _Bool r = 0;

    uint64_t size_1 = (uint64_t)(rand() % 100) + 1;
    void *data_1 = NULL;

    uint64_t size_2 = (uint64_t)(rand() % 100) + 1;
    void *data_2 = NULL;

    uint64_t managed_size = 0;
    const void *managed_data = NULL;

    zylib_private_box_t *box = NULL;

    if (!zylib_allocator_malloc(allocator, size_1, &data_1))
    {
        PRINT_ERROR("zylib_allocator_malloc() failed");
        goto error;
    }

    if (!zylib_allocator_malloc(allocator, size_2, &data_2))
    {
        PRINT_ERROR("zylib_allocator_malloc() failed");
        goto error;
    }

    memset(data_1, rand() % 256, size_1);
    memset(data_2, rand() % 256, size_2);

    if (!zylib_private_box_construct(&box, (const zylib_private_allocator_t *)allocator, size_1, data_1))
    {
        PRINT_ERROR("zylib_private_box_construct() failed");
        goto error;
    }

    if (zylib_private_box_peek_size(box) != size_1)
    {
        PRINT_ERROR("zylib_private_box_peek_size() failed");
        goto error;
    }

    if (memcmp(data_1, zylib_private_box_peek_data(box), size_1) != 0)
    {
        PRINT_ERROR("zylib_private_box_peek_data() failed");
        goto error;
    }

    if (!zylib_private_box_append(box, size_2, data_2))
    {
        PRINT_ERROR("zylib_private_box_append() failed");
        goto error;
    }

    if (zylib_private_box_peek_size(box) != size_1 + size_2)
    {
        PRINT_ERROR("zylib_private_box_peek_size() failed");
        goto error;
    }

    if (memcmp(data_1, zylib_private_box_peek_data(box), size_1) != 0 ||
        memcmp(data_2, &((const uint8_t *)zylib_private_box_peek_data(box))[size_1], size_2) != 0)
    {
        PRINT_ERROR("zylib_private_box_peek_data() failed");
        goto error;
    }

    if (!zylib_private_box_split_by_index(box, size_1, &managed_size, &managed_data) || managed_size != size_2 ||
        memcmp(data_2, managed_data, size_2) != 0)
    {
        PRINT_ERROR("zylib_private_box_split_by_index() failed");
        goto error;
    }

    r = 1;
error:
    if (data_1 != NULL)
    {
        zylib_allocator_free(allocator, &data_1);
    }
    if (data_2 != NULL)
    {
        zylib_allocator_free(allocator, &data_2);
    }
    if (box != NULL)
    {
        zylib_private_box_destruct(&box);
    }
    return r;
}
