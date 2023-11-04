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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wzero-length-array"
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct zylib_box_s
{
    uint64_t size;
    unsigned char data[0];
} zylib_box_t;

typedef enum zylib_format_e
{
    ZYLIB_FORMAT_PLAIN,
    ZYLIB_FORMAT_CSV,
    ZYLIB_FORMAT_XML
} zylib_format_t;

typedef enum zylib_return_e
{
    ZYLIB_OK,
    ZYLIB_ERROR_UNKNOWN,
    ZYLIB_ERROR_OOM,
    ZYLIB_ERROR_DEADLOCK,
    ZYLIB_ERROR_UNINITIALIZED,
    ZYLIB_ERROR_BUSY,
    ZYLIB_ERROR_PERM,
    ZYLIB_ERROR_INPUT_VALUE
} zylib_return_t;

#pragma clang diagnostic pop
