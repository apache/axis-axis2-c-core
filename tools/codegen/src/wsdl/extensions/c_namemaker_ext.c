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
 
#include <w2c_extension.h>
#include <w2c_string.h>
#include <axis2_string.h>


typedef struct w2c_c_namemaker_ext_impl
{
    w2c_extension_t extension;

} w2c_c_namemaker_ext_impl_t;

#define W2C_INTF_TO_IMPL(extension) \
        ((w2c_c_namemaker_ext_impl_t*) extension)

/************************* implmentations ********************************/
axis2_status_t AXIS2_CALL
w2c_c_namemaker_ext_free(w2c_extension_t *extension,
       const axis2_env_t *env)
{
    w2c_c_namemaker_ext_impl_t *extension_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    extension_impl = W2C_INTF_TO_IMPL(extension);

    if(extension->ops)
    {
        AXIS2_FREE(env->allocator, extension->ops);
    }
    if(extension_impl)
    {
        AXIS2_FREE(env->allocator, extension_impl);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_c_namemaker_ext_engage(w2c_extension_t *extension,
       const axis2_env_t *env,
       w2c_engine_configuration_t *conf)
{
    w2c_c_namemaker_ext_impl_t *extension_impl = NULL;
    axis2_bool_t convert_names = AXIS2_FALSE;
    axis2_char_t *lang = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    extension_impl = W2C_INTF_TO_IMPL(extension);

    convert_names = W2C_ENGINE_CONFIGURATION_GET_CONVERT_NAMES( conf, env);
    lang = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LANGUAGE( conf, env);

    /* this executes only with following configuration */
    if ( convert_names && AXIS2_STRCMP( lang, "c") == 0)
    {
        W2C_ENGINE_CONFIGURATION_SET_NAMEMAKER_FUNC( conf, env, 
               (W2C_ENGINE_CONFIGURATION_NAMEMAKER) w2c_string_make_c_simple_name);
    }
        
    return AXIS2_SUCCESS;
}

/****************** standard create and delete for DLL ************************/
AXIS2_EXPORT int
axis2_get_instance(w2c_extension_t **inst,
                   const axis2_env_t *env)
{
    w2c_c_namemaker_ext_impl_t *extension_impl = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    extension_impl = (w2c_c_namemaker_ext_impl_t*)AXIS2_MALLOC( env-> allocator, 
                                          sizeof(w2c_c_namemaker_ext_impl_t) );
 
    if(NULL == extension_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return AXIS2_FAILURE;
    }
    
    extension_impl->extension.ops = 
                AXIS2_MALLOC (env->allocator, sizeof(w2c_extension_ops_t));
    if(NULL == extension_impl->extension.ops)
    {
        w2c_c_namemaker_ext_free(&(extension_impl->extension), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }   
    extension_impl->extension.ops->free = 
                      w2c_c_namemaker_ext_free;
    extension_impl->extension.ops->engage = 
                      w2c_c_namemaker_ext_engage;

    *inst = &(extension_impl->extension);

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(w2c_extension_t *inst,
                      const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    if (inst)
    {
        status = W2C_EXTENSION_FREE(inst, env);
    }
    return status;
}

