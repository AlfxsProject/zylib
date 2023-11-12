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
#include "zylib_logger.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>

struct zylib_logger_s
{
    const zylib_allocator_t *allocator;
    FILE *logger_file;
    zylib_logger_severity_t logger_severity;
    zylib_logger_format_t logger_format;
};

static const char *severity_string[] = {"FATAL", "ERROR", "WARNING", "INFO"};

static inline uint64_t zylib_logger_send_message(const zylib_logger_t *log, zylib_logger_severity_t severity,
                                                 const char *file, uint64_t line, const char *function,
                                                 const char *display_message)
{
    char date_buf[120] = {0};
    struct tm tm = {0};
    time_t now = time(NULL);
    uint64_t size = 0;

    assert(severity >= 0 && severity < (sizeof(severity_string) / sizeof(const char *)));

    if (now == (time_t)-1)
    {
        goto send;
    }

    if (localtime_r(&now, &tm) == NULL)
    {
        goto send;
    }

    strftime(date_buf, sizeof(date_buf), ZYLIB_LOGGER_TIME_FORMAT, &tm);

send:
    switch (log->logger_format)
    {
    case ZYLIB_LOGGER_FORMAT_CSV:
        size = (uint64_t)fprintf(log->logger_file, ZYLIB_LOGGER_CSV_FORMAT, date_buf, file, line, function,
                                 severity_string[severity], display_message);
        break;
    case ZYLIB_LOGGER_FORMAT_XML:
        size = (uint64_t)fprintf(log->logger_file, ZYLIB_LOGGER_XML_FORMAT, severity_string[severity], date_buf, file,
                                 line, function, display_message);
        break;
    case ZYLIB_LOGGER_FORMAT_PLAINTEXT:
        size = (uint64_t)fprintf(log->logger_file, ZYLIB_LOGGER_PLAINTEXT_FORMAT, date_buf, file, line, function,
                                 severity_string[severity], display_message);
        break;
    }
    return size;
}

_Bool zylib_logger_construct(zylib_logger_t **log, const zylib_allocator_t *alloc, FILE *file,
                             zylib_logger_severity_t severity, zylib_logger_format_t format)
{
    _Bool r;
    if (severity >= ZYLIB_LOGGER_SEVERITY_N || format >= ZYLIB_LOGGER_FORMAT_N)
    {
        return 0;
    }
    r = zylib_allocator_malloc(alloc, sizeof(zylib_logger_t), (void **)log);
    if (r)
    {
        (*log)->allocator = alloc;
        (*log)->logger_file = file;
        (*log)->logger_severity = severity;
        (*log)->logger_format = format;
    }
    return r;
}

void zylib_logger_destruct(zylib_logger_t **log)
{
    if (*log != NULL)
    {
        fclose((*log)->logger_file);
        zylib_allocator_free((const zylib_allocator_t *)(*log)->allocator, (void **)log);
    }
}

/*
 * Print message
 * TIME, FILE, LINE, FUNCTION, SEVERITY, MESSAGE
 */
uint64_t zylib_logger_write(const zylib_logger_t *log, zylib_logger_severity_t severity, const char *file,
                            uint64_t line, const char *function, const char *format, ...)
{
    if (severity <= log->logger_severity)
    {
        uint64_t r = 0;
        va_list args;
        char *display_message = NULL;

        if (!zylib_allocator_malloc(log->allocator, ZYLIB_LOGGER_MAX_MESSAGE_SIZE, (void **)&display_message))
        {
            goto error;
        }

        va_start(args, format);
        vsnprintf(display_message, ZYLIB_LOGGER_MAX_MESSAGE_SIZE, format, args);
        va_end(args);

        r = zylib_logger_send_message(log, severity, file, line, function, display_message);
    error:
        if (display_message != NULL)
        {
            zylib_allocator_free(log->allocator, (void **)&display_message);
        }
        return r;
    }
    return 0;
}
