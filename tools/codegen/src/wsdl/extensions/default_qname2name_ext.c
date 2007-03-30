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


typedef struct w2c_default_qname2name_ext_impl
{
    w2c_extension_t extension;
    w2c_qname2name_maker_t qname2name_maker;

} w2c_default_qname2name_ext_impl_t;

#define W2C_EXTENSION_INTF_TO_IMPL(extension) \
        ((w2c_default_qname2name_ext_impl_t*) extension)

#define W2C_QNAME2NAME_MAKER_INTF_TO_IMPL(qname2name_maker) \
        ((w2c_default_qname2name_ext_impl_t*) ((char*)(qname2name_maker) - sizeof(w2c_extension_t)))

/**************implmentations for w2c_extension_t methods**********************/
axis2_status_t AXIS2_CALL
w2c_default_qname2name_ext_free(w2c_extension_t *extension,
       const axutil_env_t *env)
{
    w2c_default_qname2name_ext_impl_t *impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    impl = W2C_EXTENSION_INTF_TO_IMPL(extension);

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
w2c_default_qname2name_ext_engage(w2c_extension_t *extension,
       const axutil_env_t *env,
       w2c_engine_configuration_t *conf)
{
    w2c_default_qname2name_ext_impl_t *impl = NULL;
    w2c_qname2name_maker_t *qname2name_maker = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    impl = W2C_EXTENSION_INTF_TO_IMPL(extension);

    qname2name_maker = &(impl-> qname2name_maker);

    /* this will executes for all configuration */
    W2C_ENGINE_CONFIGURATION_SET_QNAME2NAME( conf, env, 
                   qname2name_maker);
        
    return AXIS2_SUCCESS;
}

/***********implmentations for w2c_default_qname2name_maker methods********************/

axis2_char_t* AXIS2_CALL
w2c_default_qname2name_ext_suggest_name(w2c_qname2name_maker_t *qname2name_maker,
            const axutil_env_t *env,
            axis2_qname_t *qname)
{
    w2c_default_qname2name_ext_impl_t *impl = NULL;
 
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    impl = W2C_QNAME2NAME_MAKER_INTF_TO_IMPL(qname2name_maker);

    /** here nothing special would be happened */
    return axis2_qname_get_localpart( qname, env);
}

axis2_status_t AXIS2_CALL
w2c_default_qname2name_ext_maker_free(w2c_qname2name_maker_t *qname2name_maker,
       const axutil_env_t *env)
{
    w2c_default_qname2name_ext_impl_t *impl = NULL;
    impl = W2C_QNAME2NAME_MAKER_INTF_TO_IMPL(qname2name_maker);

    return w2c_default_qname2name_ext_free( 
            (w2c_extension_t*)qname2name_maker, env);
}

/****************** standard create and delete for DLL ************************/
AXIS2_EXPORT int
axis2_get_instance(w2c_extension_t **inst,
                   const axutil_env_t *env)
{
    w2c_default_qname2name_ext_impl_t *impl = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    impl = (w2c_default_qname2name_ext_impl_t*)AXIS2_MALLOC( env-> allocator, 
                                          sizeof(w2c_default_qname2name_ext_impl_t) );
 
    if(! impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return AXIS2_FAILURE;
    }
    impl->extension.ops = 
                AXIS2_MALLOC (env->allocator, sizeof(w2c_extension_ops_t));
    if(! impl->extension.ops)
    {
        w2c_default_qname2name_ext_free(&(impl->extension), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    impl->qname2name_maker.ops = 
                AXIS2_MALLOC (env->allocator, sizeof(w2c_qname2name_maker_ops_t));
    if(! impl->extension.ops)
    {
        w2c_default_qname2name_ext_free(&(impl->extension), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }   

    impl->extension.ops->free = 
                      w2c_default_qname2name_ext_free;
    impl->extension.ops->engage = 
                      w2c_default_qname2name_ext_engage;
    impl->qname2name_maker.ops->suggest_name =
                      w2c_default_qname2name_ext_suggest_name;
    impl->qname2name_maker.ops->free =
                      w2c_default_qname2name_ext_maker_free;

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

