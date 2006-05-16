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
  
#ifndef AXIS2_STUB_H
#define AXIS2_STUB_H

#include <axis2_endpoint_ref.h>
#include <axis2_svc_client.h>
#include <axis2_options.h>
#include <axis2_xml_reader.h>
#include <axis2_property.h>


#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 * @file axis2_stub.h
 * @brief Axis2c service skeleton that should be implemented by actual service
 * wrappers
 */

/** @defgroup axis2_stub Axis2 Stub
 * @ingroup axis2_core_clientapi
 * @{
 */    
 
#define AXIS2_SOAP_11 0
#define AXIS2_SOAP_12 1

typedef struct axis2_stub_ops axis2_stub_ops_t;   
typedef struct axis2_stub axis2_stub_t;

AXIS2_DECLARE_DATA struct axis2_stub_ops
{
    axis2_status_t (AXIS2_CALL * 
    free)(axis2_stub_t *stub,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_endpoint_ref) (axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_endpoint_ref_t *endpoint_ref);
    
    axis2_status_t (AXIS2_CALL *
    set_endpoint_uri) (axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_char_t *endpoint_uri);
    
    
    /**
     *
     * @param use_separate_transport
     */
    axis2_status_t (AXIS2_CALL *
    set_use_seperate_listener)(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_bool_t use_separate_listener);
    
    /**
     * Set the soap version
     * @param soap_version
     */
    axis2_status_t (AXIS2_CALL *
    set_soap_version)(axis2_stub_t *stub,
                        axis2_env_t **env,
                        int soap_version);
    
    
    axis2_char_t *(AXIS2_CALL *
    get_svc_ctx_id)(axis2_stub_t *stub,
                    axis2_env_t **env);
    
    
    axis2_status_t (AXIS2_CALL *
    engage_module)(axis2_stub_t *stub,
		  axis2_env_t **env,
		  axis2_char_t *module_name);

    axis2_svc_client_t *(AXIS2_CALL *
    get_svc_client)(axis2_stub_t *stub,
                            axis2_env_t **env);
    
    axis2_options_t *(AXIS2_CALL *
    get_options)(axis2_stub_t *stub,
                            axis2_env_t **env);

} ;

AXIS2_DECLARE_DATA struct axis2_stub 
{
    axis2_stub_ops_t *ops;
};

/**
 * Creates axis2_stub struct
 * @param endpoint reference
 * @return pointer to newly created axis2_stub struct
 */
AXIS2_DECLARE(axis2_stub_t *)
axis2_stub_create_with_endpoint_ref_and_client_home(axis2_env_t **env,
                                        axis2_endpoint_ref_t *endpoint_ref,
                                        axis2_char_t *client_home);

/**
 * Creates axis2_stub struct
 * @param endpoint uri
 * @return pointer to newly created axis2_stub struct
 */
AXIS2_DECLARE(axis2_stub_t *)
axis2_stub_create_with_endpoint_uri_and_client_home(axis2_env_t **env,
                                        axis2_char_t *endpoint_uri,
                                        axis2_char_t *client_home);

/*************************** Function macros **********************************/

#define AXIS2_STUB_FREE(stub, env) \
		((stub)->ops->free (stub, env))

#define AXIS2_STUB_SET_ENDPOINT_REF(stub, env, endpoint_ref) \
		((stub)->ops->set_endpoint_ref (stub, env, endpoint_ref))

#define AXIS2_STUB_SET_ENDPOINT_URI(stub, env, endpoint_uri) \
		((stub)->ops->set_endpoint_uri (stub, env, endpoint_uri))
        
#define AXIS2_STUB_SET_USE_SEPERATE_LISTENER(stub, env, use_separate_listener) \
		((stub)->ops->set_transport_info (stub, env, use_separate_listener))  

#define AXIS2_STUB_ENGAGE_MODULE(stub, env, module_name) \
		((stub)->ops->engage_module (stub, env, module_name))
        
#define AXIS2_STUB_SET_SOAP_VERSION(stub, env, soap_version) \
		((stub)->ops->set_soap_version (stub, env, soap_version))
        
#define AXIS2_STUB_GET_SVC_CTX_ID(stub, env) \
		((stub)->ops->get_svc_ctx_id (stub, env))
        
#define AXIS2_STUB_GET_SVC_CLIENT(stub, env) \
		((stub)->ops->get_svc_client(stub, env))

#define AXIS2_STUB_GET_OPTIONS(stub, env) \
		((stub)->ops->get_options(stub, env))
        
        
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_STUB_H */
