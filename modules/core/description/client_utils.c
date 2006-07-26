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

#include <axis2_string.h>
#include <axis2_url.h>
#include <axis2_uri.h>
#include <axis2_qname.h>
#include <axis2_options.h>
#include <axiom_util.h>
#include <axis2_param.h>
#include <axis2_client_utils.h>
#include <woden_resolver.h>
#include <woden_nc_name.h>
#include <woden_desc.h>
#include <woden_svc.h>
#include <woden_endpoint.h>
#include <woden_binding.h>
#include <woden_binding_op.h>
#include <woden_interface_op.h>
#include <woden_interface_msg_ref.h>
#include <woden_direction.h>
#include <woden_soap_binding_op_exts.h>
#include <woden_wsdl10_desc.h>
#include <woden_wsdl10_svc.h>
#include <woden_wsdl10_endpoint.h>
#include <woden_wsdl10_binding_op.h>
#include <woden_wsdl10_interface_msg_ref.h>
#include <woden_wsdl10_soap_binding_op_exts.h>
#include <woden_wsdl10_soap_module.h>
#include <woden_ext_element.h>

/**
 * Utility methods for various clients to use.
 */

/**
     * To create a Axis Service for a given WSDL and the created client is most 
     * suitable for client side invocation not for server side invocation, since 
     * all the soap acction and wsa action is added to operations
     *
     * @param wsdl_url          location of the WSDL
     * @param wsdl_svc_qname    name of the service to be invoke , if it is null 
     *                          then the first one will
     *                          be selected if there are more than one
     * @param ep_name           name of the endpoint , if there are more than one , 
     *                          if it is null then the
     *                          first one in the  iterator will be selected
     * @param options           Service client options, to set the target EPR
     * @return axis2_svc ,      the created servie will be return
     */
AXIS2_EXTERN axis2_svc_t * AXIS2_CALL
axis2_client_utils_create_axis2_svc(
        const axis2_env_t *env,
        const axis2_uri_t *wsdl_uri,
        const axis2_qname_t *wsdl_svc_qname,
        const axis2_char_t *ep_name,
        const axis2_char_t *doc_base_uri,
        axis2_options_t *options)
{
    axis2_svc_t *axis2_svc = NULL;
    void *wsdl_svc = NULL;
    axiom_document_t *doc = NULL;
    woden_resolver_t *resolver = NULL;
    void *desc = NULL;
    axis2_bool_t svc_found = AXIS2_FALSE;
    axis2_bool_t endpoint_found = AXIS2_FALSE;
    axis2_qname_t *svc_qname = NULL;
    axis2_array_list_t *wsdl_svcs = NULL;
    axis2_array_list_t *endpoints = NULL;
    axis2_array_list_t *binding_ops = NULL;
    woden_nc_name_t *ep_ncname = NULL;
    void *endpoint = NULL;
    void *binding = NULL;
    int spec = 0;

    doc = axiom_util_new_document(env, wsdl_uri);
    resolver = woden_resolver_create(env);
    
    desc = WODEN_RESOLVER_READ(resolver, env, doc, (axis2_char_t *)doc_base_uri);
    spec = WODEN_RESOLVER_GET_SPEC(resolver, env);
    if(WODEN_WSDL20 == spec)
    {
        int no_of_svcs = 0;
        int no_of_endpoints = 0;
        int no_of_binding_ops = 0;
        int no_of_interface_msg_refs = 0;
        axis2_svc = axis2_svc_create(env);
        int i = 0;
        
        wsdl_svcs = WODEN_DESC_GET_SVCS(desc, env);
        if(wsdl_svcs)
            no_of_svcs = AXIS2_ARRAY_LIST_SIZE(wsdl_svcs, env);
        for(i = 0; i < no_of_svcs; i++)
        {
            wsdl_svc = AXIS2_ARRAY_LIST_GET(wsdl_svcs, env, i);
            svc_qname = WODEN_SVC_GET_QNAME(wsdl_svc, env);
            /* if wsdl_svc_qname is NULL we take the first service as the
             * requested service
             */
            if(!wsdl_svc_qname)
            {
                svc_found = AXIS2_TRUE;
                break;
            }
            if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(svc_qname, env, 
                        (axis2_qname_t *) wsdl_svc_qname))
            {
                svc_found = AXIS2_TRUE;
                break;
            }
        }
        if(AXIS2_TRUE == svc_found)
        {
            axis2_char_t *localname = AXIS2_QNAME_GET_LOCALPART(svc_qname, env);
            AXIS2_SVC_SET_NAME(axis2_svc, env, localname);
            AXIS2_SVC_SET_QNAME(axis2_svc, env, svc_qname);
        }
        endpoints = WODEN_SVC_GET_ENDPOINTS(wsdl_svc, env);
        if(endpoints)
            no_of_endpoints = AXIS2_ARRAY_LIST_SIZE(endpoints, env);
        for(i = 0; i < no_of_endpoints; i++)
        {
            endpoint = AXIS2_ARRAY_LIST_GET(endpoints, env, i);
            ep_ncname = WODEN_ENDPOINT_GET_NAME(endpoint, env);
            /* if endpoint_name is NULL we take the first endpoint as the
             * requested service
             */
            if(!ep_name)
            {
                endpoint_found = AXIS2_TRUE;
                break;
            }
            if(0 == AXIS2_STRCMP(ep_name, WODEN_NC_NAME_TO_STRING(ep_ncname, env)))
            {
                endpoint_found = AXIS2_TRUE;
                break;
            }
        }
        if(AXIS2_TRUE == endpoint_found)
        {
            axis2_uri_t *soap_address = NULL;
            axis2_endpoint_ref_t *endpoint_ref = NULL;
            axis2_char_t *address = NULL;

            soap_address = WODEN_ENDPOINT_GET_ADDRESS(endpoint, env);
            if(soap_address)
            {
                address = AXIS2_URI_TO_STRING(soap_address, env, 
                        AXIS2_URI_UNP_OMITUSERINFO);
                endpoint_ref = axis2_endpoint_ref_create(env, address);
                AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);
            }
        }

        if(endpoint)
            binding = WODEN_ENDPOINT_GET_BINDING(endpoint, env);
        if(binding)
            binding_ops = WODEN_BINDING_GET_BINDING_OPS(binding, env);
        if(binding_ops)
            no_of_binding_ops = AXIS2_ARRAY_LIST_SIZE(binding_ops, env);
        for(i = 0; i < no_of_binding_ops; i++)
        {
            void *binding_op = NULL;
            void *interface_op = NULL;
            axis2_op_t *axis2_op = NULL;
            void *soap_op = NULL;
            axis2_uri_t *soap_action_uri = NULL;
            axis2_array_list_t *interface_msg_refs = NULL;
            axis2_array_list_t *ext_elements = NULL;
            int j = 0;
            axis2_bool_t in = AXIS2_FALSE;
            axis2_bool_t out = AXIS2_FALSE;
            axis2_qname_t *op_qname = NULL;
            axis2_param_t *param = NULL;
            
            binding_op = AXIS2_ARRAY_LIST_GET(binding_ops, env, i);
            interface_op = WODEN_BINDING_OP_GET_INTERFACE_OP(binding_op, env);
            interface_msg_refs = WODEN_INTERFACE_OP_GET_INTERFACE_MSG_REFS(
                    interface_op, env);
            if(interface_msg_refs)
                no_of_interface_msg_refs = AXIS2_ARRAY_LIST_SIZE(
                        interface_msg_refs, env);
            for(j = 0; j < no_of_interface_msg_refs; j++)
            {
                void *interface_msg_ref = NULL; 
                void *direction = NULL;
                axis2_char_t *str_direction = NULL;
                
                interface_msg_ref = AXIS2_ARRAY_LIST_GET(interface_msg_refs, env, j);
                direction = WODEN_INTERFACE_MSG_REF_GET_DIRECTION(
                        interface_msg_ref, env);
                str_direction = WODEN_DIRECTION_TO_STRING(direction, env);
                if(0 == AXIS2_STRCMP(str_direction, "in"))
                {
                    in = AXIS2_TRUE;
                }
                if(0 == AXIS2_STRCMP(str_direction, "out"))
                {
                    out = AXIS2_TRUE;
                }
            }
            axis2_op = axis2_op_create(env);
            op_qname = WODEN_INTERFACE_OP_GET_QNAME(interface_op, env);
            AXIS2_OP_SET_QNAME(axis2_op, env, op_qname);
            binding_op = woden_binding_op_to_element_extensible(binding_op, env);
            ext_elements = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(binding_op, 
                    env);
            soap_op = AXIS2_ARRAY_LIST_GET(ext_elements, env, 0);
            soap_action_uri = WODEN_SOAP_BINDING_OP_EXTS_GET_SOAP_ACTION(
                    soap_op, env);
            param = axis2_param_create(env, AXIS2_SOAP_ACTION, soap_action_uri);
            AXIS2_OP_ADD_PARAM(axis2_op, env, param);
            AXIS2_SVC_ADD_OP(axis2_svc, env, axis2_op);
        }
    }
    if(WODEN_WSDL10 == spec)
    {

        axis2_svc = axis2_svc_create(env);
        int no_of_svcs = 0;
        int no_of_endpoints = 0;
        int no_of_binding_ops = 0;
        int no_of_interface_msg_refs = 0;
        int i = 0;
        
        wsdl_svcs = WODEN_WSDL10_DESC_GET_SVCS(desc, env);
        if(wsdl_svcs)
            no_of_svcs = AXIS2_ARRAY_LIST_SIZE(wsdl_svcs, env);
        for(i = 0; i < no_of_svcs; i++)
        {
            wsdl_svc = AXIS2_ARRAY_LIST_GET(wsdl_svcs, env, i);
            svc_qname = WODEN_SVC_GET_QNAME(wsdl_svc, env);
            /* if wsdl_svc_qname is NULL we take the first service as the
             * requested service
             */
            if(!wsdl_svc_qname)
            {
                svc_found = AXIS2_TRUE;
                break;
            }
            if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(svc_qname, env, 
                        (axis2_qname_t *) wsdl_svc_qname))
            {
                svc_found = AXIS2_TRUE;
                break;
            }
        }
        if(AXIS2_TRUE == svc_found)
        {
            axis2_char_t *localname = AXIS2_QNAME_GET_LOCALPART(svc_qname, env);
            AXIS2_SVC_SET_NAME(axis2_svc, env, localname);
            AXIS2_SVC_SET_QNAME(axis2_svc, env, svc_qname);
        }
        endpoints = WODEN_SVC_GET_ENDPOINTS(wsdl_svc, env);
        if(endpoints)
            no_of_endpoints = AXIS2_ARRAY_LIST_SIZE(endpoints, env);
        for(i = 0; i < no_of_endpoints; i++)
        {
            endpoint = AXIS2_ARRAY_LIST_GET(endpoints, env, i);
            ep_ncname = WODEN_WSDL10_ENDPOINT_GET_NAME(endpoint, env);
            /* if endpoint_name is NULL we take the first endpoint as the
             * requested service
             */
            if(!ep_name)
            {
                endpoint_found = AXIS2_TRUE;
                break;
            }
            if(0 == AXIS2_STRCMP(ep_name, WODEN_NC_NAME_TO_STRING(ep_ncname, env)))
            {
                endpoint_found = AXIS2_TRUE;
                break;
            }
        }
        if(AXIS2_TRUE == endpoint_found)
        {
            axis2_uri_t *soap_address = NULL;
            axis2_endpoint_ref_t *endpoint_ref = NULL;
            axis2_char_t *address = NULL;

            soap_address = WODEN_WSDL10_ENDPOINT_GET_ADDRESS(endpoint, env);
            if(soap_address)
            {
                address = AXIS2_URI_TO_STRING(soap_address, env, 
                        AXIS2_URI_UNP_OMITUSERINFO);
                endpoint_ref = axis2_endpoint_ref_create(env, address);
                AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);
            }
        }
        if(endpoint)
            binding = WODEN_WSDL10_ENDPOINT_GET_BINDING(endpoint, env);
        if(binding)
            binding_ops = WODEN_BINDING_GET_BINDING_OPS(binding, env);
        if(binding_ops)
            no_of_binding_ops = AXIS2_ARRAY_LIST_SIZE(binding_ops, env);
        for(i = 0; i < no_of_binding_ops; i++)
        {
            void *binding_op = NULL;
            void *interface_op = NULL;
            axis2_op_t *axis2_op = NULL;
            void *ext_element = NULL;
            axis2_array_list_t *interface_msg_refs = NULL;
            axis2_array_list_t *ext_elements = NULL;
            int j = 0, size = 0;
            axis2_bool_t in = AXIS2_FALSE;
            axis2_bool_t out = AXIS2_FALSE;
            axis2_qname_t *op_qname = NULL;
            axis2_qname_t *ext_type = NULL;
            axis2_qname_t *ext_type_l = NULL;
            axis2_param_t *param = NULL;
            
            binding_op = AXIS2_ARRAY_LIST_GET(binding_ops, env, i);
            interface_op = WODEN_WSDL10_BINDING_OP_GET_INTERFACE_OP(binding_op, 
                    env);
            interface_msg_refs = WODEN_INTERFACE_OP_GET_INTERFACE_MSG_REFS(
                    interface_op, env);
            if(interface_msg_refs)
                no_of_interface_msg_refs = AXIS2_ARRAY_LIST_SIZE(
                        interface_msg_refs, env);
            for(j = 0; j < no_of_interface_msg_refs; j++)
            {
                void *interface_msg_ref = NULL; 
                void *direction = NULL;
                axis2_char_t *str_direction = NULL;
                
                interface_msg_ref = AXIS2_ARRAY_LIST_GET(interface_msg_refs, 
                        env, j);
                direction = WODEN_WSDL10_INTERFACE_MSG_REF_GET_DIRECTION(
                        interface_msg_ref, env);
                str_direction = WODEN_DIRECTION_TO_STRING(direction, env);
                if(0 == AXIS2_STRCMP(str_direction, "in"))
                {
                    in = AXIS2_TRUE;
                }
                if(0 == AXIS2_STRCMP(str_direction, "out"))
                {
                    out = AXIS2_TRUE;
                }
            }
            axis2_op = axis2_op_create(env);
            op_qname = WODEN_INTERFACE_OP_GET_QNAME(interface_op, env);
            AXIS2_OP_SET_QNAME(axis2_op, env, op_qname);
            binding_op = woden_wsdl10_binding_op_to_element_extensible(
                    binding_op, env);
            ext_type_l = axis2_qname_create(env, "operation", 
                    "http://schemas.xmlsoap.org/wsdl/soap/", NULL);
            ext_elements = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(binding_op, 
                    env);
            if(ext_elements)
                size = AXIS2_ARRAY_LIST_SIZE(ext_elements, env);
            for(j = 0; j < size; j++)
            {
                ext_element = AXIS2_ARRAY_LIST_GET(ext_elements, env, j);
                ext_type = WODEN_EXT_ELEMENT_GET_EXT_TYPE(ext_element, env);
                if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(ext_type, env, ext_type_l))
                {
                    void *soap_binding_op = NULL;
                    axis2_uri_t *soap_action_uri = NULL;
                    
                    ext_element = 
                       woden_wsdl10_soap_module_to_soap_module_element (
                                ext_element, env);
                    soap_binding_op = 
                        WODEN_WSDL10_SOAP_MODULE_ELEMENT_GET_SOAP_BINDING_OP_EXTS(
                                ext_element, env);
                    
                    soap_action_uri = WODEN_WSDL10_SOAP_BINDING_OP_EXTS_GET_SOAP_ACTION(
                        soap_binding_op, env);
                    param = axis2_param_create(env, AXIS2_SOAP_ACTION, soap_action_uri);
                    AXIS2_OP_ADD_PARAM(axis2_op, env, param);
                    AXIS2_SVC_ADD_OP(axis2_svc, env, axis2_op);
                    break;
                }
            }
        }
    }
    return axis2_svc; 
}

