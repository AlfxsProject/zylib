# Allocator Plugin API

## LIBRARY

zylib (-lzylib)

## SYNOPSIS

```C
typedef void *zylib_allocator_t;
typedef void *(*zylib_allocator_malloc_t)(size_t size);
typedef void *(*zylib_allocator_realloc_t)(void *ptr, size_t size);
typedef void (*zylib_allocator_free_t)(void *ptr);

_Bool zylib_allocator_construct(zylib_allocator_t **obj, zylib_allocator_malloc_t malloc,
                                zylib_allocator_realloc_t realloc, zylib_allocator_free_t free);
void zylib_allocator_destruct(zylib_allocator_t **obj);
_Bool zylib_allocator_malloc(const zylib_allocator_t *obj, size_t size, void **ptr);
_Bool zylib_allocator_realloc(const zylib_allocator_t *obj, size_t size, void **ptr);
void zylib_allocator_free(const zylib_allocator_t *obj, void **ptr);
```

## DESCRIPTION

## RETURN VALUE

## ERRORS

## NOTES