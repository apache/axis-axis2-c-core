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
#include <axis2_http_transport.h>
#include <axis2_wsdl_svc.h>
#include <axis2_om_stax_builder.h>
#include <axis2_soap_header.h>
#include <axis2_soap_body.h>
#include <axis2_uuid_gen.h>

/** 
 * @brief
 */
typedef struct axis2_stub_impl
{
	axis2_stub_t stub;
    
    axis2_call_t *call;
    
    axis2_conf_ctx_t *conf_ctx;
    axis2_svc_t *svc;
    axis2_svc_ctx_t *svc_ctx;
    axis2_endpoint_ref_t *to_epr;
    axis2_bool_t do_rest;
    axis2_char_t *sender_transport;
    axis2_char_t *listener_transport;
    axis2_bool_t use_separate_listener;
    /*Default SOAP version is 11 */
    int soap_version;
    axis2_hash_t *property_map;
    axis2_array_list_t *modules;
    /**
     * If _maintainSession is set to True all the calls will use the same
     * ServiceContext and the user can Share information through that
     * ServiceContext across operations.
     */
    axis2_bool_t maintain_session;
    axis2_char_t *current_session_id;
    	
} axis2_stub_impl_t;

#define AXIS2_INTF_TO_IMPL(stub) \
    ((axis2_stub_impl_t *) stub)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_stub_free (axis2_stub_t *stub, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_ref (axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_endpoint_ref_t *endpoint_ref);

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_uri (axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_char_t *endpoint_uri);
                        
/**
 *
 * @param sender_transport
 * @param listener_transport
 * @param use_separate_transport
 */
axis2_status_t AXIS2_CALL
axis2_stub_set_transport_info(axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_char_t *sender_transport,
                                axis2_char_t *listener_transport,
                                axis2_bool_t use_separate_listener);

/**
 *
 * @param key
 * @param value
 */
axis2_status_t AXIS2_CALL
axis2_stub_put(axis2_stub_t *stub,
                axis2_env_t **env,
                axis2_char_t *key,
                axis2_property_t *value);

/**
 *
 * @param key
 * @return the object
 */
axis2_property_t *AXIS2_CALL
axis2_stub_get(axis2_stub_t *stub,
                axis2_env_t **env,
                axis2_char_t *key);

axis2_status_t AXIS2_CALL
axis2_stub_engage_module(axis2_stub_t *stub,
                            axis2_env_t **env,
                            axis2_char_t *module_name);

/**
 * Set the soap version
 * @param soap_version
 */
axis2_status_t AXIS2_CALL
axis2_stub_set_soap_version(axis2_stub_t *stub,
                            axis2_env_t **env,
                            int soap_version);

axis2_char_t *AXIS2_CALL
axis2_stub_get_svc_ctx_id(axis2_stub_t *stub,
                            axis2_env_t **env);

axis2_char_t *
axis2_stub_get_id(axis2_stub_t *stub,
                        axis2_env_t **env);

axis2_om_node_t *AXIS2_CALL
axis2_stub_get_element_from_reader(axis2_stub_t *stub,
                                    axis2_env_t **env,
                                    axis2_xml_reader_t *reader);


axis2_status_t AXIS2_CALL
axis2_stub_set_value_doc(axis2_stub_t *stub,
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
axis2_om_node_t *AXIS2_CALL
axis2_stub_get_element(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_soap_envelope_t *envelope,
                        axis2_char_t *type);

axis2_status_t AXIS2_CALL
axis2_stub_populate_properties(axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_call_t *call);

axis2_status_t AXIS2_CALL
axis2_stub_populate_modules(axis2_stub_t *stub,
                            axis2_env_t **env,
                            axis2_call_t *call);

axis2_call_t *AXIS2_CALL
axis2_stub_get_call_obj(axis2_stub_t *stub,
                        axis2_env_t **env);

/************************** End of function prototypes ************************/

AXIS2_DECLARE(axis2_stub_t *) 
axis2_stub_create (axis2_env_t **env)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	stub_impl = (axis2_stub_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_stub_impl_t));
	
	if(NULL == stub_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    stub_impl->call = NULL;
    stub_impl->conf_ctx = NULL;
    stub_impl->svc = NULL;
    stub_impl->svc_ctx = NULL;
    stub_impl->to_epr = NULL;
    stub_impl->do_rest = AXIS2_FALSE;
    stub_impl->sender_transport = NULL;
    stub_impl->listener_transport = NULL;
    stub_impl->use_separate_listener = AXIS2_FALSE;
    /* Default SOAP version is 11 */
    stub_impl->soap_version = AXIS2_SOAP_11;
    stub_impl->property_map = NULL;
    stub_impl->modules = NULL;
    stub_impl->maintain_session = AXIS2_FALSE;
    stub_impl->current_session_id = NULL;
    stub_impl->stub.ops = NULL;
    
    stub_impl->sender_transport = AXIS2_STRDUP(AXIS2_TRANSPORT_HTTP, env);
    if(!stub_impl->sender_transport)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
    stub_impl->listener_transport = AXIS2_STRDUP(AXIS2_TRANSPORT_HTTP, env);
    if(!stub_impl->sender_transport)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    stub_impl->property_map = axis2_hash_make(env);
    if(!stub_impl->property_map)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    stub_impl->modules = axis2_array_list_create(env, 0);
    if(!stub_impl->modules)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	stub_impl->stub.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_stub_ops_t));
	if(NULL == stub_impl->stub.ops)
    {
        axis2_stub_free(&(stub_impl->stub), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	stub_impl->stub.ops->free = axis2_stub_free;
    stub_impl->stub.ops->set_endpoint_ref = axis2_stub_set_endpoint_ref;
    stub_impl->stub.ops->set_endpoint_uri = axis2_stub_set_endpoint_uri;
    stub_impl->stub.ops->set_transport_info = axis2_stub_set_transport_info;
    stub_impl->stub.ops->put = axis2_stub_put;
    stub_impl->stub.ops->get = axis2_stub_get;
    stub_impl->stub.ops->engage_module = axis2_stub_engage_module;
    stub_impl->stub.ops->set_soap_version = axis2_stub_set_soap_version;
    stub_impl->stub.ops->get_svc_ctx_id = axis2_stub_get_svc_ctx_id;
    stub_impl->stub.ops->get_element_from_reader = 
            axis2_stub_get_element_from_reader;
    stub_impl->stub.ops->set_value_doc = axis2_stub_set_value_doc;
    stub_impl->stub.ops->get_element = axis2_stub_get_element;
    stub_impl->stub.ops->populate_properties = axis2_stub_populate_properties;
    stub_impl->stub.ops->populate_modules = axis2_stub_populate_modules;
	stub_impl->stub.ops->get_call_obj = axis2_stub_get_call_obj;
	return &(stub_impl->stub);
}

AXIS2_DECLARE(axis2_stub_t *) 
axis2_stub_create_with_endpoint_ref_and_client_home (axis2_env_t **env,
                                        axis2_endpoint_ref_t *endpoint_ref,
                                        axis2_char_t *client_home)
{
    axis2_stub_impl_t *stub_impl = NULL;
   
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_ref, NULL);
    
    stub_impl = (axis2_stub_impl_t *) axis2_stub_create(env);
    if(!stub_impl)
    {
        axis2_stub_free(&(stub_impl->stub), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    /* create call without passing svc_ctx_t struct */
    stub_impl->call = axis2_call_create(env, NULL,
        client_home);
    
    if(!stub_impl->call)
    {
        axis2_stub_free(&(stub_impl->stub), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    AXIS2_CALL_SET_TO(stub_impl->call, env, endpoint_ref);
    
    return &(stub_impl->stub);
}

AXIS2_DECLARE(axis2_stub_t *) 
axis2_stub_create_with_endpoint_uri_and_client_home (axis2_env_t **env,
                                        axis2_char_t *endpoint_uri,
                                        axis2_char_t *client_home)
{
    axis2_stub_impl_t *stub_impl = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
   
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_uri, NULL);
 
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
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    
    return &(stub_impl->stub);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_stub_free (axis2_stub_t *stub, 
                            axis2_env_t **env)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
	if(stub->ops)
    {
        AXIS2_FREE((*env)->allocator, stub->ops);
        stub->ops = NULL;
    }

    if(stub_impl->call)
    {
        AXIS2_CALL_FREE(stub_impl->call, env);
        stub_impl->call = NULL;
    }
    
    if(stub_impl->property_map)
    {
        axis2_hash_free(stub_impl->property_map, env);
        stub_impl->property_map = NULL;
    }

    if (stub_impl->modules)
    {
        AXIS2_ARRAY_LIST_FREE(stub_impl->modules, env);
        stub_impl->modules = NULL;
    }
    
    if(stub_impl)
    {
        AXIS2_FREE((*env)->allocator, stub_impl);
        stub_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_ref (axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_endpoint_ref_t *endpoint_ref)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_ref, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    return AXIS2_CALL_SET_TO(stub_impl->call, env, endpoint_ref);
}

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_uri (axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_char_t *endpoint_uri)
{
    axis2_stub_impl_t *stub_impl = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_uri, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    endpoint_ref = axis2_endpoint_ref_create(env, endpoint_uri);
    if(!endpoint_ref)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_CALL_SET_TO(stub_impl->call, env, endpoint_ref);
}

/**
 *
 * @param sender_transport
 * @param listener_transport
 * @param use_separate_transport
 */
axis2_status_t AXIS2_CALL
axis2_stub_set_transport_info(axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_char_t *sender_transport,
                                axis2_char_t *listener_transport,
                                axis2_bool_t use_separate_listener)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, sender_transport, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, listener_transport, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    if(stub_impl->sender_transport)
    {
        AXIS2_FREE((*env)->allocator, stub_impl->sender_transport);
        stub_impl->sender_transport = NULL;
    }
    stub_impl->sender_transport = sender_transport;
    
    if(stub_impl->listener_transport)
    {
        AXIS2_FREE((*env)->allocator, stub_impl->listener_transport);
        stub_impl->listener_transport = NULL;
    }
    stub_impl->listener_transport = listener_transport;
    stub_impl->use_separate_listener = use_separate_listener;
    return AXIS2_SUCCESS;
}

/**
 *
 * @param key
 * @param value
 */
axis2_status_t AXIS2_CALL
axis2_stub_put(axis2_stub_t *stub,
                axis2_env_t **env,
                axis2_char_t *key,
                axis2_property_t *value)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    axis2_hash_set(stub_impl->property_map, key, AXIS2_HASH_KEY_STRING, value);
    return AXIS2_SUCCESS;
}

/**
 *
 * @param key
 * @return the object
 */
axis2_property_t *AXIS2_CALL
axis2_stub_get(axis2_stub_t *stub,
                axis2_env_t **env,
                axis2_char_t *key)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    return axis2_hash_get(stub_impl->property_map, key, AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_stub_engage_module(axis2_stub_t *stub,
                            axis2_env_t **env,
                            axis2_char_t *module_name)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    return AXIS2_ARRAY_LIST_ADD(stub_impl->modules, env, module_name);
}

/**
 * Set the soap version
 * @param soap_version
 */
axis2_status_t AXIS2_CALL
axis2_stub_set_soap_version(axis2_stub_t *stub,
                            axis2_env_t **env,
                            int soap_version)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    stub_impl->soap_version = soap_version;
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_stub_get_svc_ctx_id(axis2_stub_t *stub,
                            axis2_env_t **env)
{
    axis2_stub_impl_t *stub_impl = NULL;
    axis2_char_t *svc_ctx_id = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    if(stub_impl->maintain_session)
    {
        svc_ctx_id = stub_impl->current_session_id;
    }
    else
    {
        svc_ctx_id = axis2_stub_get_id(stub, env);
    }
    return svc_ctx_id;
}

axis2_char_t *
axis2_stub_get_id(axis2_stub_t *stub,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);

    return axis2_uuid_gen(env);
}

axis2_om_node_t *AXIS2_CALL
axis2_stub_get_element_from_reader(axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_xml_reader_t *reader)
{
    axis2_om_stax_builder_t *builder = NULL;
    axis2_om_document_t *document = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, reader, NULL);
    
    /** create axis2_om_stax_builder by parsing pull_parser struct */
    builder = axis2_om_stax_builder_create (env, reader);
    if(!builder)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CREATING_XML_STREAM_READER,
            AXIS2_FAILURE)
        return NULL;
    }
    document = AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(builder, env);
    return AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(document, env);
    
}
 
axis2_status_t AXIS2_CALL
axis2_stub_set_value_doc(axis2_stub_t *stub,
                            axis2_env_t **env,
                            axis2_soap_envelope_t *envelope,
                            axis2_om_node_t *value,
                            axis2_bool_t is_header)
{
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, envelope, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    /*
    if(is_header)
    {
        axis2_soap_header_t *soap_header = NULL;
        
        soap_header = AXIS2_SOAP_ENVELOPE_GET_HEADER(envelope, env);
        if(!soap_header)
        {
            return AXIS2_FAILURE;
        }
        status = AXIS2_SOAP_HEADER_SET_BASE_NODE(soap_header, env, value);
    }
    else
    {
        axis2_soap_body_t *body = NULL;
        
        body = AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env);
        if(!body)
        {
            return AXIS2_FAILURE;
        }
        status = AXIS2_SOAP_BODY_SET_BASE_NODE(body, env, value);
    }
    */
    return status;
}

/**
 * Extract the correct element - A util method
 * @param env
 * @param type
 * @return the relevant element to be databound
 */
axis2_om_node_t *AXIS2_CALL
axis2_stub_get_element(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_soap_envelope_t *envelope,
                        axis2_char_t *type)
{
    axis2_soap_body_t *body = NULL;
    axis2_om_node_t *node = NULL;
    
    body = AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env);
    node = AXIS2_SOAP_BODY_GET_BASE_NODE(body, env);
    if(0 == AXIS2_STRCMP(AXIS2_STYLE_RPC, type))
    {
        /* todo this needs to be fixed */
        return AXIS2_OM_NODE_GET_FIRST_CHILD(node, env);
    }
    else if(0 == AXIS2_STRCMP(AXIS2_STYLE_DOC, type))
    {
        return node;
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_UNSUPPORTED_TYPE, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
}

axis2_status_t AXIS2_CALL
axis2_stub_populate_properties(axis2_stub_t *stub,
                    axis2_env_t **env,
                    axis2_call_t *call)
{
    axis2_hash_index_t *index = NULL;
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, call, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    for(index = axis2_hash_first(stub_impl->property_map, env); index; index = 
        axis2_hash_next(env, index))
    {
        const void *key = NULL;
        void *val = NULL;
        axis2_property_t *property = NULL;
        
        axis2_hash_this(index, &key, NULL, &val);
        property = (axis2_property_t *) val;
        AXIS2_CALL_SET(call, env, key, property);
        
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_stub_populate_modules(axis2_stub_t *stub,
                    axis2_env_t **env,
                    axis2_call_t *call)
{
    axis2_stub_impl_t *stub_impl = NULL;
    int i = 0;
    int size = 0;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, call, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    size = AXIS2_ARRAY_LIST_SIZE(stub_impl->modules, env);
    for (i = 0; i < size; i++)
    {
        axis2_qname_t *module_qname = NULL;
        axis2_char_t *module_name = NULL;
        
        module_name = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(stub_impl->modules,
            env, i);
        module_qname = axis2_qname_create(env, module_name, NULL, NULL);
        /* TODO uncomment this when method is available
        status = AXIS2_CALL_ENGAGE_MODULE(stub_impl->call, env, modue_qname);
        */
        if(AXIS2_SUCCESS != status)
        {
            return status;
        }
        AXIS2_QNAME_FREE(module_qname, env);
        module_qname = NULL;
    }
    return status;
}

axis2_call_t *AXIS2_CALL
axis2_stub_get_call_obj(axis2_stub_t *stub,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(stub)->call;
}

/**
 *
 * @param doRest
 */
/*    public void setDoREST(boolean doRest) {
        this.doRest = doRest;
    }
*/

/*
protected void setValueRPC(SOAPEnvelope env,
                           String methodNamespaceURI,
                           String methodName,
                           String[] paramNames,
                           Object[] values) {
    SOAPBody body = env.getBody();
    OMFactory fac = this.getFactory(this.soapVesrion);

    OMNamespace methodNamespace = fac.createOMNamespace(methodNamespaceURI,
            "ns1");
    OMElement elt = fac.createOMElement(methodName, methodNamespace);
    if (paramNames != null) {
        //find the relevant object here, convert it and add it to the elt
        for (int i = 0; i < paramNames.length; i++) {
            String paramName = paramNames[i];
            Object value = values[i];
            elt.addChild(StubSupporter.createRPCMappedElement(paramName,
                    fac.createOMNamespace("", null), //empty namespace
                    value,
                    fac));
        }
    }
    body.addChild(elt);
}
*/

/*
public void _setSessionInfo(String key, Object value) throws java.lang.Exception {
    if (!_maintainSession) {
        //TODO Comeup with a Exception
        throw new java.lang.Exception(
                "Client is running the session OFF mode: Start session before saving to a session ");
    }
    _configurationContext.getServiceContext(_currentSessionId).setProperty(
            key, value);
}


public Object _getSessionInfo(String key) throws java.lang.Exception {
    if (!_maintainSession) {
        //TODO Comeup with a Exception
        throw new java.lang.Exception(
                "Client is running the session OFF mode: Start session before saving to a session ");
    }
    return _configurationContext.getServiceContext(_currentSessionId)
            .getProperty(key);
}

public void _startSession() {
    _maintainSession = true;
    _currentSessionId = getID();
}

public void _endSession() {
    _maintainSession = false;
}
*/
