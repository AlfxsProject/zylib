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
#include "zylib_private_allocator.h"
#include <stdint.h>

/**
 * The Error Container Data Structure
 */
typedef struct zylib_private_error_box_s zylib_private_error_box_t;

/**
 * The Error Dequeue Data Structure
 */
typedef struct zylib_private_error_s zylib_private_error_t;

ZYLIB_BEGIN_DECLS

/**
 * Construct an error dequeue
 * @param obj The error dequeue object
 * @param allocator The allocator object
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_error_construct(zylib_private_error_t **obj, const zylib_private_allocator_t *allocator);

/**
 * Deconstruct an error dequeue
 * @param obj The error dequeue object
 */
ZYLIB_NONNULL
void zylib_private_error_destruct(zylib_private_error_t **obj);

/**
 * Deconstruct all error containers within an error dequeue
 * @param obj The error dequeue object
 */
ZYLIB_NONNULL
void zylib_private_error_clear(zylib_private_error_t *obj);

/**
 * Insert an error container at the beginning of an error dequeue
 * @param obj The error dequeue object
 * @param error_code The error code
 * @param file_name The file name string literal
 * @param line_number The line number
 * @param function_name The function name string literal
 * @param auxiliary_size The size of the auxiliary data
 * @param auxiliary_data The auxiliary data
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL_N(1)
ZYLIB_NONNULL_N(3)
ZYLIB_NONNULL_N(5)
_Bool zylib_private_error_push_first(zylib_private_error_t *obj, int64_t error_code, const char *file_name,
                                     uint64_t line_number, const char *function_name, uint64_t auxiliary_size,
                                     const void *auxiliary_data);

/**
 * Insert an error container at the end of an error dequeue
 * @param obj The error dequeue object
 * @param error_code The error code
 * @param file_name The file name string literal
 * @param line_number The line number
 * @param function_name The function name string literal
 * @param auxiliary_size The size of the auxiliary data
 * @param auxiliary_data The auxiliary data
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL_N(1)
ZYLIB_NONNULL_N(3)
ZYLIB_NONNULL_N(5)
_Bool zylib_private_error_push_last(zylib_private_error_t *obj, int64_t error_code, const char *file_name,
                                    uint64_t line_number, const char *function_name, uint64_t auxiliary_size,
                                    const void *auxiliary_data);

/**
 * Deconstruct the error container at the beginning of an error dequeue
 * @param obj The error dequeue object
 */
ZYLIB_NONNULL
void zylib_private_error_discard_first(zylib_private_error_t *obj);

/**
 * Deconstruct the error container at the end of an error dequeue
 * @param obj The error dequeue object
 */
ZYLIB_NONNULL
void zylib_private_error_discard_last(zylib_private_error_t *obj);

/**
 * Retrieve the error container at the beginning of an error dequeue
 * @param obj The error dequeue
 * @return A non-null pointer to a valid error container if such a container exists; otherwise null.
 */
ZYLIB_NONNULL
const zylib_private_error_box_t *zylib_private_error_peek_first(const zylib_private_error_t *obj);

/**
 * Retrieve the error container at the end of an error dequeue
 * @param obj The error dequeue
 * @return A non-null pointer to a valid error container if such a container exists; otherwise null.
 */
ZYLIB_NONNULL
const zylib_private_error_box_t *zylib_private_error_peek_last(const zylib_private_error_t *obj);

/**
 * Retrieve the number of error containers within an error dequeue
 * @param obj The error dequeue object
 * @return The number of error containers
 */
ZYLIB_NONNULL
uint64_t zylib_private_error_size(const zylib_private_error_t *obj);

/**
 * Retrieve whether or not there are any error containers stored in an error dequeue
 * @param obj The error dequeue object
 * @return True if and only if the object is empty
 */
ZYLIB_NONNULL
_Bool zylib_private_error_is_empty(const zylib_private_error_t *obj);

/**
 * Retrieve the error code from an error container
 * @param obj The error container object
 * @return The error code
 */
ZYLIB_NONNULL
int64_t zylib_private_error_box_peek_error_code(const zylib_private_error_box_t *obj);

/**
 * Retrieve the file name string from an error container
 * @param obj The error container object
 * @return The file name string
 */
ZYLIB_NONNULL
const char *zylib_private_error_box_peek_file_name(const zylib_private_error_box_t *obj);

/**
 * Retrieve the line number from an error container
 * @param obj The error container object
 * @return The line number
 */
ZYLIB_NONNULL
uint64_t zylib_private_error_box_peek_line_number(const zylib_private_error_box_t *obj);

/**
 * Retrieve the function name string from an error container
 * @param obj The error container object
 * @return The function name string
 */
ZYLIB_NONNULL
const char *zylib_private_error_box_peek_function_name(const zylib_private_error_box_t *obj);

/**
 * Retrieve the size of the auxiliary data from an error container
 * @param obj The error container object
 * @return The auxiliary data size
 */
ZYLIB_NONNULL
uint64_t zylib_private_error_box_peek_auxiliary_size(const zylib_private_error_box_t *obj);

/**
 * Retrieve the auxiliary data from an error container
 * @param obj The error container object
 * @return The auxiliary data
 */
ZYLIB_NONNULL
const void *zylib_private_error_box_peek_auxiliary_data(const zylib_private_error_box_t *obj);

ZYLIB_END_DECLS
