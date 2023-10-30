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

/*
 * Types
 */

typedef struct zylib_err_bx_s zylib_err_bx_t;
typedef struct zylib_err_s zylib_err_t;

/*
 * Functions
 */

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((nonnull)) int zylib_err_construct(zylib_err_t **err, const zylib_alloc_t *alloc);
    __attribute__((nonnull)) void zylib_err_destruct(zylib_err_t **err);
    __attribute__((nonnull)) void zylib_err_clear(zylib_err_t *err);
    __attribute__((nonnull(1, 3, 5))) int zylib_err_push_first(zylib_err_t *err, int64_t code, const char *file,
                                                               size_t line, const char *function, const void *opaque,
                                                               size_t opaque_size);
    __attribute__((nonnull(1, 3, 5))) int zylib_err_push_last(zylib_err_t *err, int64_t code, const char *file,
                                                              size_t line, const char *function, const void *opaque,
                                                              size_t opaque_size);
    __attribute__((nonnull)) void zylib_err_discard_first(zylib_err_t *err);
    __attribute__((nonnull)) void zylib_err_discard_last(zylib_err_t *err);
    __attribute__((nonnull)) zylib_err_bx_t *zylib_err_peek_first(const zylib_err_t *err);
    __attribute__((nonnull)) zylib_err_bx_t *zylib_err_peek_last(const zylib_err_t *err);
    __attribute__((nonnull)) size_t zylib_err_size(const zylib_err_t *err);
    __attribute__((nonnull)) bool zylib_err_is_empty(const zylib_err_t *err);
    __attribute__((nonnull)) int64_t zylib_err_bx_code(const zylib_err_bx_t *bx);
    __attribute__((nonnull)) const char *zylib_err_bx_file(const zylib_err_bx_t *bx);
    __attribute__((nonnull)) size_t zylib_err_bx_line(const zylib_err_bx_t *bx);
    __attribute__((nonnull)) const char *zylib_err_bx_function(const zylib_err_bx_t *bx);
    __attribute__((nonnull(1))) const void *zylib_err_bx_opaque(const zylib_err_bx_t *bx, size_t *size);

#ifdef __cplusplus
}
#endif