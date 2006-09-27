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
 
#include <w2c_cqname2name_maker.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <stdlib.h>


#define W2C_TYPEMAPPER_UPPER_PARAM_LIMIT 1000
#define W2C_TYPEMAPPER_PARAM_SUFFIX "param"

/** 
 * @brief
 */
typedef struct w2c_cqname2name_maker_impl
{
    w2c_cqname2name_maker_t cqname2name_maker;

    axis2_hash_t *qname2name;
    axis2_hash_t *name2number;
       
} w2c_cqname2name_maker_impl_t;

#define W2C_INTF_TO_IMPL(cqname2name_maker) \
    ((w2c_cqname2name_maker_impl_t *) cqname2name_maker)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_cqname2name_maker_free (w2c_cqname2name_maker_t *cqname2name_maker, 
           const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_cqname2name_maker_suggest_name(w2c_cqname2name_maker_t *cqname2name_maker,
            const axis2_env_t *env,
            axis2_qname_t *qname);

/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_cqname2name_maker_t * AXIS2_CALL 
w2c_cqname2name_maker_create (const axis2_env_t *env)
{
    w2c_cqname2name_maker_impl_t *cqname2name_maker_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    cqname2name_maker_impl = (w2c_cqname2name_maker_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_cqname2name_maker_impl_t));

    if(NULL == cqname2name_maker_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    cqname2name_maker_impl->qname2name = axis2_hash_make(env);
    cqname2name_maker_impl->name2number = axis2_hash_make(env);
    
    cqname2name_maker_impl->cqname2name_maker.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_cqname2name_maker_ops_t));
    if(NULL == cqname2name_maker_impl->cqname2name_maker.ops)
    {
        w2c_cqname2name_maker_free(&(cqname2name_maker_impl->cqname2name_maker), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    cqname2name_maker_impl->cqname2name_maker.ops->free = w2c_cqname2name_maker_free;
    cqname2name_maker_impl->cqname2name_maker.ops->suggest_name= 
                                        w2c_cqname2name_maker_suggest_name;

    return &(cqname2name_maker_impl->cqname2name_maker);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_cqname2name_maker_free (w2c_cqname2name_maker_t *cqname2name_maker, 
                            const axis2_env_t *env)
{
    w2c_cqname2name_maker_impl_t *cqname2name_maker_impl = NULL;
 
    axis2_char_t *key = NULL;
    axis2_char_t *value =NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_hash_t *h = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    cqname2name_maker_impl = W2C_INTF_TO_IMPL(cqname2name_maker);
    h = cqname2name_maker_impl-> qname2name;
    
    for (hi = axis2_hash_first(h, env) ;
                hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, (void*)&key, NULL, (void*)&value);
        if (key)
            AXIS2_FREE( env-> allocator, key);
        if (value)
            AXIS2_FREE( env-> allocator, value); 
    }
    axis2_hash_free( h , env);
 
    axis2_hash_free( cqname2name_maker_impl-> name2number, env);
       
    if(cqname2name_maker->ops)
    {
        AXIS2_FREE(env->allocator, cqname2name_maker->ops);
        cqname2name_maker->ops = NULL;
    }
    if(cqname2name_maker_impl)
    {
        AXIS2_FREE(env->allocator, cqname2name_maker_impl);
        cqname2name_maker_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
w2c_cqname2name_maker_suggest_name(w2c_cqname2name_maker_t *cqname2name_maker,
            const axis2_env_t *env,
            axis2_qname_t *qname)
{
    w2c_cqname2name_maker_impl_t *cqname2name_maker_impl = NULL;
 
    axis2_char_t *key = NULL;
    axis2_char_t *local = NULL;
    int counter = 0;
    axis2_char_t counter_str[32];
    axis2_char_t *given_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    cqname2name_maker_impl = W2C_INTF_TO_IMPL(cqname2name_maker);

    key = AXIS2_QNAME_TO_STRING(qname, env);
    local = (axis2_char_t*)axis2_hash_get( cqname2name_maker_impl-> qname2name, key,
                         AXIS2_HASH_KEY_STRING );
    if( local != NULL) /* key has been there somewhere */
    {
        return local;
    }
    /* otherwise */
    local = AXIS2_QNAME_GET_LOCALPART( qname, env);
    counter = (int)axis2_hash_get( cqname2name_maker_impl-> name2number, local,
                         AXIS2_HASH_KEY_STRING );
    if ( counter == 0 ) /** this means name doesnt exist */
    {
        axis2_hash_set(cqname2name_maker_impl-> qname2name, key, AXIS2_HASH_KEY_STRING, local);
        axis2_hash_set( cqname2name_maker_impl-> name2number, local, AXIS2_HASH_KEY_STRING, (void*)1);
        return local;
    }
    /* else qname would already exist */ 
    sprintf(counter_str, "%d", counter);
    given_name = axis2_stracat( local, counter_str, env);

    axis2_hash_set( cqname2name_maker_impl-> qname2name, key, AXIS2_HASH_KEY_STRING, given_name);
    axis2_hash_set( cqname2name_maker_impl-> name2number, local, AXIS2_HASH_KEY_STRING, (void*)counter+1);

    return given_name;    
}

