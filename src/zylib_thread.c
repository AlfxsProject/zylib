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
#ifdef DO_MUTEX
#include "zylib_thread.h"

#ifdef DO_PTHREAD
#include <errno.h>
#include <pthread.h>
#else
#error "DO_MUTEX was specified without a supported implementation (e.g. DO_PTHREAD)."
#endif

struct zylib_mutex_s
{
    const zylib_alloc_t *alloc;
#ifdef DO_PTHREAD
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
#endif
};

zylib_return_t zylib_mutex_construct(zylib_mutex_t **mutex, const zylib_alloc_t *alloc)
{
    int r = zylib_malloc(alloc, sizeof(zylib_mutex_t), (void **)mutex);
    if (r == ZYLIB_OK)
    {
        (*mutex)->alloc = alloc;
#ifdef DO_PTHREAD
        pthread_mutex_init(&(*mutex)->mutex, nullptr);
        pthread_mutexattr_init(&(*mutex)->attr);
        pthread_mutexattr_settype(&(*mutex)->attr, PTHREAD_MUTEX_ERRORCHECK);
#endif
    }
    return r;
}
zylib_return_t zylib_mutex_destruct(zylib_mutex_t **mutex)
{
    if (*mutex != nullptr)
    {
#ifdef DO_PTHREAD
        int r = pthread_mutex_destroy(&(*mutex)->mutex);
        switch (r)
        {
        case EBUSY:
            return ZYLIB_ERROR_BUSY;
        default:
        case 0:
            break;
        }
        pthread_mutexattr_destroy(&(*mutex)->attr);
#endif
        zylib_free((*mutex)->alloc, (void **)mutex);
    }
    return ZYLIB_OK;
}

zylib_return_t zylib_mutex_lock(zylib_mutex_t *mutex)
{
#ifdef DO_PTHREAD
    int r = pthread_mutex_lock(&mutex->mutex);
    switch (r)
    {
    case 0:
        return ZYLIB_OK;
    case EDEADLK:
        return ZYLIB_ERROR_DEADLOCK;
    case EINVAL:
        return ZYLIB_ERROR_UNINITIALIZED;
    default:
        return ZYLIB_ERROR_UNKNOWN;
    }
#endif
}

zylib_return_t zylib_mutex_unlock(zylib_mutex_t *mutex)
{
#ifdef DO_PTHREAD
    int r = pthread_mutex_unlock(&mutex->mutex);
    switch (r)
    {
    case 0:
        return ZYLIB_OK;
    case EPERM:
        return ZYLIB_ERROR_PERM;
    case EINVAL:
        return ZYLIB_ERROR_UNINITIALIZED;
    default:
        return ZYLIB_ERROR_UNKNOWN;
    }
#endif
}

#endif