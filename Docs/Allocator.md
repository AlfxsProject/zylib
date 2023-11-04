# Allocator Plugin API

## LIBRARY

zylib (-lzylib)

## SYNOPSIS

```C
#include <zylib_allocator.h>

/*
 * Type Definitions
 */

typedef struct zylib_allocator_s zylib_allocator_t;
typedef void *(*zylib_allocator_malloc_t)(size_t size);
typedef void *(*zylib_allocator_realloc_t)(void *ptr, size_t size);
typedef void (*zylib_allocator_free_t)(void *ptr);

/*
 * Function Declarations
 */

_Bool zylib_allocator_construct(zylib_allocator_t **allocator, zylib_allocator_malloc_t malloc,
                                zylib_allocator_realloc_t realloc, zylib_allocator_free_t free);
void zylib_allocator_destruct(zylib_allocator_t **allocator);
_Bool zylib_allocator_malloc(const zylib_allocator_t *allocator, size_t size, void **p_void);
_Bool zylib_allocator_realloc(const zylib_allocator_t *allocator, size_t size, void **p_void);
void zylib_allocator_free(const zylib_allocator_t *allocator, void **p_void);
```

## DESCRIPTION

## RETURN VALUE

## ERRORS

## NOTES