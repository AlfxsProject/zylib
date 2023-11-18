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
    ZYLIB_INFO,
    /**
     * The Number of Severity Levels
     */
    ZYLIB_LOGGER_SEVERITY_N
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
    ZYLIB_LOGGER_FORMAT_XML,
    /**
     * The Number of Supported Formats
     */
    ZYLIB_LOGGER_FORMAT_N
} zylib_logger_format_t;

typedef _Bool (*zylib_logger_filter_t)(zylib_logger_severity_t severity);

/**
 * Format Maximum Message Size
 */
#define ZYLIB_LOGGER_MAX_MESSAGE_SIZE (1024U)

/**
 * Default Time Format
 */
#define ZYLIB_LOGGER_TIME_FORMAT ("%a %b %d %H:%M:%S %Z %Y")

/**
 * DATE FILE:LINE (FUNCTION) [SEVERITY] MESSAGE
 */
#define ZYLIB_LOGGER_PLAINTEXT_FORMAT ("%s %s:%zu (%s) [%s] %s\n")

/**
 * DATE,FILE,LINE,FUNCTION,SEVERITY,MESSAGE
 */
#define ZYLIB_LOGGER_CSV_FORMAT ("%s,%s,%zu,%s,%s,%s\n")

/**
 * Default XML Format
 */
#define ZYLIB_LOGGER_XML_FORMAT                                                                                        \
    ("<message severity='%s'><date>%s</date><location file='%s' line='%zu' "                                           \
     "function='%s'/><text>%s</text></message>\n")
