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
#include "zylib_allocator.h"
#include "zylib_logger_def.h"
#include <stdio.h>

/**
 * Print Fatal Error Message Formatted String
 */
#define ZYLIB_LOGGER_FATAL_F(log, format, ...)                                                                         \
    zylib_logger_write(log, ZYLIB_FATAL, __FILE__, __LINE__, __func__, format, __VA_ARGS__)

/**
 * Print Fatal Error Message String
 */
#define ZYLIB_LOGGER_FATAL(log, string) zylib_logger_write(log, ZYLIB_FATAL, __FILE__, __LINE__, __func__, format)

/**
 * Print Error Message Formatted String
 */
#define ZYLIB_LOGGER_ERROR_F(log, format, ...)                                                                         \
    zylib_logger_write(log, ZYLIB_ERROR, __FILE__, __LINE__, __func__, format, __VA_ARGS__)

/**
 * Print Error Message String
 */
#define ZYLIB_LOGGER_ERROR(log, format) zylib_logger_write(log, ZYLIB_ERROR, __FILE__, __LINE__, __func__, format)

/**
 * Print Warning Message Formatted String
 */
#define ZYLIB_LOGGER_WARN_F(log, format, ...)                                                                          \
    zylib_logger_write(log, ZYLIB_WARN, __FILE__, __LINE__, __func__, format, __VA_ARGS__)

/**
 * Print Warning Message String
 */
#define ZYLIB_LOGGER_WARN(log, format) zylib_logger_write(log, ZYLIB_WARN, __FILE__, __LINE__, __func__, format)

/**
 * Print Informational Message Formatted String
 */
#define ZYLIB_LOGGER_INFO_F(log, format, ...)                                                                          \
    zylib_logger_write(log, ZYLIB_INFO, __FILE__, __LINE__, __func__, format, __VA_ARGS__)

/**
 * Print Informational Message String
 */
#define ZYLIB_LOGGER_INFO(log, format) zylib_logger_write(log, ZYLIB_INFO, __FILE__, __LINE__, __func__, format)

ZYLIB_BEGIN_DECLS

ZYLIB_NONNULL
_Bool zylib_logger_construct(zylib_logger_t **log, const zylib_allocator_t *alloc, FILE *file,
                             zylib_logger_severity_t severity, zylib_logger_format_t format);

ZYLIB_NONNULL
void zylib_logger_destruct(zylib_logger_t **log);

ZYLIB_PRINTF_LIKE(6, 7)
ZYLIB_NONNULL
size_t zylib_logger_write(const zylib_logger_t *log, zylib_logger_severity_t severity, const char *file, size_t line,
                          const char *function, const char *format, ...);

ZYLIB_END_DECLS
