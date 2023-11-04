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
#include "zylib_log.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct zylib_log_s
{
    const zylib_alloc_t *alloc;
    int fd;
    size_t max_message_size;
    zylib_log_severity_t max_severity;
    zylib_format_t output_format;
    const char *time_format;
};

zylib_return_t zylib_log_construct(zylib_log_t **log, const zylib_alloc_t *alloc, int file_descriptor)
{
    zylib_return_t r = zylib_malloc(alloc, sizeof(zylib_log_t), (void **)log);
    if (r == ZYLIB_OK)
    {
        (*log)->alloc = alloc;
        (*log)->fd = file_descriptor;
        (*log)->max_message_size = ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT;
        (*log)->max_severity = ZYLIB_LOG_MAX_MESSAGE_SEVERITY_DEFAULT;
        (*log)->output_format = ZYLIB_LOG_OUTPUT_FORMAT_DEFAULT;
        (*log)->time_format = ZYLIB_LOG_TIME_FORMAT_DEFAULT;
    }
    return r;
}

void zylib_log_destruct(zylib_log_t **log)
{
    if (*log != NULL)
    {
        zylib_free((const zylib_alloc_t *)(*log)->alloc, (void **)log);
    }
}

zylib_return_t zylib_log_set_max_message_size(zylib_log_t *log, size_t size)
{
    if (size >= ZYLIB_LOG_MAX_MESSAGE_SIZE_MIN && size <= ZYLIB_LOG_MAX_MESSAGE_SIZE_MAX)
    {
        log->max_message_size = size;
        return ZYLIB_OK;
    }
    return ZYLIB_ERROR_INPUT_VALUE;
}

size_t zylib_log_get_max_message_size(const zylib_log_t *log)
{
    size_t message_size = log->max_message_size;
    return message_size;
}

zylib_return_t zylib_log_set_max_severity(zylib_log_t *log, zylib_log_severity_t max)
{
    if (max <= ZYLIB_LOG_MAX_MESSAGE_SEVERITY_MAX)
    {
        log->max_severity = max;
        return ZYLIB_OK;
    }
    return ZYLIB_ERROR_INPUT_VALUE;
}

zylib_log_severity_t zylib_log_get_max_severity(const zylib_log_t *log)
{
    zylib_log_severity_t max_message_type = log->max_severity;
    return max_message_type;
}

zylib_return_t zylib_log_set_output_format(zylib_log_t *log, zylib_format_t format)
{
    if (format <= ZYLIB_LOG_OUTPUT_FORMAT_MAX)
    {
        log->output_format = format;
        return ZYLIB_OK;
    }
    return ZYLIB_ERROR_INPUT_VALUE;
}

zylib_format_t zylib_log_get_output_format(const zylib_log_t *log)
{
    zylib_format_t output_format = log->output_format;
    return output_format;
}

zylib_return_t zylib_log_set_time_format(zylib_log_t *log, const char *format)
{
    log->time_format = format;
    return ZYLIB_OK;
}
const char *zylib_log_get_time_format(const zylib_log_t *log)
{
    const char *const time_format = log->time_format;
    return time_format;
}

/*
 * Print message
 * TIME, FILE, LINE, FUNCTION, SEVERITY, MESSAGE
 */
size_t zylib_log_write(const zylib_log_t *log, zylib_log_severity_t severity, const char *file, size_t line,
                       const char *function, const char *format, ...)
{
    const zylib_log_severity_t max_severity = log->max_severity;
    const zylib_format_t output_format = log->output_format;
    const char *const time_format = log->time_format;
    const size_t max_message_size = log->max_message_size;

    if (severity <= max_severity)
    {
        char *user_message = NULL, *log_message = NULL;
        size_t r = 0;
        if (zylib_malloc(log->alloc, max_message_size, (void **)&user_message) == ZYLIB_OK &&
            zylib_malloc(log->alloc, max_message_size, (void **)&log_message) == ZYLIB_OK)
        {
            static const char *error_string = "";
            char date_buf[120] = {0};
            size_t offset;
            struct tm tm;
            va_list args;
            time_t now;

            va_start(args, format);
            vsnprintf(user_message, max_message_size, format, args);
            va_end(args);

            switch (severity)
            {
            case ZYLIB_ERROR:
                error_string = "ERROR";
                break;
            case ZYLIB_WARN:
                error_string = "WARNING";
                break;
            case ZYLIB_INFO:
                error_string = "INFO";
                break;
            }

            now = time(NULL);

            if (now == (time_t)-1)
            {
                goto write;
            }

            if (localtime_r(&now, &tm) == NULL)
            {
                goto write;
            }

            strftime(date_buf, sizeof(date_buf), time_format, &tm);

        write:
            switch (output_format)
            {
            case ZYLIB_FORMAT_CSV:
                offset = (size_t)snprintf(log_message, max_message_size, ZYLIB_LOG_CSV_OUTPUT_FORMAT_DEFAULT, date_buf,
                                          file, line, function, error_string, user_message);
                break;
            case ZYLIB_FORMAT_XML:
                offset = (size_t)snprintf(log_message, max_message_size, ZYLIB_LOG_XML_OUTPUT_FORMAT_DEFAULT,
                                          error_string, date_buf, file, line, function, user_message);
                break;
            case ZYLIB_FORMAT_PLAIN:
                offset = (size_t)snprintf(log_message, max_message_size, ZYLIB_LOG_PLAIN_OUTPUT_FORMAT_DEFAULT,
                                          date_buf, file, line, function, error_string, user_message);
                break;
            }

            r = (size_t)write(log->fd, log_message, offset);
        }
        if (user_message != NULL)
        {
            zylib_free(log->alloc, (void **)&user_message);
        }
        if (log_message != NULL)
        {
            zylib_free(log->alloc, (void **)&log_message);
        }
        return r;
    }
    return 0;
}
