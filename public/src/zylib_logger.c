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
#include "zylib_private_logger.h"
#include <assert.h>
#include <stdarg.h>

_Bool zylib_logger_construct(zylib_logger_t **logger, const zylib_allocator_t *alloc, FILE *file,
                             zylib_logger_format_t format, zylib_logger_filter_t filter)
{
    assert(logger != NULL);
    assert(alloc != NULL);
    assert(file != NULL);
    assert(format < ZYLIB_LOGGER_FORMAT_N);
    assert(filter != NULL);
    return zylib_private_logger_construct((zylib_private_logger_t **)logger, (const zylib_private_allocator_t *)alloc,
                                          file, format, filter);
}

void zylib_logger_destruct(zylib_logger_t **logger)
{
    assert(logger != NULL);
    zylib_private_logger_destruct((zylib_private_logger_t **)logger);
}

/*
 * Print message
 * TIME, FILE, LINE, FUNCTION, SEVERITY, MESSAGE
 */
uint64_t zylib_logger_write(const zylib_logger_t *logger, zylib_logger_severity_t severity, const char *file_name,
                            uint64_t line_number, const char *function_name, const char *format, ...)
{
    uint64_t r;
    va_list args;

    assert(logger != NULL);
    assert(severity < ZYLIB_LOGGER_SEVERITY_N);
    assert(file_name != NULL);
    assert(function_name != NULL);
    assert(format != NULL);

    va_start(args, format);
    r = zylib_private_logger_write((const zylib_private_logger_t *)logger, severity, file_name, line_number,
                                   function_name, format, args);
    va_end(args);
    return r;
}
