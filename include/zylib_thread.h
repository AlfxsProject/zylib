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
#pragma once
#include "zylib_alloc.h"

typedef struct zylib_mutex_s zylib_mutex_t;

#ifdef DO_MUTEX

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((nonnull)) zylib_return_t zylib_mutex_construct(zylib_mutex_t **mutex, const zylib_alloc_t *alloc);
    __attribute__((nonnull)) zylib_return_t zylib_mutex_destruct(zylib_mutex_t **mutex);
    __attribute__((nonnull)) zylib_return_t zylib_mutex_lock(zylib_mutex_t *mutex);
    __attribute__((nonnull)) zylib_return_t zylib_mutex_unlock(zylib_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#else

#define zylib_mutex_construct(mutex, alloc) (ZYLIB_OK)
#define zylib_mutex_destruct(mutex) (ZYLIB_OK)
#define zylib_mutex_lock(mutex) (ZYLIB_OK)
#define zylib_mutex_unlock(mutex) (ZYLIB_OK)

#endif