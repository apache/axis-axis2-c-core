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
#include <w2c_writer.h>
#include <w2c_xslt_utils.h>
#include <w2c_typemapper.h>
#include <w2c_file_writer.h>

#define W2C_C_EMITTER_STUB_PREFIX "axis2_stub_"
#define W2C_C_EMITTER_STUB_SUFFIX ""
#define W2C_C_EMITTER_SKEL_PREFIX "axis2_skel_"
#define W2C_C_EMITTER_SKEL_SUFFIX ""
#define W2C_C_EMITTER_SVC_SKEL_PREFIX "axis2_svc_skel_"
#define W2C_C_EMITTER_SVC_SKEL_SUFFIX ""
#define W2C_C_EMITTER_TYPE_PREFIX "axis2_"
#define W2C_C_EMITTER_TYPE_SUFFIX "_t*"

/**********************fucntion prototypes ***********************************/
axis2_status_t
w2c_c_emitter_write_skel_header( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env);

axis2_status_t
w2c_c_emitter_write_stub_header( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env);
axis2_status_t AXIS2_CALL
w2c_c_emitter_set_config(w2c_emitter_t *emitter,
       const axutil_env_t *env,
       w2c_engine_configuration_t *config,
       w2c_config_property_loader_t *loader);

axis2_status_t AXIS2_CALL
w2c_c_emitter_emit_stub( w2c_emitter_t *emitter,
                         const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_c_emitter_emit_skel( w2c_emitter_t *emitter,
                         const axutil_env_t *env);
axis2_status_t
w2c_c_emitter_write_stub_source( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env);
axis2_status_t
w2c_c_emitter_write_stub_source( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env);
axis2_status_t
w2c_c_emitter_write_skel_source( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env);
axis2_status_t
w2c_c_emitter_write_skel_source( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env);
axis2_status_t
w2c_c_emitter_write_svc_skel( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env);
axis2_status_t
w2c_c_emitter_write_svc_xml( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env);

axiom_node_t*
w2c_c_emitter_create_for_stub (w2c_emitter_impl_t *emitter_impl,
                                   const axutil_env_t *env );
axiom_node_t*
w2c_c_emitter_create_for_skel(w2c_emitter_impl_t *emitter_impl,
                                   const axutil_env_t *env );
axiom_node_t*
w2c_c_emitter_create_for_svc_skel(w2c_emitter_impl_t *emitter_impl,
                                   const axutil_env_t *env );
axiom_node_t*
w2c_c_emitter_create_for_svc_xml(w2c_emitter_impl_t *emitter_impl,
                                   const axutil_env_t *env );

/******************standard create and delete for DLL ************************/
AXIS2_EXPORT int
axis2_get_instance(w2c_emitter_t **inst,
                   const axutil_env_t *env)
{
    w2c_emitter_impl_t *emitter_impl = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    emitter_impl = w2c_emitter_create_impl (env);
    /* this is done through an extension */
    /*emitter_impl-> name_maker_func = w2c_string_make_c_simple_name;*/
    if(!emitter_impl)
    {
        return AXIS2_FAILURE;
    }

    emitter_impl->emitter.ops->emit_stub = w2c_c_emitter_emit_stub;
    emitter_impl->emitter.ops->emit_skel = w2c_c_emitter_emit_skel;
    emitter_impl->emitter.ops->set_config = w2c_c_emitter_set_config;

    *inst = &(emitter_impl->emitter);

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(w2c_emitter_t *inst,
                      const axutil_env_t *env)
{
   axis2_status_t status = AXIS2_FAILURE;

   AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   if (inst)
   {
        status = W2C_EMITTER_FREE(inst, env);
    }
    return status;
}

/*************************overriden functions ********************************/


axis2_status_t AXIS2_CALL
w2c_c_emitter_set_config(w2c_emitter_t *emitter,
       const axutil_env_t *env,
       w2c_engine_configuration_t *config,
       w2c_config_property_loader_t *loader)
{
    w2c_typemapper_t *typemapper = NULL;
    axis2_hash_t *types = NULL;
    axis2_char_t *key = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *filename = NULL;
    axis2_char_t *classname = NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_char_t *source = NULL;
    axis2_char_t *header = NULL;
    axis2_char_t *root = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   
    typemapper = W2C_ENGINE_CONFIGURATION_GET_TYPEMAPPER( config, env);
    types = W2C_TYPEMAPPER_GET_ALL( typemapper, env);

    for (hi = axis2_hash_first( types, env);
                         hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, (void*)&key, NULL, (void*)&classname);
        qname = axis2_qname_create_from_string( env, key);
        if ( !W2C_TYPEMAPPER_IS_PRIMITIVE( typemapper, env, qname) )
        {
            filename = axis2_stracat(env, W2C_C_EMITTER_TYPE_PREFIX, classname);
            root = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LOCATION( config, env);
            source = w2c_file_writer_create_classfile_name( env,
                                root, 
                                "",
                                filename,
                                ".c");

            W2C_ENGINE_CONFIGURATION_ADD_FILENAME( config, env, source);
            header = w2c_file_writer_create_classfile_name( env,
                                root, 
                                "",
                                filename,
                                ".h");

            W2C_ENGINE_CONFIGURATION_ADD_FILENAME( config, env, header);
            AXIS2_FREE( env-> allocator, filename);
        }
    }

    return w2c_emitter_set_config( emitter, env, config, loader);
}
    
axis2_status_t AXIS2_CALL
w2c_c_emitter_emit_stub( w2c_emitter_t *emitter,
                         const axutil_env_t *env)
{
    w2c_emitter_impl_t *emitter_impl = NULL;
    axis2_status_t status;
    axis2_status_t overall_status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    emitter_impl = W2C_INTF_TO_IMPL(emitter);
     
    status = w2c_c_emitter_write_stub_source( emitter_impl, env);
    if ( AXIS2_FAILURE == status )
    {
        w2c_messages_print_n_log_error( env,
                       "writer.stubSourceFailed");
        overall_status = AXIS2_FAILURE;
    }
    status = w2c_c_emitter_write_stub_header( emitter_impl, env);
    if ( AXIS2_FAILURE == status )
    {
        w2c_messages_print_n_log_error( env,
                       "writer.stubHeaderFailed");
        overall_status = AXIS2_FAILURE;
    }

    return overall_status;
}

axis2_status_t AXIS2_CALL
w2c_c_emitter_emit_skel( w2c_emitter_t *emitter,
                         const axutil_env_t *env)
{
    w2c_emitter_impl_t *emitter_impl = NULL;
    axis2_status_t status;
    axis2_status_t overall_status = AXIS2_SUCCESS;
    axis2_bool_t svc_desc = AXIS2_FALSE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    emitter_impl = W2C_INTF_TO_IMPL(emitter);
   
    status = w2c_c_emitter_write_skel_source( emitter_impl, env);
    if ( AXIS2_FAILURE == status )
    {
        w2c_messages_print_n_log_error( env,
                       "writer.skelSourceFailed");
        overall_status = AXIS2_FAILURE;
    }
    status = w2c_c_emitter_write_skel_header( emitter_impl, env);
    if ( AXIS2_FAILURE == status )
    {
        w2c_messages_print_n_log_error( env,
                       "writer.skelHeaderFailed");
        overall_status = AXIS2_FAILURE;
    }
    status = w2c_c_emitter_write_svc_skel( emitter_impl, env);
    if ( AXIS2_FAILURE == status )
    {
        w2c_messages_print_n_log_error( env,
                       "writer.svcSkelFailed");
        overall_status = AXIS2_FAILURE;
    }
    svc_desc = W2C_ENGINE_CONFIGURATION_GET_GENERATE_DEPLOYEMENT_DESCRIPTOR
                               ( emitter_impl-> config, env);
    if (svc_desc )
    {
        status = w2c_c_emitter_write_svc_xml( emitter_impl, env);
        if ( AXIS2_FAILURE == status )
        {
            w2c_messages_print_n_log_error( env,
                           "writer.svcXmlFailed");
            overall_status = AXIS2_FAILURE;
        }
    }
    return overall_status;
}

axis2_status_t
w2c_c_emitter_write_stub_source( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env)
{
    axiom_node_t *root = NULL;
    w2c_writer_t *writer = NULL;
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_status_t status;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    root = w2c_c_emitter_create_for_stub (emitter_impl, env );
    if (! root )
    {
        return AXIS2_FAILURE;
    }
    writer = (w2c_writer_t*)w2c_class_loader_get_object_from_class_name
           ( env, "w2c_cstub_source_writer", W2C_EMITTER_WRITER_DIR, &dll_desc);
    status = w2c_emitter_write_class ( emitter_impl, env, root, writer );
    w2c_class_loader_free_loaded_class(env, dll_desc);
    AXIOM_NODE_FREE_TREE(root, env);
    return status;
}

axis2_status_t
w2c_c_emitter_write_stub_header( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env)
{
    axiom_node_t *root = NULL;
    w2c_writer_t *writer = NULL;
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_status_t status;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    root = w2c_c_emitter_create_for_stub ( emitter_impl, env );
    if (! root )
    {
        return AXIS2_FAILURE;
    }
    writer = (w2c_writer_t*)w2c_class_loader_get_object_from_class_name
         ( env, "w2c_cstub_header_writer", W2C_EMITTER_WRITER_DIR, &dll_desc);
    status = w2c_emitter_write_class ( emitter_impl, env, root, writer );
    w2c_class_loader_free_loaded_class(env, dll_desc);
    AXIOM_NODE_FREE_TREE(root, env);
    return status;
}

axis2_status_t
w2c_c_emitter_write_skel_source( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env)
{
    axiom_node_t *root = NULL;
    w2c_writer_t *writer = NULL;
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_status_t status;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    root = w2c_c_emitter_create_for_skel( emitter_impl, env );
    if (! root )
    {
        return AXIS2_FAILURE;
    }
    writer = (w2c_writer_t*)w2c_class_loader_get_object_from_class_name
           ( env, "w2c_cskel_source_writer", W2C_EMITTER_WRITER_DIR, &dll_desc);
    status = w2c_emitter_write_class ( emitter_impl, env, root, writer );
    w2c_class_loader_free_loaded_class(env, dll_desc);
    AXIOM_NODE_FREE_TREE(root, env);
    return status;
}

axis2_status_t
w2c_c_emitter_write_skel_header( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env)
{
    axiom_node_t *root = NULL;
    w2c_writer_t *writer = NULL;
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_status_t status;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    root = w2c_c_emitter_create_for_skel( emitter_impl, env );
    if (! root )
    {
        return AXIS2_FAILURE;
    }
    writer = (w2c_writer_t*)w2c_class_loader_get_object_from_class_name
           ( env, "w2c_cskel_header_writer", W2C_EMITTER_WRITER_DIR, &dll_desc);
    status = w2c_emitter_write_class ( emitter_impl, env, root, writer );
    w2c_class_loader_free_loaded_class(env, dll_desc);
    AXIOM_NODE_FREE_TREE(root, env);
    return status;
} 

axis2_status_t
w2c_c_emitter_write_svc_skel( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env)
{
    axiom_node_t *root = NULL;
    w2c_writer_t *writer = NULL;
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_status_t status;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    root = w2c_c_emitter_create_for_svc_skel( emitter_impl, env );
    if (! root )
    {
        return AXIS2_FAILURE;
    }
    writer = (w2c_writer_t*)w2c_class_loader_get_object_from_class_name
           ( env, "w2c_csvc_skeleton_writer", W2C_EMITTER_WRITER_DIR, &dll_desc);
    status = w2c_emitter_write_class ( emitter_impl, env, root, writer );
    w2c_class_loader_free_loaded_class(env, dll_desc);
    AXIOM_NODE_FREE_TREE(root, env);
    return status;
} 

axis2_status_t
w2c_c_emitter_write_svc_xml( w2c_emitter_impl_t *emitter_impl,
                                        const axutil_env_t *env)
{
    axiom_node_t *root = NULL;
    w2c_writer_t *writer = NULL;
    axutil_dll_desc_t *dll_desc = NULL;
    axis2_status_t status;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    root = w2c_c_emitter_create_for_svc_xml( emitter_impl, env );
    if (! root )
    {
        return AXIS2_FAILURE;
    }
    writer = (w2c_writer_t*)w2c_class_loader_get_object_from_class_name
           ( env, "w2c_cservice_xml_writer", W2C_EMITTER_WRITER_DIR, &dll_desc);
    status = w2c_emitter_write_class ( emitter_impl, env, root, writer );
    w2c_class_loader_free_loaded_class(env, dll_desc);
    AXIOM_NODE_FREE_TREE(root, env);
    return status;
}

axiom_node_t*
w2c_c_emitter_create_for_stub (w2c_emitter_impl_t *emitter_impl,
                                   const axutil_env_t *env )
{
    axiom_node_t *root = NULL;
    axis2_bool_t flag = AXIS2_FALSE;
    axis2_char_t *full_name = NULL;
    axis2_char_t *full_name_c = NULL;
    axis2_char_t *given_name = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;
    
    qname = w2c_emitter_pick_service_name( emitter_impl, env );
    given_name = W2C_QNAME2NAME_MAKER_SUGGEST_NAME( 
            emitter_impl-> qname2name_maker, env, qname);
    qname_str = axis2_qname_to_string(qname, env);
    
    root = w2c_xslt_utils_add_child_node(env, "class", NULL);
    full_name = axis2_strdup (env, W2C_C_EMITTER_STUB_PREFIX);
    full_name = w2c_string_add_string ( full_name, given_name, env );
    full_name = w2c_string_add_string 
                            ( full_name, W2C_C_EMITTER_STUB_SUFFIX, env);
    full_name_c = emitter_impl-> name_maker_func( full_name, env);

    w2c_xslt_utils_add_attribute (env, root, "name", full_name_c);
    w2c_xslt_utils_add_attribute (env, root, "prefix", full_name_c);
    w2c_xslt_utils_add_attribute (env, root, "servicename", given_name);
    w2c_xslt_utils_add_attribute (env, root, "package", "");
    w2c_xslt_utils_add_attribute (env, root, "interfaceName", given_name);
    w2c_xslt_utils_add_attribute (env, root, "qname" , qname_str);
    

    flag = 
      W2C_ENGINE_CONFIGURATION_GET_PACK_CLASSES( emitter_impl-> config, env);
    if ( flag )
    {
       w2c_xslt_utils_add_attribute (env, root, "wrapped", "yes");
    }
    flag = 
        W2C_ENGINE_CONFIGURATION_GET_ASYNC_ON( emitter_impl-> config, env);
    if ( flag)
    {
        w2c_xslt_utils_add_attribute (env, root, "isAsync", "1");
    }
    flag = 
        W2C_ENGINE_CONFIGURATION_GET_SYNC_ON( emitter_impl-> config, env);
    if ( flag)
    {
        w2c_xslt_utils_add_attribute (env, root, "isSync", "1");
    }
    

    w2c_xslt_utils_copy_node_tree( env, root, emitter_impl-> common_node);

    AXIS2_FREE (env->allocator, full_name );
    AXIS2_FREE (env->allocator, full_name_c );
      
    return root;
}

axiom_node_t*
w2c_c_emitter_create_for_skel(w2c_emitter_impl_t *emitter_impl,
                                   const axutil_env_t *env )
{
    axiom_node_t *root = NULL;
    axis2_char_t *full_name = NULL;
    axis2_char_t *full_name_c = NULL;
    axis2_char_t *given_name = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;
    
    qname = w2c_emitter_pick_service_name( emitter_impl, env );
    given_name = W2C_QNAME2NAME_MAKER_SUGGEST_NAME( 
            emitter_impl-> qname2name_maker, env, qname);
    qname_str = axis2_qname_to_string(qname, env);
    
    root = w2c_xslt_utils_add_child_node(env, "interface", NULL);
    full_name = axis2_strdup (env, W2C_C_EMITTER_SKEL_PREFIX);
    full_name = w2c_string_add_string ( full_name, given_name, env );
    full_name = w2c_string_add_string 
                            ( full_name, W2C_C_EMITTER_SKEL_SUFFIX, env);
    full_name_c = emitter_impl-> name_maker_func( full_name, env);

    w2c_xslt_utils_add_attribute (env, root, "name", full_name_c);
    w2c_xslt_utils_add_attribute (env, root, "prefix", full_name_c);
    w2c_xslt_utils_add_attribute (env, root, "package", "");
    w2c_xslt_utils_add_attribute (env, root, "qname", qname_str); 

    w2c_xslt_utils_copy_node_tree( env, root, emitter_impl-> common_node);

    AXIS2_FREE (env->allocator, full_name );
    AXIS2_FREE (env->allocator, full_name_c );

    return root;
}

axiom_node_t*
w2c_c_emitter_create_for_svc_skel(w2c_emitter_impl_t *emitter_impl,
                                   const axutil_env_t *env )
{
    axiom_node_t *root = NULL;
    axis2_char_t *skel_name = NULL;
    axis2_char_t *skel_name_c = NULL;
    axis2_char_t *svc_skel_name = NULL;
    axis2_char_t *svc_skel_name_c = NULL;
    axis2_char_t *given_name = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;
    
    qname = w2c_emitter_pick_service_name( emitter_impl, env );
    given_name = W2C_QNAME2NAME_MAKER_SUGGEST_NAME( 
            emitter_impl-> qname2name_maker, env, qname);
    qname_str = axis2_qname_to_string(qname, env);
    
    root = w2c_xslt_utils_add_child_node(env, "interface", NULL);
    svc_skel_name = axis2_strdup (env, W2C_C_EMITTER_SVC_SKEL_PREFIX);
    svc_skel_name = w2c_string_add_string ( svc_skel_name, given_name, env );
    svc_skel_name = w2c_string_add_string 
                            ( svc_skel_name, W2C_C_EMITTER_SVC_SKEL_SUFFIX, env);
    svc_skel_name_c = emitter_impl-> name_maker_func( svc_skel_name, env);

    w2c_xslt_utils_add_attribute (env, root, "name", svc_skel_name_c);
    w2c_xslt_utils_add_attribute (env, root, "prefix", svc_skel_name_c);
    w2c_xslt_utils_add_attribute (env, root, "qname", qname_str);

    skel_name = axis2_strdup (env, W2C_C_EMITTER_SKEL_PREFIX);
    skel_name = w2c_string_add_string ( skel_name, given_name, env );
    skel_name = w2c_string_add_string 
                            ( skel_name, W2C_C_EMITTER_SKEL_SUFFIX, env);
    skel_name_c = emitter_impl-> name_maker_func( skel_name, env);

    w2c_xslt_utils_add_attribute (env, root, "svcname", skel_name_c);
    w2c_xslt_utils_add_attribute (env, root, "svcop_prefix", skel_name_c);
    w2c_xslt_utils_add_attribute (env, root, "package", "");
    
    w2c_xslt_utils_copy_node_tree( env, root, emitter_impl-> common_node);

    AXIS2_FREE (env->allocator, skel_name );
    AXIS2_FREE (env->allocator, skel_name_c );
    AXIS2_FREE (env->allocator, svc_skel_name );
    AXIS2_FREE (env->allocator, svc_skel_name_c );

    return root;
}

axiom_node_t*
w2c_c_emitter_create_for_svc_xml(w2c_emitter_impl_t *emitter_impl,
                                   const axutil_env_t *env )
{
    axiom_node_t *root = NULL;
    axis2_char_t *svc_name = NULL;
    axis2_char_t *c_svc_name = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *qname_str = NULL;
    
    qname = w2c_emitter_pick_service_name( emitter_impl, env );
    svc_name = W2C_QNAME2NAME_MAKER_SUGGEST_NAME( 
            emitter_impl-> qname2name_maker, env, qname);
    qname_str = axis2_qname_to_string(qname, env);
    c_svc_name = emitter_impl-> name_maker_func( svc_name, env);
    
    root = w2c_xslt_utils_add_child_node(env, "interface", NULL);
    w2c_xslt_utils_add_attribute (env, root, "servicename", svc_name);
    w2c_xslt_utils_add_attribute (env, root, "c_servicename", c_svc_name);
    w2c_xslt_utils_add_attribute (env, root, "qname", qname_str);
    
    w2c_xslt_utils_copy_node_tree( env, root, emitter_impl-> common_node);

    return root;
}

