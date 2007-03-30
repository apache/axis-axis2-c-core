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
 
#include <w2c_extension.h>
#include <w2c_string.h>
#include <w2c_qname2name_maker.h>


typedef struct w2c_c_qname2name_ext_impl
{
    w2c_extension_t extension;
    w2c_qname2name_maker_t qname2name_maker;

    axis2_hash_t *qname2name;
    axis2_hash_t *name2number;

} w2c_c_qname2name_ext_impl_t;

#define W2C_EXTENSION_INTF_TO_IMPL(extension) \
        ((w2c_c_qname2name_ext_impl_t*) extension)

#define W2C_QNAME2NAME_MAKER_INTF_TO_IMPL(qname2name_maker) \
        ((w2c_c_qname2name_ext_impl_t*) ((char*)(qname2name_maker) - sizeof(w2c_extension_t)))

/**
Above macro was as 
((w2c_c_qname2name_ext_impl_t*) ((void*)(qname2name_maker) - sizeof(w2c_extension_t))) 

This gives a compilation error on win32, and in current form it currupts 
memory.
*/

/**************implmentations for w2c_extension_t methods**********************/
axis2_status_t AXIS2_CALL
w2c_c_qname2name_ext_free(w2c_extension_t *extension,
       const axutil_env_t *env)
{
    w2c_c_qname2name_ext_impl_t *impl = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *value =NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_hash_t *h = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    impl = W2C_EXTENSION_INTF_TO_IMPL(extension);

    h = impl-> qname2name;
    
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
 
    axis2_hash_free( impl-> name2number, env);

    if(extension->ops)
    {
        AXIS2_FREE(env->allocator, extension->ops);
    }
    if((&impl->qname2name_maker)->ops)
    {
        AXIS2_FREE(env->allocator, 
                (&impl->qname2name_maker)->ops);
    }
    if(impl)
    {
        AXIS2_FREE(env->allocator, impl);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_c_qname2name_ext_engage(w2c_extension_t *extension,
       const axutil_env_t *env,
       w2c_engine_configuration_t *conf)
{
    w2c_c_qname2name_ext_impl_t *impl = NULL;
    axis2_char_t *lang = NULL;
    w2c_qname2name_maker_t *qname2name_maker = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    impl = W2C_EXTENSION_INTF_TO_IMPL(extension);

    qname2name_maker = &(impl-> qname2name_maker);
    lang = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LANGUAGE( conf, env);

    /* this executes only with following configuration */
    if ( axis2_strcmp( lang, "c") == 0)
    {
        W2C_ENGINE_CONFIGURATION_SET_QNAME2NAME( conf, env, 
                   qname2name_maker);
    }
        
    return AXIS2_SUCCESS;
}

/***********implmentations for w2c_qname2name_maker methods********************/

axis2_char_t* AXIS2_CALL
w2c_c_qname2name_ext_suggest_name(w2c_qname2name_maker_t *qname2name_maker,
            const axutil_env_t *env,
            axis2_qname_t *qname)
{
    w2c_c_qname2name_ext_impl_t *impl = NULL;
 
    axis2_char_t *key = NULL;
    axis2_char_t *local = NULL;
    int counter = 0;
    axis2_char_t counter_str[32];
    axis2_char_t *given_name = NULL;

	AXIS2_ENV_CHECK(env, NULL);
    
    impl = W2C_QNAME2NAME_MAKER_INTF_TO_IMPL(qname2name_maker);

    key = axis2_qname_to_string(qname, env);
    local = (axis2_char_t*)axis2_hash_get( impl-> qname2name, key,
                         AXIS2_HASH_KEY_STRING );
    if( local != NULL) /* key has been there somewhere */
    {
        return local;
    }
    /* otherwise */
    local = axis2_qname_get_localpart( qname, env);
    counter = (int)axis2_hash_get( impl-> name2number, local,
                         AXIS2_HASH_KEY_STRING );
    if ( counter == 0 ) /** this means name doesnt exist */
    {
        axis2_hash_set(impl-> qname2name, key, AXIS2_HASH_KEY_STRING, local);
        axis2_hash_set(impl-> name2number, local, AXIS2_HASH_KEY_STRING, (void*)1);
        return local;
    }
    /* else qname would already exist */ 
    sprintf(counter_str, "%d", counter);
    given_name = axis2_stracat(env, local, counter_str);

    axis2_hash_set(impl-> qname2name, key, AXIS2_HASH_KEY_STRING, given_name);
    axis2_hash_set(impl-> name2number, local, AXIS2_HASH_KEY_STRING, (void*)(counter+1));

    return given_name;    
}

axis2_status_t AXIS2_CALL
w2c_c_qname2name_ext_maker_free(w2c_qname2name_maker_t *qname2name_maker,
       const axutil_env_t *env)
{
    w2c_c_qname2name_ext_impl_t *impl = NULL;
    impl = W2C_QNAME2NAME_MAKER_INTF_TO_IMPL(qname2name_maker);

    return w2c_c_qname2name_ext_free( 
            (w2c_extension_t*)qname2name_maker, env);
}

/****************** standard create and delete for DLL ************************/
AXIS2_EXPORT int
axis2_get_instance(w2c_extension_t **inst,
                   const axutil_env_t *env)
{
    w2c_c_qname2name_ext_impl_t *impl = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    impl = (w2c_c_qname2name_ext_impl_t*)AXIS2_MALLOC( env-> allocator, 
                                          sizeof(w2c_c_qname2name_ext_impl_t) );
 
    if(! impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return AXIS2_FAILURE;
    }
    impl->qname2name = axis2_hash_make(env);
    impl->name2number = axis2_hash_make(env);
    impl->extension.ops = 
                AXIS2_MALLOC (env->allocator, sizeof(w2c_extension_ops_t));
    if(! impl->extension.ops)
    {
        w2c_c_qname2name_ext_free(&(impl->extension), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }   
    impl->qname2name_maker.ops = 
                AXIS2_MALLOC (env->allocator, sizeof(w2c_qname2name_maker_ops_t));
    if(! impl->qname2name_maker.ops)
    {
        w2c_c_qname2name_ext_free(&(impl->extension), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }   
    impl->extension.ops->free = 
                      w2c_c_qname2name_ext_free;
    impl->extension.ops->engage = 
                      w2c_c_qname2name_ext_engage;
    impl->qname2name_maker.ops->suggest_name =
                      w2c_c_qname2name_ext_suggest_name;
    impl->qname2name_maker.ops->free =
                      w2c_c_qname2name_ext_maker_free;

    *inst = &(impl->extension);

    return AXIS2_SUCCESS;
}


AXIS2_EXPORT int
axis2_remove_instance(w2c_extension_t *inst,
                      const axutil_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    if (inst)
    {
        status = W2C_EXTENSION_FREE(inst, env);
    }
    return status;
}

