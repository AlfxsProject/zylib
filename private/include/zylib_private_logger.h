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
#include "zylib_logger_def.h"
#include "zylib_private_allocator.h"
#include <stdint.h>
#include <stdio.h>

/**
 * Logger Data Structure
 */
typedef struct zylib_private_logger_s zylib_private_logger_t;

ZYLIB_BEGIN_DECLS

/**
 * Construct a logger object
 * @param logger The pointer to the logger object
 * @param alloc The allocator object
 * @param file The logger file
 * @param format The logger output format
 * @param filter The logger output filter
 * @return True if and only if the operation was successful
 */
ZYLIB_NONNULL
_Bool zylib_private_logger_construct(zylib_private_logger_t **logger, const zylib_private_allocator_t *alloc,
                                     FILE *file, zylib_logger_format_t format, zylib_logger_filter_t filter);

ZYLIB_NONNULL
void zylib_private_logger_destruct(zylib_private_logger_t **logger);

ZYLIB_PRINTF_LIKE(6, 7)
ZYLIB_NONNULL
uint64_t zylib_private_logger_write(const zylib_private_logger_t *logger, zylib_logger_severity_t severity,
                                    const char *file_name, uint64_t line_number, const char *function_name,
                                    const char *format, ...);

ZYLIB_END_DECLS
