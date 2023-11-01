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

typedef struct zylib_dequeue_s zylib_dequeue_t;

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((nonnull)) zylib_return_t zylib_dequeue_construct(zylib_dequeue_t **dqe, const zylib_alloc_t *alloc);
    __attribute__((nonnull)) void zylib_dequeue_destruct(zylib_dequeue_t **dqe);
    __attribute__((nonnull)) void zylib_dequeue_clear(zylib_dequeue_t *dqe);
    __attribute__((nonnull)) zylib_return_t zylib_dequeue_push_first(zylib_dequeue_t *dqe, const zylib_box_t *box);
    __attribute__((nonnull)) zylib_return_t zylib_dequeue_push_last(zylib_dequeue_t *dqe, const zylib_box_t *box);
    __attribute__((nonnull)) void zylib_dequeue_discard_first(zylib_dequeue_t *dqe);
    __attribute__((nonnull)) void zylib_dequeue_discard_last(zylib_dequeue_t *dqe);
    __attribute__((nonnull)) const zylib_box_t *zylib_dequeue_peek_first(const zylib_dequeue_t *dqe);
    __attribute__((nonnull)) const zylib_box_t *zylib_dequeue_peek_last(const zylib_dequeue_t *dqe);
    __attribute__((nonnull)) size_t zylib_dequeue_size(const zylib_dequeue_t *dqe);
    __attribute__((nonnull)) bool zylib_dequeue_is_empty(const zylib_dequeue_t *dqe);

#ifdef __cplusplus
}
#endif