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
#include <axiom_mtom_caching_callback.h>
#include <axutil_url.h>


AXIS2_EXTERN axis2_status_t AXIS2_CALL
caching_callback_free(axiom_mtom_caching_callback_t *caching_callback,
								const axutil_env_t* env)
{
	if (caching_callback)
	{
		if (caching_callback->ops)
		{
			AXIS2_FREE(env->allocator, caching_callback->ops);
		}
		AXIS2_FREE(env->allocator, caching_callback);
	}
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN void* AXIS2_CALL
caching_callback_init_handler(axiom_mtom_caching_callback_t *caching_callback, 
                              const axutil_env_t* env,
                              axis2_char_t *key)
{
    FILE *fp = NULL;
    axis2_char_t *file_name = NULL;
    axis2_char_t *encoded_key = NULL;

    if(caching_callback->user_param)
    {
        /* If this is set the callback implementer may use 
           it to create the void return value at the end of
           this function.  
         */
    }   
 
    if(key)
    {
        /*encoded_key = axutil_strdup(env, key);*/
        encoded_key = AXIS2_MALLOC(env->allocator, (sizeof(axis2_char_t ))* (strlen(key)));
        memset(encoded_key, 0, strlen(key));

        if(encoded_key)
        {
            encoded_key = axutil_url_encode(env, encoded_key, key, strlen(key));
        }

        file_name = axutil_stracat(env, "/opt/tmp/", encoded_key);
        /*file_name = key;*/
        if(file_name)
        {
            fp = fopen(file_name, "ab+");
        }
        else
        {
            return NULL;
        }
    }

    #if !defined(WIN32)
    {
        axis2_char_t permission_str[256];
        sprintf(permission_str, "chmod 777 %s", file_name);
        system(permission_str);
    }
    #endif
    
    if(encoded_key)
    {
        AXIS2_FREE(env->allocator, encoded_key);
        encoded_key = NULL;
    }


    if(file_name)
    {
        AXIS2_FREE(env->allocator, file_name);
        file_name = NULL;
    }

    return (void *)fp;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
caching_callback_cache(axiom_mtom_caching_callback_t *caching_callback, 
            const axutil_env_t* env,
            axis2_char_t *buf,
            int buf_len,
            void* handler)
{
    int len = 0;
    FILE *fp = (FILE *)(handler);


    len = fwrite(buf, 1, buf_len, fp);
    if(len < 0)
    {
        return AXIS2_FAILURE;
    }
   
    return AXIS2_SUCCESS; 

}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
caching_callback_close_handler(axiom_mtom_caching_callback_t *caching_callback, 
            const axutil_env_t* env,
            void* handler)
{
    axis2_status_t status = AXIS2_SUCCESS;
                                                                                                                
    if(fclose((FILE *)handler) != 0)
    {
        status = AXIS2_FAILURE;
    }
    return status;
}


/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int
axis2_get_instance(axiom_mtom_caching_callback_t **inst,
        const axutil_env_t *env)
{
    axiom_mtom_caching_callback_t* caching_callback = NULL;

    caching_callback = AXIS2_MALLOC(env->allocator,
            sizeof(axiom_mtom_caching_callback_t));

    caching_callback->ops = AXIS2_MALLOC(
                env->allocator, sizeof(axiom_mtom_caching_callback_ops_t));

    /*assign function pointers*/

    caching_callback->ops->init_handler = caching_callback_init_handler;
    caching_callback->ops->cache = caching_callback_cache;
    caching_callback->ops->close_handler = caching_callback_close_handler;
    caching_callback->ops->free = caching_callback_free;

    *inst = caching_callback;

    if (!(*inst))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Cannot load the caching callback");
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int axis2_remove_instance(axiom_mtom_caching_callback_t *inst,
        const axutil_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIOM_MTOM_CACHING_CALLBACK_FREE(inst, env);
    }
    return status;
}
