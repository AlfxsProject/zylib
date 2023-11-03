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
#include <zylib_alloc.h>

int main()
{
    void *ptr1 = nullptr, *ptr2 = nullptr;
    int r = EXIT_FAILURE;
    zylib_alloc_t *alloc = nullptr;

    if (zylib_alloc_construct(&alloc, malloc, realloc, free) != ZYLIB_OK)
    {
        fprintf(stderr, "ERROR: zylib_alloc_construct()\n");
        goto done;
    }

    if (zylib_malloc(alloc, 1, &ptr1) != ZYLIB_OK)
    {
        fprintf(stderr, "ERROR: malloc(1)\n");
        goto done;
    }

    if (zylib_realloc(alloc, 1, &ptr2) != ZYLIB_OK)
    {
        fprintf(stderr, "ERROR: realloc(1)\n");
        goto done;
    }

    r = EXIT_SUCCESS;
done:
    if (ptr1 != nullptr)
    {
        zylib_free(alloc, &ptr1);
    }
    if (ptr2 != nullptr)
    {
        zylib_free(alloc, &ptr2);
    }
    if (alloc != nullptr)
    {
        zylib_alloc_destruct(&alloc);
    }
    return r;
}