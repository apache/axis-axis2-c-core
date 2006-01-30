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
#include <axis2_call.h>
#include <axis2_xml_reader.h>


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

typedef struct axis2_stub_ops axis2_stub_ops_t;   
typedef struct axis2_stub axis2_stub_t;
 
#define AXIS2_SOAP_11 0
#define AXIS2_SOAP_12 1

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
     * @param sender_transport
     * @param listener_transport
     * @param use_separate_transport
     */
    axis2_status_t (AXIS2_CALL *
    set_transport_info)(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_char_t *sender_transport,
                        axis2_char_t *listener_transport,
                        axis2_bool_t use_separate_listener);
    
    /**
     *
     * @param key
     * @param value
     */
    axis2_status_t (AXIS2_CALL *
    put)(axis2_stub_t *stub,
            axis2_env_t **env,
            axis2_char_t *key,
            void *value);
    
    /**
     *
     * @param key
     * @return the object
     */
    void *(AXIS2_CALL *
    get)(axis2_stub_t *stub,
                    axis2_env_t **env,
                    axis2_char_t *key);
    
    axis2_status_t (AXIS2_CALL *
    engage_module)(axis2_stub_t *stub,
                    axis2_env_t **env,
                    axis2_char_t *module_name);
    
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
    
    
    axis2_om_node_t *(AXIS2_CALL *
    get_element_from_reader)(axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_xml_reader_t *reader);


    axis2_status_t (AXIS2_CALL *
    set_value_doc)(axis2_stub_t *stub,
                    axis2_env_t **env,
                    axis2_soap_envelope_t *envelope,
                    axis2_om_node_t *value,
                    axis2_bool_t is_header);

    /**
     * Extract the correct element - A util method
     * @param env
     * @param type
     * @return the relevant element to be databound
     */
    axis2_om_node_t *(AXIS2_CALL *
    get_element)(axis2_stub_t *stub,
                    axis2_env_t **env,
                    axis2_soap_envelope_t *envelope,
                    axis2_char_t *type);
    
    axis2_status_t (AXIS2_CALL *
    populate_properties)(axis2_stub_t *stub,
                            axis2_env_t **env,
                            axis2_call_t *call);
        
    axis2_status_t (AXIS2_CALL *
    populate_modules)(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_call_t *call); 

    axis2_call_t *(AXIS2_CALL *
    get_call_obj)(axis2_stub_t *stub,
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
axis2_stub_create_with_endpoint_ref(axis2_env_t **env,
                                        axis2_endpoint_ref_t *endpoint_ref);

/**
 * Creates axis2_stub struct
 * @param endpoint uri
 * @return pointer to newly created axis2_stub struct
 */
AXIS2_DECLARE(axis2_stub_t *)
axis2_stub_create_with_endpoint_uri(axis2_env_t **env,
                                        axis2_char_t *endpoint_uri);

/*************************** Function macros **********************************/

#define AXIS2_STUB_FREE(stub, env) \
		((stub)->ops->free (stub, env))

#define AXIS2_STUB_SET_ENDPOINT_REF(stub, env, endpoint_ref) \
		((stub)->ops->set_endpoint_ref (stub, env, endpoint_ref))

#define AXIS2_STUB_SET_ENDPOINT_URI(stub, env, endpoint_uri) \
		((stub)->ops->set_endpoint_uri (stub, env, endpoint_uri))
        
#define AXIS2_STUB_SET_TRANSPORT_INFO(stub, env, sender_transport, listener_transport, \
        use_separate_listener) \
		((stub)->ops->set_transport_info (stub, env, sender_transport, \
        listener_transport, use_separate_listener))  

#define AXIS2_STUB_PUT(stub, env, key, value) \
		((stub)->ops->put (stub, env, key, value))
        
#define AXIS2_STUB_GET(stub, env, key) \
		((stub)->ops->get (stub, env, key))

#define AXIS2_STUB_ENGAGE_MODULE(stub, env, module_name) \
		((stub)->ops->engage_module (stub, env, module_name))
        
#define AXIS2_STUB_SET_SOAP_VERSION(stub, env, soap_version) \
		((stub)->ops->set_soap_version (stub, env, soap_version))

#define AXIS2_STUB_GET_SVC_CTX_ID(stub, env) \
		((stub)->ops->get_svc_ctx_id (stub, env))
        
#define AXIS2_STUB_GET_ELEMENT_FROM_READER(stub, env, reader) \
		((stub)->ops->get_element_from_reader (stub, env, reader))

#define AXIS2_STUB_SET_VALUE_DOC(stub, env, envelope, value, is_header) \
		((stub)->ops->set_value_doc (stub, env, envelope, value, is_header))
        
#define AXIS2_STUB_GET_ELEMENT(stub, env, envelope, type) \
		((stub)->ops->get_element (stub, env, envelope, type))

#define AXIS2_STUB_POPULATE_PROPERTIES(stub, env, call) \
		((stub)->ops->populate_properties (stub, env, call))
        
#define AXIS2_STUB_POPULATE_MODULES(stub, env, call) \
		((stub)->ops->populate_modules (stub, env, call))

#define AXIS2_STUB_GET_CALL_OBJ(stub, env) \
		((stub)->ops->get_call_obj (stub, env))
        
        
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_STUB_H */
