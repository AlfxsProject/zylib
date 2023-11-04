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

struct zylib_log_s
{
    const zylib_allocator_t *alloc;
    FILE *log_file;
    zylib_log_severity_t log_severity;
    zylib_log_format_t log_format;
};

_Bool zylib_log_construct(zylib_log_t **log, const zylib_allocator_t *alloc, const char *path,
                          zylib_log_severity_t severity, zylib_log_format_t format)
{
    _Bool r = 0;
    FILE *file = NULL;
    if (severity > ZYLIB_LOG_SEVERITY_MAX)
    {
        // ZYLIB_ERROR_INPUT_VALUE;
        goto error;
    }
    if (format > ZYLIB_LOG_OUTPUT_FORMAT_MAX)
    {
        // ZYLIB_ERROR_INPUT_VALUE;
        goto error;
    }
    if ((file = fopen(path, "w")) == NULL)
    {
        // ZYLIB_ERROR_OOM;
        goto error;
    }
    r = zylib_allocator_malloc(alloc, sizeof(zylib_log_t), (void **)log);
    if (r)
    {
        (*log)->alloc = alloc;
        (*log)->log_file = file;
        (*log)->log_severity = severity;
        (*log)->log_format = format;
    }
    goto done;
error:
    if (file != NULL)
    {
        fclose(file);
        file = NULL;
    }
done:
    return r;
}

void zylib_log_destruct(zylib_log_t **log)
{
    if (*log != NULL)
    {
        fclose((*log)->log_file);
        zylib_allocator_free((const zylib_allocator_t *)(*log)->alloc, (void **)log);
    }
}

/*
 * Print message
 * TIME, FILE, LINE, FUNCTION, SEVERITY, MESSAGE
 */
size_t zylib_log_write(const zylib_log_t *log, zylib_log_severity_t severity, const char *file, size_t line,
                       const char *function, const char *format, ...)
{
    if (severity <= log->log_severity)
    {
        char *user_message = NULL, *log_message = NULL;
        size_t r = 0;
        if (zylib_allocator_malloc(log->alloc, ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT, (void **)&user_message) &&
            zylib_allocator_malloc(log->alloc, ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT, (void **)&log_message))
        {
            static const char *error_string = "";
            char date_buf[120] = {0};
            size_t offset;
            struct tm tm;
            va_list args;
            time_t now;

            va_start(args, format);
            vsnprintf(user_message, ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT, format, args);
            va_end(args);

            switch (severity)
            {
            case ZYLIB_FATAL:
                error_string = "FATAL";
                break;
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

            strftime(date_buf, sizeof(date_buf), ZYLIB_LOG_TIME_FORMAT_DEFAULT, &tm);

        write:
            switch (log->log_format)
            {
            case ZYLIB_LOG_FORMAT_CSV:
                offset = (size_t)snprintf(log_message, ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT,
                                          ZYLIB_LOG_CSV_OUTPUT_FORMAT_DEFAULT, date_buf, file, line, function,
                                          error_string, user_message);
                break;
            case ZYLIB_LOG_FORMAT_XML:
                offset = (size_t)snprintf(log_message, ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT,
                                          ZYLIB_LOG_XML_OUTPUT_FORMAT_DEFAULT, error_string, date_buf, file, line,
                                          function, user_message);
                break;
            case ZYLIB_LOG_FORMAT_PLAIN:
                offset = (size_t)snprintf(log_message, ZYLIB_LOG_MAX_MESSAGE_SIZE_DEFAULT,
                                          ZYLIB_LOG_PLAIN_OUTPUT_FORMAT_DEFAULT, date_buf, file, line, function,
                                          error_string, user_message);
                break;
            }

            r = (size_t)fwrite(log_message, 1, offset, log->log_file);
        }
        if (user_message != NULL)
        {
            zylib_allocator_free(log->alloc, (void **)&user_message);
        }
        if (log_message != NULL)
        {
            zylib_allocator_free(log->alloc, (void **)&log_message);
        }
        return r;
    }
    return 0;
}
