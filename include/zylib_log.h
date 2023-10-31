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

typedef struct zylib_log_s zylib_log_t;

typedef enum zylib_log_severity_e
{
    ZYLIB_ERROR,
    ZYLIB_WARN,
    ZYLIB_INFO
} zylib_log_severity_t;

/*
 * Constants
 */

#define ZYLIB_LOG_MAX_MESSAGE_SEVERITY_DEFAULT (ZYLIB_ERROR)
#define ZYLIB_LOG_MAX_MESSAGE_SEVERITY_MAX (ZYLIB_INFO)

#define ZYLIB_LOG_OUTPUT_FORMAT_DEFAULT (ZYLIB_FORMAT_PLAIN)
#define ZYLIB_LOG_OUTPUT_FORMAT_MAX (ZYLIB_FORMAT_XML)
#define ZYLIB_LOG_PLAIN_OUTPUT_FORMAT_DEFAULT ("%s %s:%zu (%s) [%s] %s\n")
#define ZYLIB_LOG_CSV_OUTPUT_FORMAT_DEFAULT ("%s,%s,%zu,%s,%s,%s\n")
#define ZYLIB_LOG_XML_OUTPUT_FORMAT_DEFAULT                                                                            \
    ("<message severity='%s'><date>%s</date><location file='%s' line='%zu' function='%s'/><text>%s</text></message>\n")

#define ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT (2048U)
#define ZYLIB_LOG_MAX_MESSAGE_SIZE_MIN (1024U)
#define ZYLIB_LOG_MAX_MESSAGE_SIZE_MAX (65536U)

#define ZYLIB_LOG_TIME_FORMAT_DEFAULT ("%a %b %d %H:%M:%S %Z %Y")

/*
 * Macros
 */

#define zylib_log_error(log, format, ...)                                                                                 \
    zylib__log_write(log, ZYLIB_ERROR, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define zylib_log_warn(log, format, ...)                                                                                  \
    zylib__log_write(log, ZYLIB_WARN, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)
#define zylib_log_info(log, format, ...)                                                                                  \
    zylib__log_write(log, ZYLIB_INFO, __FILE__, __LINE__, __func__, format, ##__VA_ARGS__)

/*
 * Functions
 */

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((nonnull)) zylib_return_t zylib_log_construct(zylib_log_t **log, const zylib_alloc_t *alloc,
                                                                int file_descriptor);
    __attribute__((nonnull)) void zylib_log_destruct(zylib_log_t **log);

    __attribute__((nonnull)) zylib_return_t zylib_log_set_max_message_size(zylib_log_t *log, size_t size);
    __attribute__((nonnull)) size_t zylib_log_get_max_message_size(const zylib_log_t *log);

    __attribute__((nonnull)) zylib_return_t zylib_log_set_max_severity(zylib_log_t *log, zylib_log_severity_t max);
    __attribute__((nonnull)) zylib_log_severity_t zylib_log_get_max_severity(const zylib_log_t *log);

    __attribute__((nonnull)) zylib_return_t zylib_log_set_output_format(zylib_log_t *log, zylib_format_t format);
    __attribute__((nonnull)) zylib_format_t zylib_log_get_output_format(const zylib_log_t *log);

    __attribute__((nonnull)) zylib_return_t zylib_log_set_time_format(zylib_log_t *log, const char *format);
    __attribute__((nonnull)) const char *zylib_log_get_time_format(const zylib_log_t *log);

    /* Internal Use Only. */
    __attribute__((nonnull)) __attribute__((format(printf, 6, 7))) zylib_return_t zylib__log_write(
        const zylib_log_t *log, zylib_log_severity_t severity, const char *file, size_t line, const char *function,
        const char *format, ...);

#ifdef __cplusplus
}
#endif