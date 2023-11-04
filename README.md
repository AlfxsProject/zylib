# zylib

General Purpose Library, Written in C.

## Features

### Allocator Plugin API

The `allocator plugin API` provides the ability to supply custom allocators via `malloc`, `realloc`, and `free`.

### Error Dequeue API

The `error dequeue API` provides the ability to enhance error-handling capabilities. The API allows the user to
programmatically store and retrieve detailed information about the defect; this includes the file, line number, function
name, error code, and auxiliary data.

### Logger API

The `logger API` provides the ability to enhance understanding about the flow of software. The API allows the user to
store detailed information about the frame of interest; this includes the file, line number, function name, severity,
and formatted message. The levels of severity include fatal, error, warning, and info. Additionally, the logger API
supports multiple message formats, including plaintext, comma-separated values (CSV), and extensible markup language (
XML).

### Data Structures

Implementations for various common data structures are provided; these include double-ended queues (dequeue),
variable-length arrays (vector), self-balancing binary search trees (Ordered Set/Map), hash tables (Unordered Set/Map),
and circular double-ended linked lists (list).

## Support

The library is written in standard C. As such, all platforms and conforming compilers are supported.

## License

```text
 Copyright 2023 Alexandre Fernandez <alex@fernandezfamily.email>
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
```