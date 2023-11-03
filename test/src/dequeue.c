#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <zylib_dequeue.h>
#include <zylib_log.h>

#define SIZE (512U)
#define ITERATIONS (10U)

#define PRINT(format, ...) ZYLIB_LOG_ERROR(log, format, ##__VA_ARGS__)

static zylib_log_t *log = nullptr;

bool test_empty(const zylib_dequeue_t *dqe)
{
    bool r = false;

    if (zylib_dequeue_size(dqe) != 0)
    {
        PRINT("ERROR: zylib_dequeue_size()");
        goto done;
    }

    if (!zylib_dequeue_is_empty(dqe))
    {
        PRINT("ERROR: zylib_dequeue_is_empty()");
        goto done;
    }

    r = true;
done:
    return r;
}

bool test_size_positive_n(const zylib_dequeue_t *dqe, size_t n)
{
    bool r = false;

    if (zylib_dequeue_size(dqe) != n)
    {
        PRINT("ERROR: zylib_dequeue_size()");
        goto done;
    }

    if (zylib_dequeue_is_empty(dqe))
    {
        PRINT("ERROR: zylib_dequeue_is_empty()");
        goto done;
    }

    r = true;
done:
    return r;
}

bool test_loop(zylib_dequeue_t *dqe, zylib_return_t (*push)(zylib_dequeue_t *, const zylib_box_t *),
               const zylib_box_t *(*peek)(const zylib_dequeue_t *))
{
    bool r = false;

    if (!test_empty(dqe))
    {
        PRINT("ERROR: test_empty()");
        goto done;
    }

    for (unsigned int i = 0; i < ITERATIONS; ++i)
    {
        char buf[SIZE] = {0};
        zylib_box_t *const box = (zylib_box_t *)buf;
        box->size = sizeof(buf) - sizeof(zylib_box_t);

        getrandom(box->data, box->size, GRND_NONBLOCK);

        if (push(dqe, box) != ZYLIB_OK)
        {
            PRINT("ERROR: push()");
            goto done;
        }

        if (!test_size_positive_n(dqe, i + 1))
        {
            PRINT("ERROR: test_size_positive_n");
            goto done;
        }

        const zylib_box_t *const first = peek(dqe);

        if (box->size != first->size)
        {
            PRINT("ERROR: peek(); size");
            goto done;
        }

        if (memcmp(box->data, first->data, box->size) != 0)
        {
            PRINT("ERROR: peek(); data");
            goto done;
        }
    }

    zylib_dequeue_clear(dqe);

    if (!test_empty(dqe))
    {
        PRINT("ERROR: test_empty()");
        goto done;
    }

    r = true;
done:
    return r;
}

int main()
{
    int r = EXIT_FAILURE;

    zylib_alloc_t *alloc = nullptr;
    zylib_dequeue_t *dqe = nullptr;

    if (zylib_alloc_construct(&alloc, malloc, realloc, free) != ZYLIB_OK)
    {
        fprintf(stderr, "ERROR: zylib_alloc_construct()\n");
        goto done;
    }

    if (zylib_log_construct(&log, alloc, 2) != ZYLIB_OK)
    {
        fprintf(stderr, "ERROR: zylib_log_construct()\n");
        goto done;
    }

    if (zylib_dequeue_construct(&dqe, alloc) != ZYLIB_OK)
    {
        PRINT("ERROR: zylib_dequeue_construct()");
        goto done;
    }

    if (!test_loop(dqe, zylib_dequeue_push_first, zylib_dequeue_peek_first))
    {
        PRINT("ERROR: test_loop(); first");
        goto done;
    }

    if (!test_loop(dqe, zylib_dequeue_push_last, zylib_dequeue_peek_last))
    {
        PRINT("ERROR: test_loop(); last");
        goto done;
    }

    r = EXIT_SUCCESS;
done:
    if (log != nullptr)
    {
        zylib_log_destruct(&log);
    }
    if (dqe != nullptr)
    {
        zylib_dequeue_destruct(&dqe);
    }
    if (alloc != nullptr)
    {
        zylib_alloc_destruct(&alloc);
    }
    return r;
}