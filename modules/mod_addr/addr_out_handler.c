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

#include <axis2_handler_desc.h>
#include <axis2_qname.h>
#include <axis2_svc.h>
#include <axiom_soap_header.h>
#include <axiom_soap_body.h>
#include <axis2_addr.h>
#include <axiom_soap_header_block.h>
#include <axis2_endpoint_ref.h>
#include <axis2_property.h>
#include <stdio.h>

axis2_status_t AXIS2_CALL
axis2_addr_out_handler_invoke (struct axis2_handler *handler,
                               const axis2_env_t *env,
                               struct axis2_msg_ctx *msg_ctx);


axis2_status_t 
axis2_addr_out_handler_add_to_soap_header (const axis2_env_t *env,
                                           axis2_endpoint_ref_t *
                                           endpoint_ref, 
                                           const axis2_char_t * type,
                                           axiom_soap_header_t * soap_header,
                                           const axis2_char_t * addr_ns);


axis2_status_t
axis2_addr_out_handler_add_to_header (const axis2_env_t *env,
                                      axis2_endpoint_ref_t * epr,
                                      axiom_node_t ** parent_node,
                                      const axis2_char_t * addr_ns);

axis2_status_t
axis2_addr_out_handler_process_any_content_type (const axis2_env_t *env,
                                                 axis2_any_content_type_t *
                                                 reference_values,
                                                 axiom_node_t *
                                                 parent_ele_node,
                                                 const axis2_char_t * addr_ns);

axiom_node_t *axis2_addr_out_handler_process_string_info (const axis2_env_t *env,
                                                             const axis2_char_t *value,
                                                             const axis2_char_t *type,
                                                             axiom_soap_header_t **soap_header,
                                                             const axis2_char_t *addr_ns);


AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
axis2_addr_out_handler_create (const axis2_env_t *env, axis2_qname_t * qname)
{
    axis2_handler_t *handler = NULL;
    axis2_qname_t *handler_qname = NULL;

    AXIS2_ENV_CHECK (env, NULL);

    if (qname)
    {
        handler_qname = AXIS2_QNAME_CLONE (qname, env);
        if (!(handler_qname))
        {
            AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY,
                             AXIS2_FAILURE);
            return NULL;
        }
    }
    else
    {
        /* create default qname */
        handler_qname = axis2_qname_create (env, "addr_out_handler",
                                            "http://axis.ws.apache.org",
                                            NULL);
        if (!handler_qname)
        {
            return NULL;
        }
    }

    handler = axis2_handler_create (env);
    if (!handler)
    {
        return NULL;
    }

    /* set the base struct's invoke op */
    if (handler->ops)
        handler->ops->invoke = axis2_addr_out_handler_invoke;

    AXIS2_QNAME_FREE(handler_qname, env);

    return handler;
}

axis2_status_t AXIS2_CALL
axis2_addr_out_handler_invoke (struct axis2_handler * handler,
                               const axis2_env_t *env, axis2_msg_ctx_t * msg_ctx)
{
    axis2_char_t *addressing_version_from_msg_ctx = NULL;
    const axis2_char_t *addr_ns = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_ctx_t *ctx = NULL;
    axiom_namespace_t *addressing_namespace = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_node_t *soap_header_node = NULL;
    axiom_element_t *soap_header_ele = NULL;
    axis2_endpoint_ref_t *epr = NULL;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);

    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS (msg_ctx, env);
    if (!msg_info_headers)
        return AXIS2_SUCCESS; /* no addressing in use */
    if (!AXIS2_MSG_INFO_HEADERS_GET_ACTION (msg_info_headers, env))
        return AXIS2_SUCCESS; /* If no action present, assume no addressing in use */


    ctx = AXIS2_MSG_CTX_GET_BASE (msg_ctx, env);
    property =
        AXIS2_CTX_GET_PROPERTY (ctx, env, AXIS2_WSA_VERSION, AXIS2_FALSE);
    if(property)
    {
        addressing_version_from_msg_ctx = AXIS2_PROPERTY_GET_VALUE(property, 
            env);
        property = NULL;
    }

    if (addressing_version_from_msg_ctx)
    {
        if (AXIS2_STRCMP
            (AXIS2_WSA_NAMESPACE, addressing_version_from_msg_ctx) == 0)
        {
            addr_ns = AXIS2_WSA_NAMESPACE;
        }
        else
        {
            addr_ns = AXIS2_WSA_NAMESPACE_SUBMISSION;
        }
    }
    else if (AXIS2_MSG_CTX_GET_OP_CTX (msg_ctx, env) != NULL)
    {
        axis2_op_ctx_t *op_ctx = NULL;
        axis2_msg_ctx_t *in_msg_ctx = NULL;
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX (msg_ctx, env);
        if (op_ctx)
        {
            in_msg_ctx =
                AXIS2_OP_CTX_GET_MSG_CTX (op_ctx, env,
                                          AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE);
        }

        if (!in_msg_ctx)
        {
            addr_ns = AXIS2_WSA_NAMESPACE;   /* setting Submission version as the default addressing namespace */
        }
        else
        {
            axis2_ctx_t *in_ctx = NULL;
            in_ctx = AXIS2_MSG_CTX_GET_BASE (in_msg_ctx, env);

            property =
                AXIS2_CTX_GET_PROPERTY (in_ctx, env, AXIS2_WSA_VERSION,
                                        AXIS2_FALSE);
            if(property)
            {
                addr_ns = AXIS2_PROPERTY_GET_VALUE(property, env);
                property = NULL;
            }

            if (!addr_ns)
            {
                addr_ns = AXIS2_WSA_NAMESPACE;   /* Addressing version has not been set in the IN path */
            }
        }
    }

    if (!addr_ns || AXIS2_STRCMP ("", addr_ns) == 0)
    {
        addr_ns = AXIS2_WSA_NAMESPACE;
    }

    addressing_namespace = axiom_namespace_create (env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS (msg_ctx, env);
    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE (msg_ctx, env);
    if (!soap_envelope)
    {
        AXIOM_NAMESPACE_FREE(addressing_namespace, env);
        return AXIS2_SUCCESS; /* can happen in case of one way services/clients */
    }
    soap_header  = AXIOM_SOAP_ENVELOPE_GET_HEADER (soap_envelope, env);
  
    if (!soap_header)
    {
        AXIOM_NAMESPACE_FREE(addressing_namespace, env);
        return AXIS2_SUCCESS; /*No SOAP header, so no point proceeding*/
    }

    /* by this time, we definitely have some addressing information to be sent. This is because,
       // we have tested at the start of this whether msg_info_headers are null or not.
       // So rather than declaring addressing namespace in each and every addressing header, lets
       // define that in the Header itself. */
    if (soap_header)
    {
        const axis2_char_t *action = NULL;
        const axis2_char_t *address = NULL;
        const axis2_char_t *svc_group_context_id = NULL;
        const axis2_char_t *message_id =  NULL;
        axis2_relates_to_t *relates_to = NULL;
        axiom_node_t *relates_to_header_node = NULL;
        axiom_element_t *relates_to_header_ele = NULL;

        soap_header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE (soap_header, env);
        soap_header_ele =
            (axiom_element_t *)
            AXIOM_NODE_GET_DATA_ELEMENT (soap_header_node, env);
        AXIOM_ELEMENT_DECLARE_NAMESPACE (soap_header_ele, env,
                                            soap_header_node, addressing_namespace);

        epr = AXIS2_MSG_INFO_HEADERS_GET_TO (msg_info_headers, env);

        if (soap_envelope && epr)
        {
            axiom_soap_body_t *body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
            if (body)
            {
                /* in case of a SOAP fault, we got to send the response to 
                   the adress specified by FaultTo */
                if (AXIOM_SOAP_BODY_HAS_FAULT( body, env))
                {
                    axis2_endpoint_ref_t *fault_epr = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO (msg_info_headers, env); 
                    if (fault_epr)
                    {
                        const axis2_char_t *fault_address = AXIS2_ENDPOINT_REF_GET_ADDRESS(fault_epr, env);
                        if (fault_address)
                        {
                            if (AXIS2_STRCMP(AXIS2_WSA_NONE_URL, fault_address) != 0 &&
                                    AXIS2_STRCMP(AXIS2_WSA_NONE_URL_SUBMISSION, fault_address) != 0)
                            {
                                AXIS2_ENDPOINT_REF_SET_ADDRESS (epr, env, fault_address);
                            }
                        }
                    }
                }
            }
        }
    
        if (epr) 
        {
            address = AXIS2_ENDPOINT_REF_GET_ADDRESS (epr, env);
            if (address && AXIS2_STRCMP (address, "") != 0)
            {
                axiom_node_t *to_header_block_node = NULL;
                axiom_soap_header_block_t *to_header_block = NULL;
                
                to_header_block  =
                    AXIOM_SOAP_HEADER_ADD_HEADER_BLOCK (soap_header, env,
                                                        AXIS2_WSA_TO,
                                                        addressing_namespace);
                to_header_block_node =
                    AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE (to_header_block, env);
                if (to_header_block_node)
                {
                    axiom_element_t *to_header_block_element = NULL;
                    to_header_block_element = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(to_header_block_node, env);
                    if (to_header_block_element)
                    {
                        AXIOM_ELEMENT_SET_TEXT(to_header_block_element, env, address, to_header_block_node);
                    }
                }
            }
        }

        action = AXIS2_MSG_INFO_HEADERS_GET_ACTION (msg_info_headers, env);
        if (action)
        {
            axis2_addr_out_handler_process_string_info (env, action,
                                                        AXIS2_WSA_ACTION,
                                                        &soap_header,
                                                        addr_ns);
        }

        epr = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO (msg_info_headers, env);
        if (!epr)
        {
            const axis2_char_t *anonymous_uri = NULL;
            axis2_bool_t anonymous = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO_ANONYMOUS(msg_info_headers, env);
            axis2_bool_t none = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO_NONE(msg_info_headers, env);
            if (AXIS2_STRCMP (addr_ns, AXIS2_WSA_NAMESPACE_SUBMISSION) == 0)
            {
                if (none)
                    anonymous_uri = AXIS2_WSA_NONE_URL_SUBMISSION;
                else if (anonymous)
                    anonymous_uri = AXIS2_WSA_ANONYMOUS_URL_SUBMISSION;
            }
            else
            {
                if (none)
                    anonymous_uri = AXIS2_WSA_NONE_URL;
                else if (anonymous)
                    anonymous_uri = AXIS2_WSA_ANONYMOUS_URL;
            }

            if (anonymous_uri)
                epr = axis2_endpoint_ref_create (env, anonymous_uri);
            if(epr)
                AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(msg_info_headers, env, epr);
        }


        /* add the service group id as a reference parameter */
        svc_group_context_id = AXIS2_MSG_CTX_GET_SVC_GRP_CTX_ID (msg_ctx, env);
        
        if (svc_group_context_id
            && AXIS2_STRCMP (svc_group_context_id, "") != 0)
        {
        }

        axis2_addr_out_handler_add_to_soap_header (env, epr,
                                                   AXIS2_WSA_REPLY_TO,
                                                   soap_header, addr_ns);

        /* It is wrong freeing the epr here. Instead I set the locally
         * created epr to msg_info_headers just after creating it
         */
        /*if(NULL != epr)
        {
            AXIS2_ENDPOINT_REF_FREE(epr, env);
            epr = NULL;
        }*/

        epr = AXIS2_MSG_INFO_HEADERS_GET_FROM (msg_info_headers, env);

        if (epr)
        {
            axis2_addr_out_handler_add_to_soap_header (env, epr,
                                                       AXIS2_WSA_FROM,
                                                       soap_header, addr_ns);

        }

        epr = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO (msg_info_headers, env);
        if (!epr)
        {
            const axis2_char_t *anonymous_uri = NULL;
            axis2_bool_t anonymous = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO_ANONYMOUS(msg_info_headers, env);
            axis2_bool_t none = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO_NONE(msg_info_headers, env);
            if (AXIS2_STRCMP (addr_ns, AXIS2_WSA_NAMESPACE_SUBMISSION) == 0)
            {
                if (none)
                    anonymous_uri = AXIS2_WSA_NONE_URL_SUBMISSION;
                else if (anonymous)
                    anonymous_uri = AXIS2_WSA_ANONYMOUS_URL_SUBMISSION;
            }
            else
            {
                if (none)
                    anonymous_uri = AXIS2_WSA_NONE_URL;
                else if (anonymous)
                    anonymous_uri = AXIS2_WSA_ANONYMOUS_URL;
            }

            if (anonymous_uri)
                epr = axis2_endpoint_ref_create (env, anonymous_uri);
        }

        if (epr)
        {
            /* optional */
            axis2_addr_out_handler_add_to_soap_header (env, epr,
                                                       AXIS2_WSA_FAULT_TO,
                                                       soap_header, addr_ns);
        }

        message_id = AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID (msg_info_headers, env);
        
        if (message_id)
        {
            axis2_addr_out_handler_process_string_info (env, message_id,
                                                        AXIS2_WSA_MESSAGE_ID,
                                                        &soap_header,
                                                        addr_ns);
        }

        relates_to = AXIS2_MSG_INFO_HEADERS_GET_RELATES_TO (msg_info_headers, env);
        if (relates_to)
        {
            axis2_char_t *value = NULL;
            value = AXIS2_RELATES_TO_GET_VALUE (relates_to, env);
            relates_to_header_node =
                axis2_addr_out_handler_process_string_info (env, value,
                                                            AXIS2_WSA_RELATES_TO,
                                                            &soap_header,
                                                            addr_ns);
        }

        if (relates_to_header_node)
        {
            axis2_char_t *relationship_type = NULL;
            relationship_type = AXIS2_RELATES_TO_GET_RELATIONSHIP_TYPE (relates_to, env);
            if (AXIS2_STRCMP (relationship_type, "") != 0)
            {
                axiom_attribute_t *om_attr = NULL;
                axiom_namespace_t *addr_ns_obj = NULL;
                axiom_namespace_t *dec_ns = NULL;
                relates_to_header_ele =
                    (axiom_element_t *)
                    AXIOM_NODE_GET_DATA_ELEMENT (relates_to_header_node,
                                                    env);
                if(NULL != relates_to_header_ele)
                {
                    dec_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE(relates_to_header_ele, env,
                                                 addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                    if(NULL != dec_ns)
                    {
                        addr_ns_obj = dec_ns;
                    }
                    else
                    {
                        addr_ns_obj = axiom_namespace_create (env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                    }
                    
                    om_attr = axiom_attribute_create (env,
                            AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE,
                            AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE,
                            addr_ns_obj);
                            
                    AXIOM_ELEMENT_ADD_ATTRIBUTE (relates_to_header_ele, env,
                                om_attr, relates_to_header_node);

                }                                
                if(NULL != relates_to_header_ele)
                {
                    dec_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE(relates_to_header_ele, env,
                                                         addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                    if(NULL == dec_ns)
                    {
                        dec_ns = AXIOM_ELEMENT_FIND_NAMESPACE(relates_to_header_ele, env, 
                                    relates_to_header_node, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
                        if(NULL != dec_ns)
                        {
                            AXIOM_NAMESPACE_FREE(addr_ns_obj, env);
                            addr_ns_obj = NULL ;
                            AXIOM_ATTRIBUTE_SET_NAMESPACE(om_attr, env, dec_ns);
                        }                            
                    }
                }
                                                                                                                                                                                                                                 
            }
        }
    }
    return AXIS2_SUCCESS;
}


axiom_node_t *
axis2_addr_out_handler_process_string_info (const axis2_env_t *env,
                                            const axis2_char_t * value,
                                            const axis2_char_t * type,
                                            axiom_soap_header_t **
                                            soap_header_p,
                                            const axis2_char_t * addr_ns)
{
    axiom_soap_header_t *soap_header = NULL;
    axiom_soap_header_block_t *header_block = NULL;
    axiom_node_t *header_block_node = NULL;
    axiom_element_t *header_block_ele = NULL;

    AXIS2_ENV_CHECK (env, NULL);
    AXIS2_PARAM_CHECK (env->error, value, NULL);
    AXIS2_PARAM_CHECK (env->error, type, NULL);
    AXIS2_PARAM_CHECK (env->error, soap_header_p, NULL);
    AXIS2_PARAM_CHECK (env->error, addr_ns, NULL);

    soap_header = *(soap_header_p);

    if (value && AXIS2_STRCMP (value, "") != 0)
    {
        axiom_namespace_t *addr_ns_obj = NULL;
        addr_ns_obj =
            axiom_namespace_create (env, addr_ns,
                                       AXIS2_WSA_DEFAULT_PREFIX);
        header_block =
            AXIOM_SOAP_HEADER_ADD_HEADER_BLOCK (soap_header, env, type,
                                                addr_ns_obj);
        header_block_node =
            AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE (header_block, env);
        header_block_ele =
            (axiom_element_t *)
            AXIOM_NODE_GET_DATA_ELEMENT (header_block_node, env);
        if(NULL != header_block_ele)
        {
            axiom_namespace_t *dec_ns = NULL;
            AXIOM_ELEMENT_SET_TEXT (header_block_ele, env, value,
                                        header_block_node);
            dec_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE(header_block_ele, env,
                                                 addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
            if(! dec_ns)
            {
                 AXIOM_NAMESPACE_FREE(addr_ns_obj, env);
                 addr_ns_obj = NULL;
            }
        }
    }
    return header_block_node;
}

axis2_status_t
axis2_addr_out_handler_add_to_soap_header (const axis2_env_t *env,
                                           axis2_endpoint_ref_t *
                                           endpoint_ref, 
                                           const axis2_char_t * type,
                                           axiom_soap_header_t * soap_header,
                                           const axis2_char_t * addr_ns)
{
    axiom_soap_header_block_t *header_block = NULL;
    const axis2_char_t *address = NULL;
    axis2_array_list_t *ref_param_list = NULL;
    axis2_array_list_t *meta_data_list = NULL;
    axis2_array_list_t *extension_list = NULL;
    axiom_node_t *header_block_node = NULL;
    axiom_node_t *header_node = NULL;
    axiom_namespace_t *addr_ns_obj = NULL;
    
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, endpoint_ref, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, soap_header, AXIS2_FAILURE);

    header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE (soap_header, env);
    
    addr_ns_obj = axiom_namespace_create (env, 
                        addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
    header_block =
        AXIOM_SOAP_HEADER_ADD_HEADER_BLOCK (soap_header, env, type,
                                            addr_ns_obj);

    if(NULL !=  addr_ns_obj)
    {
        AXIOM_NAMESPACE_FREE(addr_ns_obj, env);
        addr_ns_obj = NULL;
    }

    address = AXIS2_ENDPOINT_REF_GET_ADDRESS (endpoint_ref, env);
    if (address && AXIS2_STRCMP ("", address) != 0)
    {
        axiom_node_t *hb_node = NULL;
        axiom_element_t *hb_ele = NULL;
        axiom_node_t *address_node = NULL;
        axiom_element_t *address_ele = NULL;
        hb_node = AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE (header_block, env);
        hb_ele = (axiom_element_t *) 
                    AXIOM_NODE_GET_DATA_ELEMENT (hb_node, env);

        addr_ns_obj = axiom_namespace_create (env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
        
        address_ele =
            axiom_element_create (env, hb_node, EPR_ADDRESS, addr_ns_obj,
                                     &address_node);
        if(NULL != address_ele)
        {
            axiom_namespace_t *dec_ns = NULL;
            AXIOM_ELEMENT_SET_TEXT (address_ele, env, address, address_node);
            dec_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE(address_ele, env, 
                                                 addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
            if(! dec_ns)
            {
                 AXIOM_NAMESPACE_FREE(addr_ns_obj, env);
                 addr_ns_obj = NULL;
            }
        }
    }

    header_block_node =
        AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE (header_block, env);
    axis2_addr_out_handler_add_to_header (env, endpoint_ref,
                                          &header_block_node, addr_ns);

    ref_param_list = AXIS2_ENDPOINT_REF_GET_REF_PARAM_LIST(endpoint_ref, env);
    if (ref_param_list && AXIS2_ARRAY_LIST_SIZE(ref_param_list, env) > 0)
    {
        axiom_node_t *reference_node = NULL;
        axiom_element_t *reference_ele = NULL;
        axis2_array_list_t *ref_attribute_list = NULL;
        int i = 0;
        
        addr_ns_obj = axiom_namespace_create (env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
         
        reference_ele = axiom_element_create (env,
                                                 header_block_node,
                                                 EPR_REFERENCE_PARAMETERS,
                                                 addr_ns_obj,
                                                 &reference_node);
        
        ref_attribute_list = AXIS2_ENDPOINT_REF_GET_REF_ATTRIBUTE_LIST(endpoint_ref, env);
        if (ref_attribute_list)
        {
            int j = 0;
            for (j = 0; j < AXIS2_ARRAY_LIST_SIZE(ref_attribute_list, env); j++)
            {
                axiom_attribute_t *attr = (axiom_attribute_t *)AXIS2_ARRAY_LIST_GET(ref_attribute_list, env, j);
                if (attr)
                {
                    AXIOM_ELEMENT_ADD_ATTRIBUTE(reference_ele, env, attr, reference_node);
                }
            }
        }
        
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(ref_param_list, env); i++)
        {
            axiom_node_t *ref_node = (axiom_node_t *)AXIS2_ARRAY_LIST_GET(ref_param_list, env, i);
            if (ref_node)
            {
                AXIOM_NODE_ADD_CHILD(reference_node, env, ref_node);
            }
        }

    }
    
    meta_data_list = AXIS2_ENDPOINT_REF_GET_METADATA_LIST(endpoint_ref, env);
    if (meta_data_list && AXIS2_ARRAY_LIST_SIZE(meta_data_list, env) > 0)
    {
        axiom_node_t *reference_node = NULL;
        axiom_element_t *reference_ele = NULL;
        axis2_array_list_t *meta_attribute_list = NULL;
        int i = 0;
        
        if (!reference_node) /* may be we alredy created this in ref params block */
        {
            addr_ns_obj = axiom_namespace_create (env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
         
            reference_ele = axiom_element_create (env,
                                                 header_block_node,
                                                 AXIS2_WSA_METADATA,
                                                 addr_ns_obj,
                                                 &reference_node);
        }
        
        meta_attribute_list = AXIS2_ENDPOINT_REF_GET_META_ATTRIBUTE_LIST(endpoint_ref, env);
        if (meta_attribute_list)
        {
            int j = 0;
            for (j = 0; j < AXIS2_ARRAY_LIST_SIZE(meta_attribute_list, env); j++)
            {
                axiom_attribute_t *attr = (axiom_attribute_t *)AXIS2_ARRAY_LIST_GET(meta_attribute_list, env, j);
                if (attr)
                {
                    AXIOM_ELEMENT_ADD_ATTRIBUTE(reference_ele, env, attr, reference_node);
                }
            }
        }
        
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(meta_data_list, env); i ++)
        {
            axiom_node_t *ref_node = (axiom_node_t *)AXIS2_ARRAY_LIST_GET(meta_data_list, env, i);
            if (ref_node)
            {
                AXIOM_NODE_ADD_CHILD(reference_node, env, ref_node);
            }
        }
    }
    
    extension_list = AXIS2_ENDPOINT_REF_GET_REF_EXTENSION_LIST(endpoint_ref, env);
    if (extension_list && AXIS2_ARRAY_LIST_SIZE(extension_list, env) > 0)
    {
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(extension_list, env); i ++)
        {
            axiom_node_t *ref_node = (axiom_node_t *)AXIS2_ARRAY_LIST_GET(extension_list, env, i);
            if (ref_node)
            {
                AXIOM_NODE_ADD_CHILD(header_block_node, env, ref_node);
            }
        }
    }

    if (AXIS2_STRCMP (AXIS2_WSA_NAMESPACE_SUBMISSION, addr_ns) == 0)
    {
       /* axis2_any_content_type_t *referece_properties = NULL;
        axiom_element_t *reference_ele = NULL;
        axiom_node_t *reference_node = NULL;
        referece_properties =
            AXIS2_ENDPOINT_REF_GET_REF_PROPERTIES (endpoint_ref, env);
        addr_ns_obj = axiom_namespace_create (env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
        reference_ele =
            axiom_element_create (env, header_node,
                                     EPR_REFERENCE_PROPERTIES, addr_ns_obj,
                                     &reference_node);
        axis2_addr_out_handler_process_any_content_type (env,
                                                         referece_properties,
                                                         reference_node,
                                                         addr_ns);
         if(NULL != reference_ele)
        {
            axiom_namespace_t *dec_ns = NULL;
            dec_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE( reference_ele , env,
                                                 addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
            if(! dec_ns)
            {
                 AXIOM_NAMESPACE_FREE(addr_ns_obj, env);
                 addr_ns_obj = NULL;
            }
        }
        */
       
    }
    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_addr_out_handler_add_to_header (const axis2_env_t *env,
                                      axis2_endpoint_ref_t * epr,
                                      axiom_node_t ** parent_node_p,
                                      const axis2_char_t * addr_ns)
{
    axiom_node_t *parent_node = NULL;
    const axis2_qname_t *interface_qname = NULL;
    axiom_node_t *interface_node = NULL;
    axiom_element_t *interface_ele = NULL;
    const axis2_char_t *element_localname = NULL;
    axis2_svc_name_t *service_name = NULL;
    axiom_namespace_t *addr_ns_obj = NULL;

    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, epr, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, parent_node_p, AXIS2_FAILURE);

    parent_node = *(parent_node_p);


    interface_qname = AXIS2_ENDPOINT_REF_GET_INTERFACE_NAME (epr, env);

    if (interface_qname)
    {
        axis2_char_t *text = NULL;
        axis2_char_t *qname_prefix = NULL;
        axis2_char_t *qname_localpart = NULL;
         
        addr_ns_obj = axiom_namespace_create (env, addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
        
        if (AXIS2_STRCMP (addr_ns, AXIS2_WSA_NAMESPACE_SUBMISSION) == 0)
        {
            element_localname = EPR_PORT_TYPE;
        }
        else
        {
            element_localname = AXIS2_WSA_INTERFACE_NAME;
        }

        interface_ele = axiom_element_create (env, parent_node,
                                                 element_localname,
                                                 addr_ns_obj,
                                                 &interface_node);
        qname_prefix = AXIS2_QNAME_GET_PREFIX (interface_qname, env);
        qname_localpart = AXIS2_QNAME_GET_LOCALPART (interface_qname, env);

        text =
            AXIS2_MALLOC (env->allocator,
                          sizeof (axis2_char_t) *
                          (AXIS2_STRLEN (qname_prefix) +
                           AXIS2_STRLEN (qname_localpart) + 2));
        sprintf (text, "%s:%s", qname_prefix, qname_localpart);
        AXIOM_ELEMENT_SET_TEXT (interface_ele, env, text, interface_node);
        AXIS2_FREE (env->allocator, text);
        if(NULL !=interface_ele )
        {
            axiom_namespace_t *dec_ns = NULL;
            dec_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE( interface_ele , env,
                                                 addr_ns, AXIS2_WSA_DEFAULT_PREFIX);
            if(! dec_ns)
            {
                 AXIOM_NAMESPACE_FREE(addr_ns_obj, env);
                 addr_ns_obj = NULL;
            }
        }

    }

    service_name = AXIS2_ENDPOINT_REF_GET_SVC_NAME (epr, env);
    return AXIS2_SUCCESS;
}


axis2_status_t
axis2_addr_out_handler_process_any_content_type (const axis2_env_t *env,
                                                 axis2_any_content_type_t *
                                                 reference_values,
                                                 axiom_node_t *
                                                 parent_ele_node,
                                                 const axis2_char_t * addr_ns)
{
    axis2_hash_t *value_ht = NULL;
    axis2_hash_index_t *hash_index = NULL;

    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);

    if (reference_values)
    {
        const void *k = NULL;
        void *v = NULL;
        axis2_ssize_t len = 0;
        value_ht =
            AXIS2_ANY_CONTENT_TYPE_GET_VALUE_MAP (reference_values, env);
        if (!value_ht)
            return AXIS2_FAILURE;

        for (hash_index = axis2_hash_first (value_ht, env); hash_index;
             hash_index = axis2_hash_next (env, hash_index))
        {
            axis2_hash_this (hash_index, &k, &len, &v);
            if (k)
            {
                axiom_node_t *node = NULL;
                axiom_element_t *ele = NULL;
                ele =
                    axiom_element_create (env, parent_ele_node, k, NULL,
                                             &node);
                if (ele)
                {
                    if (AXIS2_STRCMP (AXIS2_WSA_NAMESPACE, addr_ns) == 0)
                    {
                        axiom_namespace_t *addr_ns_obj = NULL;
                        axiom_attribute_t *att = NULL;
                        addr_ns_obj =
                            axiom_namespace_create (env, addr_ns,
                                                       AXIS2_WSA_DEFAULT_PREFIX);
                        att =
                            axiom_attribute_create (env,
                                                       AXIS2_WSA_IS_REFERENCE_PARAMETER_ATTRIBUTE,
                                                       AXIS2_WSA_TYPE_ATTRIBUTE_VALUE,
                                                       addr_ns_obj);
                    }
                    AXIOM_ELEMENT_SET_TEXT (ele, env, v, node);
                }
            }
        }
    }
    return AXIS2_SUCCESS;
}
