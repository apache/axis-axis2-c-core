/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2.h>
#include "axis2_thread_windows.h"

AXIS2_DECLARE(axis2_threadattr_t*)
axis2_threadattr_create(axis2_allocator_t *allocator)
{
    axis2_threadattr_t *new = NULL;

    new = AXIS2_MALLOC(allocator, sizeof(axis2_threadattr_t));
	if(NULL == new)
	{
		/*AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)*/
		return NULL;
	}
    new->detach = 0;
	new->stacksize = 0;

    return new;
}

/* Destroy the threadattr object */
AXIS2_DECLARE(axis2_status_t)
threadattr_cleanup(void *data)
{
    axis2_threadattr_t *attr = data;
    /*nothing to clean up*/
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t) axis2_threadattr_detach_set(
						axis2_threadattr_t *attr,
						axis2_bool_t detached)
{
	attr->detach = detached;
	return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_threadattr_detach_get(axis2_threadattr_t *attr, axis2_env_t **env)
{
	if (1 == attr->detach)
	{
        return AXIS2_TRUE;
	}
    return AXIS2_FALSE;
}

AXIS2_DECLARE(axis2_status_t) 
axis2_threadattr_stacksize_set(axis2_threadattr_t *attr, size_t stacksize)
{
    attr->stacksize = stacksize;
    return AXIS2_SUCCESS;
}

static void *dummy_worker(void *opaque)
{
    axis2_thread_t *thd = (axis2_thread_t *)opaque;
    TlsSetValue(tls_axis2_thread, thd->td);
    return thd->func(thd, thd->data);
}

AXIS2_DECLARE(axis2_thread_t*)
axis2_thread_create(axis2_allocator_t *allocator, axis2_threadattr_t *attr,
						axis2_thread_start_t func, void *data)
{
    HANDLE           handle;
	unsigned         temp;
    axis2_thread_t   *new = NULL;

    new = (axis2_thread_t *)AXIS2_MALLOC(allocator, 
						sizeof(axis2_thread_t));

    if (NULL == new) 
	{
        return NULL;
    }
    
	new->data = data;
    new->func = func;
	new->td   = NULL;

	/* Use 0 for Thread Stack Size, because that will default the stack to the
     * same size as the calling thread. 
     */
    if ((handle = CreateThread(NULL,
                        attr && attr->stacksize > 0 ? attr->stacksize : 0,
                        (unsigned int (AXIS2_THREAD_FUNC *)(void *))dummy_worker,
                        new, 0, &temp)) == 0) 
	{
        return NULL;
    }
	
    if (attr && attr->detach) {
        CloseHandle(handle);
    }
    else
        new->td = handle;

	return new;
}

AXIS2_DECLARE(axis2_status_t)
axis2_thread_exit(axis2_thread_t *thd)
{
    ExitThread(0);

    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_os_thread_t)
axis2_os_thread_current(void)
{
    HANDLE hthread = (HANDLE)TlsGetValue(tls_axis2_thread);
    HANDLE hproc;

    if (hthread) 
	{
        return hthread;
    }
    
    hproc = GetCurrentProcess();
    hthread = GetCurrentThread();
    if (!DuplicateHandle(hproc, hthread, 
                         hproc, &hthread, 0, FALSE, 
                         DUPLICATE_SAME_ACCESS)) {
        return NULL;
    }
    TlsSetValue(tls_axis2_thread, hthread);
    return hthread;
}

AXIS2_DECLARE(int)
axis2_os_thread_equal(axis2_os_thread_t tid1, axis2_os_thread_t tid2)
{
    return (tid1 == tid2);
}

AXIS2_DECLARE(axis2_status_t)
axis2_thread_join(axis2_thread_t *thd)
{
    axis2_status_t rv = AXIS2_SUCCESS,rv1;
    
    if (!thd->td)
	{
        /* Can not join on detached threads */
        return AXIS2_FAILURE;
    }
    rv1 = WaitForSingleObject(thd->td, INFINITE);
    if ( rv1 == WAIT_OBJECT_0 || rv1 == WAIT_ABANDONED) 
	{
        /*rv = AXIS2_INCOMPLETE;*/
    }
    else
        rv = AXIS2_FAILURE;
    CloseHandle(thd->td);
    thd->td = NULL;

    return rv;
}

AXIS2_DECLARE(axis2_status_t)
axis2_thread_detach(axis2_thread_t *thd)
{
    if (thd->td && CloseHandle(thd->td)) 
	{
        thd->td = NULL;
        return AXIS2_SUCCESS;
    }
    else 
	{
        return AXIS2_FAILURE;
    }
}

AXIS2_DECLARE(axis2_os_thread_t)
axis2_os_thread_get(axis2_thread_t *thd, axis2_env_t **env)
{
	return thd->td;
}

AXIS2_DECLARE(axis2_status_t)
axis2_thread_once_init(axis2_thread_once_t **control, axis2_allocator_t *allocator)
{
    *control = AXIS2_MALLOC(allocator, sizeof(**control));
	if(NULL == *control)
	{
		return AXIS2_FAILURE;
	}
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_thread_once(axis2_thread_once_t *control, void (*func)(void))
{
    if (!InterlockedExchange(&control->value, 1)) {
        func();
    }
    return AXIS2_SUCCESS;
}
