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

#define ZYLIB_LOG_SEVERITY_MAX (ZYLIB_INFO)
#define ZYLIB_LOG_OUTPUT_FORMAT_MAX (ZYLIB_FORMAT_XML)

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

#ifdef __cplusplus
extern "C"
{
#endif

    __attribute__((nonnull)) zylib_return_t zylib_log_construct(zylib_log_t **log, const zylib_alloc_t *alloc,
                                                                const char *path, zylib_log_severity_t severity,
                                                                zylib_format_t format);
    __attribute__((nonnull)) void zylib_log_destruct(zylib_log_t **log);

    /* Internal Use Only. */
    __attribute__((nonnull)) __attribute__((format(printf, 6, 7))) size_t zylib_log_write(const zylib_log_t *log,
                                                                                          zylib_log_severity_t severity,
                                                                                          const char *file, size_t line,
                                                                                          const char *function,
                                                                                          const char *format, ...);

#ifdef __cplusplus
}
#endif
