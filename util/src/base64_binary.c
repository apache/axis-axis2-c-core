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
 
#include <axis2_base64_binary.h>
#include <string.h>
#include <axis2_utils.h>

/** 
 * @brief
 */
typedef struct axis2_base64_binary_impl
{
    axis2_base64_binary_t base64_binary;
    
    unsigned char *plain_binary;
    int plain_binary_len;

} axis2_base64_binary_impl_t;

#define AXIS2_INTF_TO_IMPL(base64_binary) \
    ((axis2_base64_binary_impl_t *) base64_binary)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_base64_binary_free (axis2_base64_binary_t *base64_binary, 
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_base64_binary_set_plain_binary(axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env,
                        const unsigned char* plain_binary,
                        int plain_binary_len );

unsigned char* AXIS2_CALL
axis2_base64_binary_get_plain_binary (axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env,
                        int* plain_binary_len );

axis2_status_t AXIS2_CALL
axis2_base64_binary_set_encoded_binary (axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env,
                        const char* plain_binary );

char* AXIS2_CALL
axis2_base64_binary_get_encoded_binary (axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env );

int AXIS2_CALL
axis2_base64_binary_get_encoded_binary_len(axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env );

int AXIS2_CALL
axis2_base64_binary_get_decoded_binary_len(axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env );


/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_base64_binary_t * AXIS2_CALL 
axis2_base64_binary_create (const axis2_env_t *env)
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    base64_binary_impl = (axis2_base64_binary_impl_t *) AXIS2_MALLOC(env->
        allocator, sizeof(axis2_base64_binary_impl_t));

    if(NULL == base64_binary_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    base64_binary_impl->plain_binary = NULL;

    base64_binary_impl->base64_binary.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_base64_binary_ops_t));
    if(NULL == base64_binary_impl->base64_binary.ops)
    {
        axis2_base64_binary_free(&(base64_binary_impl->base64_binary), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    base64_binary_impl->base64_binary.ops->free = axis2_base64_binary_free;
    base64_binary_impl->base64_binary.ops->set_plain_binary = axis2_base64_binary_set_plain_binary;
    base64_binary_impl->base64_binary.ops->get_plain_binary = axis2_base64_binary_get_plain_binary;
    base64_binary_impl->base64_binary.ops->set_encoded_binary = axis2_base64_binary_set_encoded_binary;
    base64_binary_impl->base64_binary.ops->get_encoded_binary = axis2_base64_binary_get_encoded_binary;
    base64_binary_impl->base64_binary.ops->get_encoded_binary_len = axis2_base64_binary_get_encoded_binary_len;
    base64_binary_impl->base64_binary.ops->get_decoded_binary_len = axis2_base64_binary_get_decoded_binary_len;

    return &(base64_binary_impl->base64_binary);
}

AXIS2_EXTERN axis2_base64_binary_t * AXIS2_CALL 
axis2_base64_binary_create_with_plain_binary(const axis2_env_t *env,
                                        const unsigned char* plain_binary,
                                        int plain_binary_len)
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
  
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, plain_binary , NULL);
    
    base64_binary_impl = (axis2_base64_binary_impl_t *) axis2_base64_binary_create(env);
    if(!base64_binary_impl)
    {
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    /* initialize variables */
    base64_binary_impl->plain_binary = AXIS2_MALLOC(env->
        allocator, sizeof(unsigned char) * plain_binary_len );
    
    if(!base64_binary_impl->plain_binary)
    {
        axis2_base64_binary_free(&(base64_binary_impl->base64_binary), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    memcpy ( base64_binary_impl ->plain_binary, plain_binary, plain_binary_len );
    base64_binary_impl->plain_binary_len = plain_binary_len;

    return &(base64_binary_impl->base64_binary);
}

AXIS2_EXTERN axis2_base64_binary_t * AXIS2_CALL 
axis2_base64_binary_create_with_encoded_binary(const axis2_env_t *env,
                                        const char* encoded_binary )
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
    int plain_binary_len = 0;
  
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, encoded_binary , NULL);
    
    base64_binary_impl = (axis2_base64_binary_impl_t *) axis2_base64_binary_create(env);
    if(!base64_binary_impl)
    {
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    /* initialize variables */
    plain_binary_len = axis2_base64_decode_len ( encoded_binary );
    base64_binary_impl->plain_binary =  AXIS2_MALLOC(env->
        allocator, sizeof(unsigned char) * plain_binary_len );

    if(!base64_binary_impl->plain_binary)
    {
        axis2_base64_binary_free(&(base64_binary_impl->base64_binary), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    axis2_base64_decode_binary( base64_binary_impl-> plain_binary,
                                encoded_binary );
    base64_binary_impl-> plain_binary_len = plain_binary_len;

    return &(base64_binary_impl->base64_binary);
}


/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_base64_binary_free (axis2_base64_binary_t *base64_binary, 
                            const axis2_env_t *env)
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    base64_binary_impl = AXIS2_INTF_TO_IMPL(base64_binary);
    
    if(base64_binary->ops)
    {
        AXIS2_FREE(env->allocator, base64_binary->ops);
        base64_binary->ops = NULL;
    }

    if(base64_binary_impl->plain_binary)
    {
        AXIS2_FREE (env->allocator, base64_binary_impl->plain_binary);
        base64_binary_impl->plain_binary = NULL;
        base64_binary_impl->plain_binary_len = 0; 
    }
    
    if(base64_binary_impl)
    {
        AXIS2_FREE(env->allocator, base64_binary_impl);
        base64_binary_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_base64_binary_set_plain_binary(axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env,
                        const unsigned char* plain_binary,
                        int plain_binary_len )
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    base64_binary_impl = AXIS2_INTF_TO_IMPL(base64_binary);
    
    base64_binary_impl->plain_binary = AXIS2_MALLOC(env->
        allocator, sizeof(unsigned char) * plain_binary_len );
    
    if(!base64_binary_impl->plain_binary)
    {
        axis2_base64_binary_free(&(base64_binary_impl->base64_binary), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;  
    }
    memcpy ( base64_binary_impl ->plain_binary, plain_binary, base64_binary_impl->plain_binary_len );
    base64_binary_impl->plain_binary_len = plain_binary_len;

    return AXIS2_SUCCESS;
}

unsigned char* AXIS2_CALL
axis2_base64_binary_get_plain_binary (axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env,
                        int* plain_binary_len )
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    base64_binary_impl = AXIS2_INTF_TO_IMPL(base64_binary);
    *plain_binary_len = base64_binary_impl -> plain_binary_len;
    return base64_binary_impl -> plain_binary;
}
    
axis2_status_t AXIS2_CALL
axis2_base64_binary_set_encoded_binary (axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env,
                        const char* encoded_binary )
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
    int plain_binary_len = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    base64_binary_impl = AXIS2_INTF_TO_IMPL(base64_binary);
 
    plain_binary_len = axis2_base64_decode_len ( encoded_binary );
    base64_binary_impl->plain_binary =  AXIS2_MALLOC(env->
        allocator, sizeof(unsigned char) * plain_binary_len );

    if(!base64_binary_impl->plain_binary)
    {
        axis2_base64_binary_free(&(base64_binary_impl->base64_binary), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;  
    }
    axis2_base64_decode_binary( base64_binary_impl-> plain_binary,
                                encoded_binary );
    base64_binary_impl-> plain_binary_len = plain_binary_len;

   return AXIS2_SUCCESS;
}

char* AXIS2_CALL
axis2_base64_binary_get_encoded_binary (axis2_base64_binary_t *base64_binary,
                        const axis2_env_t *env )
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
    char* encoded_binary = NULL;
    int encoded_binary_len = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    base64_binary_impl = AXIS2_INTF_TO_IMPL(base64_binary);
    encoded_binary_len = axis2_base64_encode_len( 
                             base64_binary_impl-> plain_binary_len );
    encoded_binary = AXIS2_MALLOC ( env-> allocator,
                         sizeof ( char )* encoded_binary_len );
 
   if(!encoded_binary)
   {
        axis2_base64_binary_free(base64_binary, env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
   }
   axis2_base64_encode_binary ( encoded_binary,
                                base64_binary_impl-> plain_binary,
                                base64_binary_impl-> plain_binary_len );
   return encoded_binary;
}


int AXIS2_CALL
axis2_base64_binary_get_encoded_binary_len(axis2_base64_binary_t *base64_binary,
                              const axis2_env_t *env )
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;
    int encoded_binary_len = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    base64_binary_impl = AXIS2_INTF_TO_IMPL(base64_binary);
    encoded_binary_len = axis2_base64_encode_len(
                       base64_binary_impl-> plain_binary_len );    
    return encoded_binary_len;
}

int AXIS2_CALL
axis2_base64_binary_get_decoded_binary_len(axis2_base64_binary_t *base64_binary,
                              const axis2_env_t *env )
{
    axis2_base64_binary_impl_t *base64_binary_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    base64_binary_impl = AXIS2_INTF_TO_IMPL(base64_binary);
    return base64_binary_impl -> plain_binary_len;   
}
