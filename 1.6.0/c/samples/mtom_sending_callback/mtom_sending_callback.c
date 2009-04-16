/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <axutil_string.h>
#include <axutil_utils.h>
#include <axiom_mtom_sending_callback.h>
#include <sys/stat.h>

#define MAX_BUFFER_SIZE 1024

struct storage_handler
{
    FILE *fp;   
    int storage_size; 
    axis2_char_t *buffer;
    int buffer_size;
};
typedef struct storage_handler storage_handler_t;


AXIS2_EXTERN axis2_status_t AXIS2_CALL
mtom_sending_callback_free(axiom_mtom_sending_callback_t *mtom_sending_callback,
								const axutil_env_t* env)
{
	if (mtom_sending_callback)
	{
		if (mtom_sending_callback->ops)
		{
			AXIS2_FREE(env->allocator, mtom_sending_callback->ops);
		}
		AXIS2_FREE(env->allocator, mtom_sending_callback);
	}
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN void* AXIS2_CALL
mtom_sending_callback_init_handler(axiom_mtom_sending_callback_t *mtom_sending_callback, 
                              const axutil_env_t* env,
                              void *user_param)
{
    axis2_char_t *file_name = NULL;
    storage_handler_t *storage_handler = NULL;
    struct stat stat_p;

    if(user_param)
    {
        /*file_name = axutil_stracat(env, "/tmp/", (axis2_char_t *)user_param);*/
        file_name = (axis2_char_t *)user_param;                
    }
    else
    {
        return NULL;
    }

    if (stat(file_name, &stat_p) == -1)
    {
        return NULL;
    }

    storage_handler = AXIS2_MALLOC(env->allocator,
        sizeof(storage_handler_t));

    storage_handler->fp = fopen(file_name, "rb");
    storage_handler->storage_size = stat_p.st_size;

    if(storage_handler->storage_size > MAX_BUFFER_SIZE)
    {
        storage_handler->buffer_size = MAX_BUFFER_SIZE;    
    }
    else
    {
        storage_handler->buffer_size = storage_handler->storage_size;
    }

    storage_handler->buffer = (axis2_char_t *)AXIS2_MALLOC(env->allocator,
        sizeof(axis2_char_t)*(storage_handler->buffer_size));

    /*
    #if !defined(WIN32)
    {
        axis2_char_t permission_str[256];
        sprintf(permission_str, "chmod 777 %s", file_name);
        system(permission_str);
    }
    #endif
    */

    /*if(file_name)
    {
        AXIS2_FREE(env->allocator, file_name);
        file_name = NULL;
    }*/

    return (void *)storage_handler;
}

AXIS2_EXTERN int AXIS2_CALL
mtom_sending_callback_load_data(axiom_mtom_sending_callback_t *mtom_sending_callback, 
            const axutil_env_t* env,
            void *handler,
            axis2_char_t **buffer)
{
    int len = 0;
    FILE *fp = ((storage_handler_t *)(handler))->fp;
    storage_handler_t *storage_handler = NULL;

    storage_handler = (storage_handler_t *)handler;

    memset(storage_handler->buffer, 0, storage_handler->buffer_size);    

    if(feof(fp))
    {
        return 0;
    }
    else
    {
        len = (int)fread(storage_handler->buffer, 1, 
            storage_handler->buffer_size, fp);
    }

    *buffer = storage_handler->buffer;
    return len;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
mtom_sending_callback_close_handler(axiom_mtom_sending_callback_t *mtom_sending_callback, 
            const axutil_env_t* env,
            void* handler)
{
    axis2_status_t status = AXIS2_SUCCESS;
    storage_handler_t *storage_handler = NULL;

    storage_handler = (storage_handler_t *)handler;
                                                                                                                
    if(fclose(storage_handler->fp) != 0)
    {
        status = AXIS2_FAILURE;
    }
    
    AXIS2_FREE(env->allocator, storage_handler->buffer);
    storage_handler->buffer = NULL;

    AXIS2_FREE(env->allocator, storage_handler);
    storage_handler = NULL;

    return status;
}


/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int
axis2_get_instance(axiom_mtom_sending_callback_t **inst,
        const axutil_env_t *env)
{
    axiom_mtom_sending_callback_t* mtom_sending_callback = NULL;

    mtom_sending_callback = AXIS2_MALLOC(env->allocator,
            sizeof(axiom_mtom_sending_callback_t));

    mtom_sending_callback->ops = AXIS2_MALLOC(
                env->allocator, sizeof(axiom_mtom_sending_callback_ops_t));

    /*assign function pointers*/

    mtom_sending_callback->ops->init_handler = mtom_sending_callback_init_handler;
    mtom_sending_callback->ops->load_data = mtom_sending_callback_load_data;
    mtom_sending_callback->ops->close_handler = mtom_sending_callback_close_handler;
    mtom_sending_callback->ops->free = mtom_sending_callback_free;

    *inst = mtom_sending_callback;

    if (!(*inst))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Cannot load the mtom_sending callback");
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(axiom_mtom_sending_callback_t *inst,
        const axutil_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIOM_MTOM_SENDING_CALLBACK_FREE(inst, env);
    }
    return status;
}
