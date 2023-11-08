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
#include <stdio.h>

/**
 * Logger Data Structure
 */
typedef struct zylib_logger_s zylib_logger_t;

/**
 * Logger Severity Data Type.
 * Highest => 0.
 */
typedef enum zylib_logger_severity_e
{
    /**
     * Irrecoverable Error
     */
    ZYLIB_FATAL,
    /**
     * Recoverable Error
     */
    ZYLIB_ERROR,
    /**
     * Warning
     */
    ZYLIB_WARN,
    /**
     * Informational
     */
    ZYLIB_INFO
} zylib_logger_severity_t;

/**
 * Logger Output Format
 */
typedef enum zylib_logger_format_e
{
    /**
     * Human Readable (text)
     */
    ZYLIB_LOGGER_FORMAT_PLAINTEXT,
    /**
     * Comma-Separated Values (CSV)
     */
    ZYLIB_LOGGER_FORMAT_CSV,
    /**
     * Extensible Markup Format (XML)
     */
    ZYLIB_LOGGER_FORMAT_XML
} zylib_logger_format_t;

/**
 * The Lowest Defined Logger Severity.
 */
#define ZYLIB_LOGGER_SEVERITY_LOWEST (ZYLIB_INFO)

/**
 * The Highest Defined Logger Severity.
 */
#define ZYLIB_LOGGER_SEVERITY_HIGHEST (ZYLIB_FATAL)

#define ZYLIB_LOGGER_FORMAT_LAST (ZYLIB_LOGGER_FORMAT_XML)

#define ZYLIB_LOGGER_MAX_MESSAGE_SIZE_DEFAULT (1024U)
#define ZYLIB_LOGGER_TIME_FORMAT_DEFAULT ("%a %b %d %H:%M:%S %Z %Y")

/* DATE FILE:LINE (FUNCTION) [SEVERITY] MESSAGE\n */
#define ZYLIB_LOGGER_PLAINTEXT_FORMAT_DEFAULT ("%s %s:%zu (%s) [%s] %s\n")
/* DATE,FILE,LINE,FUNCTION,SEVERITY,MESSAGE */
#define ZYLIB_LOGGER_CSV_FORMAT_DEFAULT ("%s,%s,%zu,%s,%s,%s\n")
#define ZYLIB_LOGGER_XML_FORMAT_DEFAULT                                                                                \
    ("<message severity='%s'><date>%s</date><location file='%s' line='%zu' "                                           \
     "function='%s'/><text>%s</text></message>\n")

#define ZYLIB_LOGGER_ERROR(log, format, ...)                                                                           \
    zylib_logger_write(log, ZYLIB_ERROR, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define ZYLIB_LOGGER_WARN(log, format, ...)                                                                            \
    zylib_logger_write(log, ZYLIB_WARN, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define ZYLIB_LOGGER_INFO(log, format, ...)                                                                            \
    zylib_logger_write(log, ZYLIB_INFO, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

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
