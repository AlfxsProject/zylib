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

/*
 * Types
 */

typedef struct zylib_log_s zylib_log_t;

typedef enum zylib_log_severity_e
{
    ZYLIB_FATAL,
    ZYLIB_ERROR,
    ZYLIB_WARN,
    ZYLIB_INFO
} zylib_log_severity_t;

typedef enum zylib_log_format_e
{
    ZYLIB_LOG_FORMAT_PLAIN,
    ZYLIB_LOG_FORMAT_CSV,
    ZYLIB_LOG_FORMAT_XML
} zylib_log_format_t;

/*
 * Constants
 */

#define ZYLIB_LOG_SEVERITY_MAX (ZYLIB_INFO)
#define ZYLIB_LOG_OUTPUT_FORMAT_MAX (ZYLIB_LOG_FORMAT_XML)

#define ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT (1024U)
#define ZYLIB_LOG_TIME_FORMAT_DEFAULT ("%a %b %d %H:%M:%S %Z %Y")

/* DATE FILE:LINE (FUNCTION) [SEVERITY] MESSAGE\n */
#define ZYLIB_LOG_PLAIN_OUTPUT_FORMAT_DEFAULT ("%s %s:%zu (%s) [%s] %s\n")
/* DATE,FILE,LINE,FUNCTION,SEVERITY,MESSAGE */
#define ZYLIB_LOG_CSV_OUTPUT_FORMAT_DEFAULT ("%s,%s,%zu,%s,%s,%s\n")
#define ZYLIB_LOG_XML_OUTPUT_FORMAT_DEFAULT                                                                            \
    ("<message severity='%s'><date>%s</date><location file='%s' line='%zu' "                                           \
     "function='%s'/><text>%s</text></message>\n")

/*
 * Macros
 */

#define ZYLIB_LOG_ERROR(log, format, ...)                                                                              \
    zylib_log_write(log, ZYLIB_ERROR, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define ZYLIB_LOG_WARN(log, format, ...)                                                                               \
    zylib_log_write(log, ZYLIB_WARN, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define ZYLIB_LOG_INFO(log, format, ...)                                                                               \
    zylib_log_write(log, ZYLIB_INFO, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

/*
 * Functions
 */

ZYLIB_BEGIN_DECLS

ZYLIB_NONNULL
_Bool zylib_log_construct(zylib_log_t **log, const zylib_allocator_t *alloc, FILE *file, zylib_log_severity_t severity,
                          zylib_log_format_t format);

ZYLIB_NONNULL
void zylib_log_destruct(zylib_log_t **log);

ZYLIB_PRINTF_LIKE(6, 7)
ZYLIB_NONNULL
size_t zylib_log_write(const zylib_log_t *log, zylib_log_severity_t severity, const char *file, size_t line,
                       const char *function, const char *format, ...);

ZYLIB_END_DECLS
