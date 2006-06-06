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
 
#include <axis2_stub.h>

/** 
 * @brief
 */
typedef struct axis2_stub_impl
{
    axis2_stub_t stub;
    
    axis2_svc_client_t *svc_client;
    axis2_options_t *options;
       
} axis2_stub_impl_t;

#define AXIS2_INTF_TO_IMPL(stub) \
    ((axis2_stub_impl_t *) stub)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_stub_free (axis2_stub_t *stub, 
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_ref (axis2_stub_t *stub,
                                const axis2_env_t *env,
                                axis2_endpoint_ref_t *endpoint_ref);

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_uri (axis2_stub_t *stub,
                                const axis2_env_t *env,
                                axis2_char_t *endpoint_uri);
                        
/**
 * @param use_separate_transport
 */
axis2_status_t AXIS2_CALL 
axis2_stub_set_use_seperate_listener(axis2_stub_t *stub,
                                const axis2_env_t *env,
                                axis2_bool_t use_separate_listener);


axis2_status_t AXIS2_CALL
axis2_stub_engage_module(axis2_stub_t *stub,
                            const axis2_env_t *env,
                            axis2_char_t *module_name);

/**
 * Set the soap version
 * @param soap_version
 */
axis2_status_t AXIS2_CALL
axis2_stub_set_soap_version(axis2_stub_t *stub,
                            const axis2_env_t *env,
                            int soap_version);


axis2_char_t *AXIS2_CALL
axis2_stub_get_svc_ctx_id(axis2_stub_t *stub,
                            const axis2_env_t *env);

axis2_svc_client_t *AXIS2_CALL
axis2_stub_get_svc_client(axis2_stub_t *stub,
                        const axis2_env_t *env);

axis2_options_t *AXIS2_CALL
axis2_stub_get_options(axis2_stub_t *stub,
                        const axis2_env_t *env);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_stub_t * AXIS2_CALL 
axis2_stub_create (const axis2_env_t *env)
{
    axis2_stub_impl_t *stub_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    stub_impl = (axis2_stub_impl_t *) AXIS2_MALLOC(env->
        allocator, sizeof(axis2_stub_impl_t));

    if(NULL == stub_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    stub_impl->svc_client = NULL;
    stub_impl->options = NULL;

    stub_impl->stub.ops = NULL;
    
    stub_impl->stub.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_stub_ops_t));
    if(NULL == stub_impl->stub.ops)
    {
        axis2_stub_free(&(stub_impl->stub), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    stub_impl->stub.ops->free = axis2_stub_free;
    stub_impl->stub.ops->set_endpoint_ref = axis2_stub_set_endpoint_ref;
    stub_impl->stub.ops->set_endpoint_uri = axis2_stub_set_endpoint_uri;
    stub_impl->stub.ops->set_use_seperate_listener = axis2_stub_set_use_seperate_listener;
    stub_impl->stub.ops->engage_module = axis2_stub_engage_module;
    stub_impl->stub.ops->set_soap_version = axis2_stub_set_soap_version;
    stub_impl->stub.ops->get_svc_ctx_id = axis2_stub_get_svc_ctx_id;
    stub_impl->stub.ops->get_svc_client= axis2_stub_get_svc_client;
    stub_impl->stub.ops->get_options= axis2_stub_get_options;

    return &(stub_impl->stub);
}

AXIS2_EXTERN axis2_stub_t * AXIS2_CALL 
axis2_stub_create_with_endpoint_ref_and_client_home (const axis2_env_t *env,
                                        axis2_endpoint_ref_t *endpoint_ref,
                                        axis2_char_t *client_home)
{
    axis2_stub_impl_t *stub_impl = NULL;
  
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, endpoint_ref, NULL);
    
    stub_impl = (axis2_stub_impl_t *) axis2_stub_create(env);
    if(!stub_impl)
    {
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    /* create service_client*/
    stub_impl->svc_client = axis2_svc_client_create(env , client_home );
    
    if(!stub_impl->svc_client )
    {
        axis2_stub_free(&(stub_impl->stub), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    /* create options */
    stub_impl->options = axis2_options_create(env );
    if (!stub_impl->options )
    {
        axis2_stub_free(&(stub_impl->stub), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
   return NULL;

    }
    /* Set service client options */
    AXIS2_SVC_CLIENT_SET_OPTIONS( stub_impl->svc_client,
                        env,
              stub_impl-> options);
    
    AXIS2_OPTIONS_SET_TO( stub_impl->options, env, endpoint_ref);
    
    return &(stub_impl->stub);
}

AXIS2_EXTERN axis2_stub_t * AXIS2_CALL 
axis2_stub_create_with_endpoint_uri_and_client_home (const axis2_env_t *env,
                                        const axis2_char_t *endpoint_uri,
                                        const axis2_char_t *client_home)
{
    axis2_stub_impl_t *stub_impl = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, endpoint_uri, NULL);
 
    endpoint_ref = axis2_endpoint_ref_create(env, endpoint_uri);
    if(!endpoint_ref)
    {
        return NULL;
    }
    stub_impl = (axis2_stub_impl_t *) 
        axis2_stub_create_with_endpoint_ref_and_client_home(env, endpoint_ref,
                                                            client_home);
    
    if(!stub_impl)
    {
        axis2_stub_free(&(stub_impl->stub), env);
   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    
    return &(stub_impl->stub);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_stub_free (axis2_stub_t *stub, 
                            const axis2_env_t *env)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    if(stub->ops)
    {
        AXIS2_FREE(env->allocator, stub->ops);
        stub->ops = NULL;
    }

    if(stub_impl->svc_client )
    {
        AXIS2_SVC_CLIENT_FREE (stub_impl->svc_client, env);
        stub_impl->svc_client = NULL;
    }
    
    if(stub_impl)
    {
        AXIS2_FREE(env->allocator, stub_impl);
        stub_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_ref (axis2_stub_t *stub,
                                const axis2_env_t *env,
                                axis2_endpoint_ref_t *endpoint_ref)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, endpoint_ref, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);

    AXIS2_OPTIONS_SET_TO ( stub_impl->options , env, endpoint_ref);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_uri (axis2_stub_t *stub,
                                const axis2_env_t *env,
                                axis2_char_t *endpoint_uri)
{
    axis2_stub_impl_t *stub_impl = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, endpoint_uri, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    endpoint_ref = axis2_endpoint_ref_create(env, endpoint_uri);
    if(!endpoint_ref)
    {
        return AXIS2_FAILURE;
    }
    AXIS2_OPTIONS_SET_TO ( stub_impl->options , env, endpoint_ref);
    
    return AXIS2_SUCCESS;
}

/**
 * @param use_separate_transport
 */
axis2_status_t AXIS2_CALL
axis2_stub_set_use_seperate_listener(axis2_stub_t *stub,
                                const axis2_env_t *env,
                                axis2_bool_t use_separate_listener)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    AXIS2_OPTIONS_SET_USE_SEPERATE_LISTENER ( stub_impl-> options, env,
                                    use_separate_listener );
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_stub_engage_module(axis2_stub_t *stub,
                            const axis2_env_t *env,
                            axis2_char_t *module_name)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_name, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
   
    return AXIS2_SVC_CLIENT_ENGAGE_MODULE(stub_impl->svc_client, env, module_name); 
}

/**
 * Set the soap version
 * @param soap_version
 */
axis2_status_t AXIS2_CALL
axis2_stub_set_soap_version(axis2_stub_t *stub,
                            const axis2_env_t *env,
                            int soap_version)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
   
    if ( !stub_impl-> options ) 
    {
      return AXIS2_FAILURE;
    }
    return AXIS2_OPTIONS_SET_SOAP_VERSION( stub_impl-> options,
                                 env, soap_version );
}

axis2_char_t *AXIS2_CALL
axis2_stub_get_svc_ctx_id(axis2_stub_t *stub,
                            const axis2_env_t *env)
{
    axis2_stub_impl_t *stub_impl = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_char_t *svc_ctx_id = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
   
    svc_ctx = AXIS2_SVC_CLIENT_GET_SERVICE_CONTEXT(stub_impl->svc_client,
                                            env );
    svc_ctx_id = AXIS2_SVC_CTX_GET_SVC_ID ( svc_ctx, env );
    return svc_ctx_id;
}

axis2_svc_client_t *AXIS2_CALL
axis2_stub_get_svc_client(axis2_stub_t *stub,
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(stub)->svc_client;
}

axis2_options_t *AXIS2_CALL
axis2_stub_get_options(axis2_stub_t *stub,
                 const axis2_env_t *env)
{
      AXIS2_ENV_CHECK(env, NULL);
      return AXIS2_INTF_TO_IMPL(stub)->options;
}
