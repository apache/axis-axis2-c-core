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

#include "w2c_emitter_protected.h"

#include <woden_wsdl10_desc.h>
#include <woden_wsdl10_svc.h>
#include <woden_wsdl10_endpoint.h>
#include <woden_ext_element.h>
#include <woden_wsdl10_soap_module.h>
#include <woden_wsdl10_soap_address_exts.h>

#include <axutil_qname.h>

#include <woden_desc.h>
#include <woden_svc.h>
#include <woden_endpoint.h>
#include <woden_element_decl.h>
#include <woden_interface.h>
#include <woden_binding.h>
#include <woden_interface_element.h>
#include <woden_interface_op_element.h>
#include <woden_endpoint_element.h>
#include <woden_interface_msg_ref.h>
#include <woden_wsdl10_interface_msg_ref.h>
#include <woden_wsdl10_interface_msg_ref_element.h>
#include <woden_binding_element.h>
#include <woden_binding_op.h>
#include <woden_wsdl10_binding_op.h>
#include <woden_binding_op.h>
#include <woden_interface_op.h>
#include <woden_interface.h>
#include <woden_direction.h>
#include <woden_soap_binding_exts.h>
#include <woden_wsdl10_soap_binding_exts.h>
#include <woden_soap_binding_op_exts.h>
#include <woden_wsdl10_soap_binding_op_exts.h>
#include <woden_wsdl10_interface_msg_ref_element.h>
#include <woden_wsdl10_interface_msg_ref.h>
#include <woden_wsdl10_msg_ref.h>
#include <woden_wsdl10_msg_ref_element.h>
#include <woden_wsdl10_part.h>
#include <woden_element_decl.h>

#include <w2c_typemapper.h>
#include <w2c_cmdline_option_consts.h>
#include <w2c_xslt_utils.h>


axis2_status_t
w2c_emitter_load_services_wsdl1( w2c_emitter_impl_t *emitter_impl,
                            const axutil_env_t *env,
                            axiom_node_t *root);
axis2_status_t
w2c_emitter_load_services_wsdl2( w2c_emitter_impl_t* emitter_impl,
                            const axutil_env_t* env,
                            axiom_node_t* root);

axis2_status_t
w2c_emitter_load_operations( w2c_emitter_impl_t *emitter_impl,
                                    const axutil_env_t *env,
                                    axiom_node_t *method, 
                                    void *interface_op, int wsdl_version);
/***************************Function implementation****************************/

w2c_emitter_impl_t*
w2c_emitter_create_impl( const axutil_env_t *env)
{
    w2c_emitter_impl_t *emitter_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    emitter_impl = 
         AXIS2_MALLOC (env->allocator, sizeof(w2c_emitter_impl_t));

    if(! emitter_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    w2c_emitter_init(emitter_impl, env );
    
    return emitter_impl;
}

axis2_status_t
w2c_emitter_init( w2c_emitter_impl_t *emitter_impl,
                  const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(! emitter_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE); 
        return AXIS2_FAILURE;
    }
    emitter_impl-> config = NULL;
    emitter_impl-> loader = NULL;
    emitter_impl-> desc = NULL;
    emitter_impl-> name_maker_func = NULL;
    emitter_impl-> qname2name_maker = NULL;
    emitter_impl-> resolver = woden_resolver_create(env);
    axiom_element_create(env, NULL, "class", NULL, &(emitter_impl->common_node));

    emitter_impl->emitter.ops = 
           AXIS2_MALLOC (env->allocator, sizeof(w2c_emitter_ops_t));
    if(! emitter_impl->emitter.ops)
    {
        w2c_emitter_free(&(emitter_impl->emitter), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
  
    emitter_impl->emitter.ops->free = w2c_emitter_free;
    emitter_impl->emitter.ops->set_config = w2c_emitter_set_config;
    emitter_impl->emitter.ops->emit_stub = w2c_emitter_emit_stub;
    emitter_impl->emitter.ops->emit_skel = w2c_emitter_emit_skel;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_emitter_free(w2c_emitter_t *emitter,
       const axutil_env_t *env)
{
    w2c_emitter_impl_t *emitter_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    emitter_impl = W2C_INTF_TO_IMPL(emitter);

    if ( emitter_impl-> desc)
    {
        if (WODEN_RESOLVER_GET_SPEC( emitter_impl-> resolver, env ) 
                                                          == WODEN_WSDL10 )
        {
            WODEN_WSDL10_DESC_FREE ( emitter_impl-> desc, env);
        }
        else if (WODEN_RESOLVER_GET_SPEC( emitter_impl-> resolver, env)
                                                          == WODEN_WSDL20 )
        {
            WODEN_DESC_FREE ( emitter_impl-> desc, env);
        }
    }
    if ( emitter_impl-> resolver )
    {
        WODEN_RESOLVER_FREE( emitter_impl-> resolver, env);
    }
    if ( emitter_impl-> common_node )
    {
        AXIOM_NODE_FREE_TREE( emitter_impl-> common_node, env );
    }
    if(emitter->ops)
    {
        AXIS2_FREE(env->allocator, emitter->ops);
        emitter->ops = NULL;
    }
    if(emitter_impl)
    {
        AXIS2_FREE(env->allocator, emitter_impl);
        emitter_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_emitter_set_config(w2c_emitter_t *emitter,
       const axutil_env_t *env,
       w2c_engine_configuration_t *config,
       w2c_config_property_loader_t *loader)
{
    w2c_emitter_impl_t *emitter_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    W2C_EMITTER_NAME_MAKER name_maker_func = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    emitter_impl = W2C_INTF_TO_IMPL(emitter);
    emitter_impl-> config = config;
    emitter_impl-> loader = loader;

    name_maker_func =(W2C_EMITTER_NAME_MAKER)
         W2C_ENGINE_CONFIGURATION_GET_NAMEMAKER_FUNC( config, env);
    emitter_impl-> qname2name_maker = W2C_ENGINE_CONFIGURATION_GET_QNAME2NAME( config, env);
    
    if ( ! name_maker_func )
    {
        emitter_impl-> name_maker_func = w2c_emitter_default_namemaker;
    }
    else
    {
        emitter_impl-> name_maker_func = name_maker_func;
    }
    
    
    status = w2c_emitter_parse_wsdl( emitter_impl, env); 
    if ( AXIS2_FAILURE == status )
    {
        w2c_messages_print_n_log_error( env,
                       "emitter.errorParsingWSDL");
        return AXIS2_FAILURE;
    }
 
    status =  w2c_emitter_load_services ( emitter_impl, env,
                                     emitter_impl-> common_node );
    if ( AXIS2_FAILURE == status )
    {
        w2c_messages_print_n_log_error( env,
                       "emitter.errorLoadingServices");
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/* these 2 functions would be overriden */

axis2_status_t AXIS2_CALL
w2c_emitter_emit_stub(w2c_emitter_t *emitter,
       const axutil_env_t *env)
{
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_emitter_emit_skel(w2c_emitter_t *emitter,
       const axutil_env_t *env)
{
    return AXIS2_SUCCESS;
}

/* implementations for protected functions */

axis2_status_t
w2c_emitter_write_class(w2c_emitter_impl_t *emitter_impl,
                      const axutil_env_t *env,
                      axiom_node_t *root,
                      w2c_writer_t *writer)
{
    axis2_char_t *package_name = NULL;
    axis2_char_t *file_name = NULL;
    axiom_element_t *root_ele = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_char_t *full_path = NULL;

    if ( ! writer )
    {
       return AXIS2_FAILURE;
    }
    
    status = W2C_WRITER_INITIALIZE( writer, env, emitter_impl->loader, 
                                       emitter_impl->config );
    if ( AXIS2_FAILURE == status)
    {
        w2c_messages_print_n_log_error( env, 
                       "writer.templateLoadFailed");
        return AXIS2_FAILURE;
    }
    root_ele = AXIOM_NODE_GET_DATA_ELEMENT( root, env);
    
    package_name = axiom_element_get_attribute_value_by_name 
                            ( root_ele, env, "package");
    file_name = axiom_element_get_attribute_value_by_name 
                            ( root_ele, env, "name");
    full_path = W2C_WRITER_CREATE_OUT_FILE ( writer, env, package_name, file_name);
    if ( ! full_path )
    {
        return AXIS2_FAILURE;
    }
    W2C_ENGINE_CONFIGURATION_ADD_FILENAME( emitter_impl-> config, env,
                                       full_path);
    W2C_WRITER_PARSE ( writer, env, root );

    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_emitter_parse_wsdl( w2c_emitter_impl_t *emitter_impl,
                        const axutil_env_t *env)
{
    axiom_document_t *om_doc = NULL;
    axis2_char_t *doc_base_uri = NULL;
    /*axis2_char_t *axis2c_home = NULL;*/
    void *desc = NULL;
    axis2_char_t *filename = NULL;

  
    filename =
       W2C_ENGINE_CONFIGURATION_GET_WSDL_URI ( emitter_impl-> config, env);

    om_doc = get_root_element_from_filename(env, filename);
    if ( ! om_doc )
    {
        return AXIS2_FAILURE;
    }
    /*axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    doc_base_uri = axis2_stracat (axis2c_home, "/woden/", env);*/
    doc_base_uri = W2C_ENGINE_CONFIGURATION_GET_BASE_URI ( emitter_impl-> config, env);
    
    desc = (void *)WODEN_RESOLVER_READ(emitter_impl-> resolver , env, om_doc,
                                                                 doc_base_uri);
    /* freee existing desc ) */
    if ( emitter_impl-> desc )
    {
        if (WODEN_RESOLVER_GET_SPEC( emitter_impl-> resolver, env )
                                                            == WODEN_WSDL10 )
        {
            WODEN_WSDL10_DESC_FREE ( emitter_impl-> desc, env);
        }
        else if (WODEN_RESOLVER_GET_SPEC( emitter_impl-> resolver, env)
                                                            == WODEN_WSDL20 )
        {
            WODEN_DESC_FREE ( emitter_impl-> desc, env);
        }
    }
    emitter_impl-> desc = desc ;

    AXIS2_FREE ( env-> allocator, doc_base_uri);
    return AXIS2_SUCCESS;
}

axiom_document_t*
get_root_element_from_filename(
        const axutil_env_t *env,
        axis2_char_t *filename)
{
    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *doc   = NULL;

    reader = axiom_xml_reader_create_for_file(env, filename, NULL);
    if ( ! reader )
    {
       return NULL;
    }
    om_builder = axiom_stax_builder_create(env, reader);
    doc = axiom_document_create(env, NULL, om_builder);
    axiom_document_build_all(doc, env);

    return doc;
}

axutil_qname_t*
w2c_emitter_pick_service_name ( w2c_emitter_impl_t *emitter_impl,
                               const axutil_env_t *env)
{
    axutil_array_list_t *svc_list = NULL;
    void  *svc = NULL;
    axutil_qname_t *svc_qname = NULL;

    if (WODEN_RESOLVER_GET_SPEC( emitter_impl-> resolver,env ) 
                                                            == WODEN_WSDL10 )
    {
        svc_list = 
            WODEN_WSDL10_DESC_ELEMENT_GET_SVC_ELEMENTS(emitter_impl->desc, env);
        if (svc_list)
        {
            svc = axutil_array_list_get(svc_list, env, 0);
            if (svc)
            {
                svc_qname = WODEN_WSDL10_SVC_GET_QNAME(svc, env);
            }
        }
    }
    if (WODEN_RESOLVER_GET_SPEC( emitter_impl-> resolver, env ) 
                                                            == WODEN_WSDL20 )
    {
        svc_list = 
            WODEN_DESC_ELEMENT_GET_SVC_ELEMENTS(emitter_impl->desc, env);
        if (svc_list)
        {
            svc = axutil_array_list_get(svc_list, env, 0);
            if (svc)
            {
                svc_qname = WODEN_SVC_GET_QNAME(svc, env);
            }
        }
    }
    return svc_qname;
}

                   
axis2_status_t
w2c_emitter_load_services( w2c_emitter_impl_t *emitter_impl,
                            const axutil_env_t *env,
                            axiom_node_t *root)
{
    axis2_status_t status = AXIS2_FAILURE;

    if (WODEN_RESOLVER_GET_SPEC( emitter_impl-> resolver,env )
                                                            == WODEN_WSDL10 )
    {
        status = w2c_emitter_load_services_wsdl1( emitter_impl, env, root);
    }
    else if (WODEN_RESOLVER_GET_SPEC( emitter_impl-> resolver, env )
                                                            == WODEN_WSDL20 )
    {
        status = w2c_emitter_load_services_wsdl2( emitter_impl, env, root);
    }
    
    return status;
}
                   
axis2_status_t
w2c_emitter_load_services_wsdl1( w2c_emitter_impl_t *emitter_impl,
                            const axutil_env_t *env,
                            axiom_node_t *root)
{
    axutil_array_list_t *svc_list = NULL;
    woden_wsdl10_svc_t *svc = NULL; 
    axutil_qname_t* svc_qname = NULL;

    axutil_array_list_t *endpoint_list = NULL;
    woden_wsdl10_endpoint_t *endpoint = NULL;
    axis2_char_t *address = NULL;
    axutil_qname_t *ext_type_l = NULL;
    axutil_qname_t *ext_type = NULL;
    int size = 0;
    axutil_array_list_t *ext_elements = NULL;
    void *soap_address = NULL;
    axutil_uri_t *soap_address_uri = NULL;
    axiom_node_t *endpoint_node = NULL;
    void *ext_element = NULL;
    
    woden_binding_t *binding = NULL;
    axis2_char_t *binding_style = NULL;
    
    int j = 0;
    axutil_array_list_t *binding_ops_list = NULL;
    int binding_ops_count = 0;
    void *binding_op = NULL;
   
    int i = 0;
    void* interface_op = NULL;
    axutil_qname_t *op_qname = NULL;
    axis2_char_t *local_part = NULL;
    axis2_char_t *ns = NULL;
    axiom_node_t *method = NULL;
    
    void *soap_binding_op = NULL;
    axutil_uri_t *soap_action_uri = NULL;
    axis2_char_t *soap_action_str = NULL;
   
    axis2_char_t *given_name = NULL;
    axis2_char_t *qname_str = NULL;

    /**********************************************************************
     * extract service(s)
     **********************************************************************/
    svc_list = WODEN_WSDL10_DESC_GET_SVCS(emitter_impl-> desc, env);
    
    /** currently work only for one service */
    if (axutil_array_list_size ( svc_list, env ) < 1 )
    {
       /** no need to continue more */
       return AXIS2_FAILURE; 
    }
    svc = axutil_array_list_get( svc_list, env, 0 );
    svc_qname = WODEN_WSDL10_SVC_GET_QNAME( svc, env);
    ns = axutil_qname_get_uri( svc_qname, env);
    w2c_xslt_utils_add_attribute (env, root, "namespace", ns);

    /**********************************************************************
     * extract endpoint(s)
     **********************************************************************/
    endpoint_list = WODEN_WSDL10_SVC_GET_ENDPOINTS (svc, env);
    
    /** currently work only for one endpoint*/
    if (axutil_array_list_size ( endpoint_list, env ) < 1 )
    {
       return AXIS2_FAILURE; /* no need to continue more */
    }

    endpoint = axutil_array_list_get( endpoint_list, env, 0 );
    endpoint = woden_wsdl10_endpoint_to_element_extensible(
                            endpoint, env);

    ext_type_l = axutil_qname_create(env, "address",
            "http://schemas.xmlsoap.org/wsdl/soap/", NULL);
    ext_elements = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(endpoint,
            env);
    if(ext_elements)
        size = axutil_array_list_size(ext_elements, env);
    else
        return AXIS2_FAILURE;
    for(j = 0; j < size; j++)
    {
        ext_element = axutil_array_list_get(ext_elements, env, j);
        ext_type = (axutil_qname_t*)WODEN_EXT_ELEMENT_GET_EXT_TYPE(ext_element, env);
        if(AXIS2_TRUE == axutil_qname_equals(ext_type, env, ext_type_l))
        {
            ext_element =
               woden_wsdl10_soap_module_to_soap_module_element (
                        ext_element, env);
            soap_address =
                WODEN_WSDL10_SOAP_MODULE_ELEMENT_GET_SOAP_ADDRESS_EXTS(
                        ext_element, env);

            soap_address_uri = WODEN_WSDL10_SOAP_ADDRESS_EXTS_GET_SOAP_ADDRESS(
                soap_address, env);
            break;
        }
    }

    if(soap_address_uri)
    {
        address = axutil_uri_to_string(soap_address_uri, env,
                AXIS2_URI_UNP_OMITUSERINFO);
        endpoint_node = w2c_xslt_utils_add_child_node(env, "endpoint",
                                                        root);
        /** here the endpoint adding */
        w2c_xslt_utils_add_text ( env, endpoint_node, address);
    }

    /**********************************************************************
     * extract bindings(s)
     **********************************************************************/
    binding = 
          WODEN_WSDL10_ENDPOINT_GET_BINDING( endpoint, env);
    binding_style = ""; /* currently hard coded */

    /**********************************************************************
     * extract binding ops
     **********************************************************************/
    binding_ops_list = 
         WODEN_BINDING_GET_BINDING_OPS( binding, env);
    binding_ops_count = 
         axutil_array_list_size( binding_ops_list, env );

    for ( i = 0; i < binding_ops_count; i ++ )
    {
        binding_op = axutil_array_list_get ( binding_ops_list, env, i );
        /**********************************************************************
         * extract interface ops
         **********************************************************************/
        interface_op = WODEN_WSDL10_BINDING_OP_GET_INTERFACE_OP
                                                          (binding_op, env);
        
        op_qname = WODEN_INTERFACE_OP_GET_QNAME( interface_op, env);
        if ( op_qname )
        {
            method = w2c_xslt_utils_add_child_node(env, "method",
                                               root);
            local_part= axutil_qname_get_localpart(op_qname, env);
            given_name = W2C_QNAME2NAME_MAKER_SUGGEST_NAME( 
                    emitter_impl-> qname2name_maker, env, op_qname);
            qname_str = axutil_qname_to_string(op_qname, env);
            w2c_xslt_utils_add_attribute (env, method, "localpart",
                                                local_part);
            given_name = emitter_impl-> name_maker_func(given_name, env);
            w2c_xslt_utils_add_attribute(env, method, "name",
                                                given_name);
            w2c_xslt_utils_add_attribute(env, method, "qname",
                                                qname_str); 
            AXIS2_FREE( env-> allocator, local_part);
            ns = axutil_qname_get_uri(op_qname, env);
            w2c_xslt_utils_add_attribute (env, method, "namespace",
                                                ns);
            /** todos */
            w2c_xslt_utils_add_attribute (env, method, "style",
                                                             binding_style);
        }
        else
        {
            return AXIS2_FAILURE;
        }
        /**********************************************************************
         * extract interface message refs
         **********************************************************************/
        w2c_emitter_load_operations( emitter_impl, env, method, interface_op, 1);

        /** get soap action */
        ext_type_l = axutil_qname_create(env, "operation",
                                "http://schemas.xmlsoap.org/wsdl/soap/", NULL);
        binding_op = woden_wsdl10_binding_op_to_element_extensible(binding_op, env);
        ext_elements = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(binding_op, env);
        size = axutil_array_list_size( ext_elements, env);
        for(j=0; j < size; j++)
        {
            ext_element = axutil_array_list_get(ext_elements, env, j);
            ext_type = WODEN_EXT_ELEMENT_GET_EXT_TYPE(ext_element, env);
            if(AXIS2_TRUE == axutil_qname_equals(ext_type, env, ext_type_l))
            {
                ext_element = woden_wsdl10_soap_module_to_soap_module_element (
                                                               ext_element, env);
                soap_binding_op =
                      WODEN_WSDL10_SOAP_MODULE_ELEMENT_GET_SOAP_BINDING_OP_EXTS(
                                                                 ext_element, env);
                soap_action_uri = 
                        WODEN_WSDL10_SOAP_BINDING_OP_EXTS_GET_SOAP_ACTION
                                                           (soap_binding_op, env);
                soap_action_str = axutil_uri_to_string 
                                  ( soap_action_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
                w2c_xslt_utils_add_attribute (env, method, "soapaction", soap_action_str);
            }
        }
    }
    
    return AXIS2_SUCCESS;
        
}

axis2_status_t
w2c_emitter_load_operations( w2c_emitter_impl_t *emitter_impl,
                                    const axutil_env_t *env,
                                    axiom_node_t *method, 
                                    void *interface_op, int wsdl_version)
{
    axutil_array_list_t *interface_msg_refs = NULL;
    int msg_refs_size = 0;
    int j = 0;
    void *interface_msg_ref = NULL;
    void *direction = NULL;
    axis2_char_t *str_direction = NULL;
    axis2_char_t *in_out = NULL;
    axis2_bool_t in = AXIS2_FALSE;
    axis2_bool_t out = AXIS2_FALSE;

    axutil_qname_t *msg_qname = NULL;
    void* if_msg_ref_ele = NULL;
    void* msg_ref = NULL;
    woden_wsdl10_part_t *part = NULL;
    void *msg_ref_ele = NULL;
    /*void *decl = NULL;*/
    void *part_ele = NULL;
  
    axiom_node_t* input_param = NULL;
    axiom_node_t* output_param = NULL;

    output_param = w2c_xslt_utils_add_child_node(env, "param",
                                          output_param);
    /*w2c_xslt_utils_add_attribute (env, output_param, "type",
                                                        "axiom_node_t*");
    w2c_xslt_utils_add_attribute (env, output_param, "name",
                                                        "param1"); */
  
    interface_msg_refs = WODEN_INTERFACE_OP_GET_INTERFACE_MSG_REFS(
            interface_op, env);
    if(interface_msg_refs)
        msg_refs_size = axutil_array_list_size(interface_msg_refs, env);
    for(j = 0; j < msg_refs_size; j++)
    {
        interface_msg_ref = axutil_array_list_get(interface_msg_refs, env, j);
        if( wsdl_version == 1)
        {
            direction =  WODEN_WSDL10_INTERFACE_MSG_REF_GET_DIRECTION
                                    (interface_msg_ref, env);
            if_msg_ref_ele= (woden_wsdl10_interface_msg_ref_element_t*)
                woden_wsdl10_interface_msg_ref_to_interface_msg_ref_element( interface_msg_ref, env);
            
            msg_ref = WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_GET_MSG( if_msg_ref_ele, env);
            msg_ref_ele = woden_wsdl10_msg_ref_to_msg_ref_element( msg_ref, env);
            part = WODEN_WSDL10_MSG_REF_ELEMENT_GET_PART( msg_ref_ele, env);
            part_ele = woden_wsdl10_part_to_part_element( part, env);
            msg_qname = WODEN_WSDL10_PART_ELEMENT_GET_ELEMENT_QNAME( part_ele, env);
            /*decl = WODEN_WSDL10_PART_GET_ELEMENT_DECLARATION( part, env);
            msg_qname = WODEN_ELEMENT_DECL_GET_QNAME( decl, env);*/
            /*msg_qname = WODEN_WSDL10_PART_GET_QNAME( part, env);*/
           /* msg_qname = WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_GET_MSG_QNAME( if_msg_ref_ele, env);*/
        }
        else if ( wsdl_version == 2 )
        {
            direction =  WODEN_INTERFACE_MSG_REF_GET_DIRECTION
                                    (interface_msg_ref, env);
            if_msg_ref_ele= (woden_interface_msg_ref_element_t*)
                woden_interface_msg_ref_to_interface_msg_ref_element( interface_msg_ref, env);
            /*msg_qname = WODEN_INTERFACE_MSG_REF_ELEMENT_GET_MSG_LABEL( msg_ref_ele, env);*/
            msg_qname = WODEN_INTERFACE_MSG_REF_ELEMENT_GET_ELEMENT_QNAME( if_msg_ref_ele, env);
        }
        str_direction = WODEN_DIRECTION_TO_STRING(direction, env);
        if(0 == axis2_strcmp(str_direction, "in"))
        {
            if (!in) /* for the first in iteration */
            {
                input_param = w2c_xslt_utils_add_child_node(env, "input", method);
                in = AXIS2_TRUE;
            }
            w2c_emitter_add_param( emitter_impl, env, input_param, msg_qname);
        }
        if(0 == axis2_strcmp(str_direction, "out"))
        {
            if (!out)
            {
                output_param = w2c_xslt_utils_add_child_node(env, "output",
                                                                  method);
                out = AXIS2_TRUE;
            }
            w2c_emitter_add_param( emitter_impl, env, output_param, msg_qname);
        }
    }
    /** mep should be direcly called from woden */
    if ( in && out )
    {
         in_out = "12";
    }
    if ( in && !out)
    {
         in_out = "11";
    }

    w2c_xslt_utils_add_attribute (env, method, "mep", in_out);
 
    return AXIS2_SUCCESS;
}

axis2_status_t
w2c_emitter_load_services_wsdl2( w2c_emitter_impl_t* emitter_impl,
                            const axutil_env_t* env,
                            axiom_node_t* root)
{
    axutil_array_list_t* svc_list = NULL;
    woden_svc_t* svc = NULL; 

    axutil_array_list_t* endpoint_list = NULL;
    woden_endpoint_t * endpoint = NULL;
    axutil_uri_t* endpoint_uri = NULL;
    axis2_char_t* endpoint_uri_str = NULL;
    axiom_node_t* endpoint_node = NULL;
    
    woden_binding_t* binding = NULL;
    axis2_char_t* binding_style = NULL;
    
    axutil_array_list_t* binding_ops_list = NULL;
    int binding_ops_count = 0;
    void* binding_op = NULL;
   
    void* interface_op = NULL;
    int i = 0;
    axutil_qname_t* op_qname = NULL;
    axis2_char_t* local_part = NULL;
    axis2_char_t* ns = NULL;
    axiom_node_t* method = NULL;
  
    /*void *soap_binding_op = NULL;*/
    axutil_uri_t *soap_action_uri = NULL;
    axis2_char_t *soap_action_str = NULL;
    axutil_qname_t *ext_type_l = NULL;
    axutil_qname_t *ext_type = NULL;
    axutil_array_list_t *ext_elements = NULL;
    void *ext_element = NULL;
    int size = 0, j = 0;

    axis2_char_t *given_name = NULL;
    axis2_char_t *qname_str = NULL;

    /**********************************************************************
     * extract service(s)
     **********************************************************************/
    svc_list = WODEN_DESC_GET_SVCS(emitter_impl->desc, env);

    /** currently work only for one service */
    if (axutil_array_list_size ( svc_list, env ) < 1 )
    {
       /** no need to continue more */
       return AXIS2_FAILURE; 
    }
    svc = axutil_array_list_get( svc_list, env, 0 );

    /**********************************************************************
     * extract endpoint(s)
     **********************************************************************/
    endpoint_list = WODEN_SVC_GET_ENDPOINTS (svc, env);
    
    /** currently work only for one endpoint*/
    if (axutil_array_list_size ( endpoint_list, env ) < 1 )
    {
       return AXIS2_FAILURE; /* no need to continue more */
    }
    endpoint = axutil_array_list_get( endpoint_list, env, 0 );
    endpoint_uri = WODEN_ENDPOINT_GET_ADDRESS( endpoint, env);
    endpoint_uri_str = axutil_uri_to_string ( endpoint_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
    endpoint_node = w2c_xslt_utils_add_child_node(env, "endpoint",
                                                    root);
    /** here the endpoint adding */
    w2c_xslt_utils_add_text ( env, endpoint_node, endpoint_uri_str );

    /**********************************************************************
     * extract bindings(s)
     **********************************************************************/
    binding = 
          WODEN_ENDPOINT_GET_BINDING( endpoint, env);
    binding_style = ""; /* currently hard coded */

    /**********************************************************************
     * extract binding ops
     **********************************************************************/
    binding_ops_list = 
         WODEN_BINDING_GET_BINDING_OPS( binding, env);
    binding_ops_count = 
         axutil_array_list_size( binding_ops_list, env );

    for ( i = 0; i < binding_ops_count; i ++ )
    {
        binding_op = axutil_array_list_get ( binding_ops_list, env, i );
        /**********************************************************************
         * extract interface ops
         **********************************************************************/

        interface_op = WODEN_BINDING_OP_GET_INTERFACE_OP
                                               (binding_op, env);
        op_qname = WODEN_INTERFACE_OP_GET_QNAME( interface_op, env);
        if ( op_qname )
        {
            method = w2c_xslt_utils_add_child_node(env, "method",
                                               root);
            local_part= axutil_qname_get_localpart(op_qname, env);
            given_name = W2C_QNAME2NAME_MAKER_SUGGEST_NAME( 
                    emitter_impl-> qname2name_maker, env, op_qname);
            qname_str = axutil_qname_to_string(op_qname, env);
            w2c_xslt_utils_add_attribute (env, method, "localpart",
                                                local_part);
            given_name = emitter_impl-> name_maker_func(given_name, env);
            w2c_xslt_utils_add_attribute (env, method, "name",
                                                given_name);
            w2c_xslt_utils_add_attribute (env, method, "qname",
                                                qname_str);
            ns = axutil_qname_get_uri(op_qname, env);
            w2c_xslt_utils_add_attribute (env, method, "namespace",
                                                ns);
            /** todos */
            w2c_xslt_utils_add_attribute (env, method, "style",
                                                             binding_style);
        }
        else
        {
            return AXIS2_FAILURE;
        }
        /**********************************************************************
         * extract interface message refs
         **********************************************************************/
        w2c_emitter_load_operations( emitter_impl, env, method, interface_op, 2);


        /** get soap action */
        ext_type_l = axutil_qname_create(env, "operation",
                                "http://schemas.xmlsoap.org/wsdl/soap/", NULL);
        binding_op = woden_binding_op_to_element_extensible(binding_op, env);
        ext_elements = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(binding_op, env);
        size = axutil_array_list_size( ext_elements, env);
        for(j=0; j < size; j++)
        {
            ext_element = axutil_array_list_get(ext_elements, env, j);
            ext_type = WODEN_EXT_ELEMENT_GET_EXT_TYPE(ext_element, env);
            if(AXIS2_TRUE == axutil_qname_equals(ext_type, env, ext_type_l))
            {
                /*ext_element = woden_soap_module_to_soap_module_element (
                                                               ext_element, env);
                soap_binding_op =
                      WODEN_SOAP_MODULE_ELEMENT_GET_SOAP_BINDING_OP_EXTS(
                                                                 ext_element, env);
                soap_action_uri = 
                        WODEN_SOAP_BINDING_OP_EXTS_GET_SOAP_ACTION
                                                           (soap_binding_op, env); */
                soap_action_str = axutil_uri_to_string 
                                  ( soap_action_uri, env, AXIS2_URI_UNP_OMITUSERINFO);
                w2c_xslt_utils_add_attribute (env, method, "soapaction", soap_action_str);
            }
        }
    }
 
   
    return AXIS2_SUCCESS;
        
}
axis2_status_t
w2c_emitter_add_param( w2c_emitter_impl_t *emitter_impl,
                            const axutil_env_t *env,
                            axiom_node_t *param_direction,
                            axutil_qname_t *msg_qname)
{
    axiom_node_t *param = NULL;
    axis2_char_t *type = NULL;
    axis2_char_t *name = NULL;
    w2c_typemapper_t *typemapper = NULL;
    axis2_bool_t is_primitive = AXIS2_FALSE;

    param = w2c_xslt_utils_add_child_node(env, "param",
                                          param_direction);
    typemapper = W2C_ENGINE_CONFIGURATION_GET_TYPEMAPPER( 
                            emitter_impl-> config, env);
    if( msg_qname != NULL)
    {
        type = W2C_TYPEMAPPER_GET_TYPE_NAME( typemapper, env, msg_qname);
    }
    if ( ! type ) /* this is databinding not present for the message*/
    {
        msg_qname = W2C_TYPEMAPPER_GET_DEFAULT_QNAME( typemapper, env);
        type = W2C_TYPEMAPPER_GET_TYPE_NAME( typemapper, env, msg_qname);
        is_primitive = W2C_TYPEMAPPER_IS_PRIMITIVE( typemapper, env, msg_qname);
    }
    if ( !is_primitive)
    {
        w2c_xslt_utils_add_attribute (env, param, "ours", "yes");
    }
    type = axis2_strdup(env, type);
    w2c_xslt_utils_add_attribute (env, param, "type", type);
    type = axutil_string_toupper( type);
    w2c_xslt_utils_add_attribute (env, param, "caps-type", type);

    name = W2C_TYPEMAPPER_GET_PARAMETER_NAME( typemapper, env, msg_qname);
    w2c_xslt_utils_add_attribute (env, param, "name", name);

    return AXIS2_SUCCESS;
}

/** do nothing just echo the value*/
axis2_char_t* w2c_emitter_default_namemaker( axis2_char_t *name, 
                             const axutil_env_t *env)
{
    return axis2_strdup(env, name);
}

axis2_char_t* w2c_emitter_default_qname2name( axutil_qname_t *qname,
                             const axutil_env_t *env)
{
    return axutil_qname_get_localpart( qname, env);
}
