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

#include <axis2_arch_reader.h>
#include <string.h>
#include <axis2_class_loader.h>
#include <axis2_svc_builder.h>
#include <axis2_module_builder.h>
#include <axis2_svc.h>

/**
 * To do the common tasks for all Builder
 */
typedef struct axis2_arch_reader_impl
{
    axis2_arch_reader_t arch_reader;
    axis2_desc_builder_t *desc_builder;

}
axis2_arch_reader_impl_t;

#define AXIS2_INTF_TO_IMPL(arch_reader) \
    ((axis2_arch_reader_impl_t *) arch_reader)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_arch_reader_free(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env);

struct axis2_svc *AXIS2_CALL
            axis2_arch_reader_create_svc(
                axis2_arch_reader_t *arch_reader,
                const axis2_env_t *env,
                struct axis2_arch_file_data *file);

axis2_status_t AXIS2_CALL
axis2_arch_reader_process_svc_grp(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *file_path,
    struct axis2_dep_engine *dep_engine,
    axis2_svc_grp_t *svc_grp);

axis2_status_t AXIS2_CALL
axis2_arch_reader_build_svc_grp(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *file_path,
    struct axis2_dep_engine *dep_engine,
    struct axis2_svc_grp *svc_grp);

axis2_status_t AXIS2_CALL
axis2_arch_reader_read_module_arch(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *file_path,
    struct axis2_dep_engine *dep_engine,
    axis2_module_desc_t *module);

axis2_file_t *AXIS2_CALL
axis2_arch_reader_create_module_arch(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *module_name);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_arch_reader_t *AXIS2_CALL
axis2_arch_reader_create(
    const axis2_env_t *env)
{
    axis2_arch_reader_impl_t *arch_reader_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    arch_reader_impl = (axis2_arch_reader_impl_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_arch_reader_impl_t));


    if (NULL == arch_reader_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    arch_reader_impl->desc_builder = NULL;
    arch_reader_impl->arch_reader.ops = NULL;

    arch_reader_impl->arch_reader.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_arch_reader_ops_t));
    if (NULL == arch_reader_impl->arch_reader.ops)
    {
        axis2_arch_reader_free(&(arch_reader_impl->arch_reader), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    arch_reader_impl->arch_reader.ops->free = axis2_arch_reader_free;
    arch_reader_impl->arch_reader.ops->create_svc = axis2_arch_reader_create_svc;
    arch_reader_impl->arch_reader.ops->process_svc_grp =
        axis2_arch_reader_process_svc_grp;
    arch_reader_impl->arch_reader.ops->build_svc_grp =
        axis2_arch_reader_build_svc_grp;
    arch_reader_impl->arch_reader.ops->read_module_arch =
        axis2_arch_reader_read_module_arch;
    arch_reader_impl->arch_reader.ops->create_module_arch =
        axis2_arch_reader_create_module_arch;

    return &(arch_reader_impl->arch_reader);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_arch_reader_free(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env)
{
    axis2_arch_reader_impl_t *arch_reader_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    arch_reader_impl = AXIS2_INTF_TO_IMPL(arch_reader);

    if (arch_reader->ops)
        AXIS2_FREE(env->allocator, arch_reader->ops);

    if (arch_reader_impl->desc_builder)
    {
        AXIS2_DESC_BUILDER_FREE(arch_reader_impl->desc_builder, env);
        arch_reader_impl->desc_builder = NULL;
    }

    if (arch_reader_impl)
    {
        AXIS2_FREE(env->allocator, arch_reader_impl);
        arch_reader_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

struct axis2_svc *AXIS2_CALL
            axis2_arch_reader_create_svc(
                axis2_arch_reader_t *arch_reader,
                const axis2_env_t *env,
                struct axis2_arch_file_data *file)
{
    axis2_svc_t *svc = NULL;
    /* TODO comment this until WODEN implementation is done */
    /*
    axis2_char_t *doc_base_uri = NULL;
    axiom_document_t *doc = NULL;
    axis2_hash_t *svcs = NULL;
    axis2_hash_index_t *index = NULL;
    axis2_bool_t *found_svc = AXIS2_FALSE;
    if(// Check for wsdl file)
    {
        doc = get_root_element_from_filename(env, filename);
        doc_base_uri = AXIS2_STRACAT (axis2c_home, "/woden", env);
        reader = woden_reader_create(env); 
        desc = (void *)WODEN_READER_READ_WSDL(reader, env, om_doc, doc_base_uri);
        svcs = WODEN_DESC_GET_SVCS(desc, env);
        if(svcs)
        {
            void *key = NULL;
            axis2_hash_index_t *index = NULL;
        
            axis2_hash_this (index, *key, NULL, NULL);
            if(!key) 
            {
                svc = axis2_svc_create(env, wsdl_svc);
            }
            else
            { 
                wsdl_svc = axis2_hash_get(svcs, key, AXIS2_HASH_KEY_STRING);
                // remove <wsdl:service> and <wsdl:binding> elements from the service
                // description we read in as we will be replacing them anyway.
                svc = axis2_svc_create(env, wsdl_svc);
                qname = AXIS2_WSDL_SVC_GET_QNAME(wsdl_svc, env);
                AXIS2_SVC_SET_QNAME(svc, env, qname);
                AXIS2_SVC_SET_WSDL_DEF(svc, env, desc);
                curr_file_item = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env);
                AXIS2_ARCH_FILE_DATA_ADD_SVC(curr_file_item, env, svc);
            }
        }
    }
    else 
    {
    */
    svc = axis2_svc_create(env);
    /*TODO log */
    /*WSDL_FILE_NOT_FOUND, filename);
    */
    /*
    }
    */
    return svc;
}

/* TODO comment this until WOM implementation is done */
/*
axis2_status_t *AXIS2_CALL
axis2_arch_reader_process_wsdl_file(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *filename,
    axis2_dep_engine_t *dep_engine)
{
    axis2_char_t *doc_base_uri = NULL;
    axiom_document_t *doc = NULL;
    axis2_hash_t *bindings = NULL;
    axis2_hash_t *svcs = NULL;
    axis2_hash_index_t *index = NULL;

    doc = get_root_element_from_filename(env, filename);
    doc_base_uri = AXIS2_STRACAT (axis2c_home, "/woden", env);
    reader = woden_reader_create(env);
    desc = (void *)WODEN_READER_READ_WSDL(reader, env, om_doc, doc_base_uri);
    // Remove binding
    bindings = WODEN_DESC_GET_BINDINGS(desc, env);
    for (index = axis2_hash_first (bindings, env); index; index =
            axis2_hash_next (env, index))
    {
        void *key = NULL;

        axis2_hash_this (index, *key, NULL, NULL);
        axis2_hash_set(bindings, key, AXIS2_HASH_KEY_STRING, NULL)
    }
    svcs = WODEN_DESC_GET_BINDINGS(desc, env);
    for (index = axis2_hash_first (svcs, env); index; index =
            axis2_hash_next (env, index))
    {
        void *key = NULL;
        void *wsdl_svc = NULL;
        axiss2_svc_desc_t *svc = NULL;

        axis2_hash_this (index, *key, NULL, NULL);
        wsdl_svc = axis2_hash_get(svcs, key, AXIS2_HASH_KEY_STRING);
        svc = axis2_svc_create(env, wsdl_svc);
        qname = AXIS2_WSDL_SVC_GET_QNAME(wsdl_svc, env);
        AXIS2_SVC_SET_QNAME(svc, env, qname);
        AXIS2_SVC_SET_WSDL_DEF(svc, env, desc);
        curr_file_item = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env);
        AXIS2_ARCH_FILE_DATA_ADD_SVC(curr_file_item, env, svc);
    }
}
*/

/**
 * To create service objects out form wsdls file inside a service archive file
 * @param file <code>arch_file_data</code>
 * @param dep_engine <code>dep_engine</code>
 */
/* TODO comment this until WOM implementation is done */
/*axis2_status_t AXIS2_CALL
axis2_arch_reader_process_wsdls(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_arch_file_data_t *file,
    axis2_dep_engine_t *dep_engine)
{
    axis2_file_t *svc_file = NULL;

    // List the wsdl files in a wsdl directory.
     // Calculate the path for each wsdl and call
     // process_wsdl method
     //
    process_wsdl_file(arch_reader, env, wsdl_file, dep_engine);
    return AXIS2_SUCCESS;
}
*/

axis2_status_t AXIS2_CALL
axis2_arch_reader_process_svc_grp(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *file_name,
    struct axis2_dep_engine *dep_engine,
    axis2_svc_grp_t *svc_grp)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *svcs_xml = NULL;
    axis2_char_t *repos_path = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, file_name, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dep_engine, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_grp, AXIS2_FAILURE);

    repos_path = AXIS2_DEP_ENGINE_GET_REPOS_PATH(dep_engine, env);

    svcs_xml = axis2_strcat(env, repos_path, AXIS2_PATH_SEP_STR,
            AXIS2_SERVICE_FOLDER, AXIS2_PATH_SEP_STR, file_name,
            AXIS2_PATH_SEP_STR, AXIS2_SVC_XML, NULL);

    if (!svcs_xml)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status = axis2_file_handler_access(svcs_xml, AXIS2_F_OK);
    if (AXIS2_SUCCESS == status)
    {
        struct axis2_arch_file_data *arch_file_data = NULL;
        axis2_char_t *svc_name = NULL;

        status = axis2_arch_reader_build_svc_grp(arch_reader, env, svcs_xml,
                dep_engine, svc_grp);
        if (AXIS2_SUCCESS != status)
        {
            return status;
        }
        arch_file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env);
        svc_name = AXIS2_ARCH_FILE_DATA_GET_SVC_NAME(arch_file_data, env);
        AXIS2_SVC_GRP_SET_NAME(svc_grp, env, svc_name);
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SERVICE_XML_NOT_FOUND,
                AXIS2_FAILURE);
        status = AXIS2_FAILURE;
    }
    AXIS2_FREE(env->allocator, svcs_xml);
    return status;
}

axis2_status_t AXIS2_CALL
axis2_arch_reader_build_svc_grp(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *svc_xml,
    axis2_dep_engine_t *dep_engine,
    axis2_svc_grp_t *svc_grp)
{
    axis2_arch_reader_impl_t *arch_reader_impl = NULL;
    axis2_char_t *root_element_name = NULL;
    axiom_node_t *svcs = NULL;
    axiom_element_t *svcs_element = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_xml, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dep_engine, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_grp, AXIS2_FAILURE);
    arch_reader_impl = AXIS2_INTF_TO_IMPL(arch_reader);

    if (arch_reader_impl->desc_builder)
    {
        AXIS2_DESC_BUILDER_FREE(arch_reader_impl->desc_builder, env);
        arch_reader_impl->desc_builder = NULL;
    }
    arch_reader_impl->desc_builder =
        axis2_desc_builder_create_with_file_and_dep_engine(env, svc_xml,
                dep_engine);
    if (!arch_reader_impl->desc_builder)
    {
        return AXIS2_FAILURE;
    }
    svcs = AXIS2_DESC_BUILDER_BUILD_OM(arch_reader_impl->desc_builder, env);
    svcs_element = AXIOM_NODE_GET_DATA_ELEMENT(svcs, env);
    root_element_name = AXIOM_ELEMENT_GET_LOCALNAME(svcs_element, env);
    if (0 == AXIS2_STRCMP(AXIS2_SVC_ELEMENT, root_element_name))
    {
        axis2_svc_t *svc = NULL;
        axis2_svc_builder_t *svc_builder = NULL;
        axis2_arch_file_data_t *file_data = NULL;
        axis2_array_list_t *dep_svcs = NULL;
        axis2_char_t *svc_name = NULL;

        file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env);
        svc_name = AXIS2_ARCH_FILE_DATA_GET_NAME(file_data, env);
        svc = AXIS2_ARCH_FILE_DATA_GET_SVC(file_data, env, svc_name);
        if (NULL == svc)
        {
            axis2_qname_t *svc_qname = NULL;

            svc_qname = axis2_qname_create(env, svc_name, NULL, NULL);
            svc = axis2_svc_create_with_qname(env, svc_qname);
            status = AXIS2_ARCH_FILE_DATA_ADD_SVC(file_data, env, svc);
            AXIS2_QNAME_FREE(svc_qname, env);
            if (AXIS2_SUCCESS != status)
            {
                AXIS2_SVC_FREE(svc, env);
                return status;
            }
        }
        AXIS2_SVC_SET_PARENT(svc, env, svc_grp);

        svc_builder = axis2_svc_builder_create_with_dep_engine_and_svc(env,
                dep_engine, svc);
        status = AXIS2_SVC_BUILDER_POPULATE_SVC(svc_builder, env, svcs);
        AXIS2_SVC_BUILDER_FREE(svc_builder, env);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "populating service is not successful");

            return AXIS2_FAILURE;
        }

        dep_svcs = AXIS2_ARCH_FILE_DATA_GET_DEPLOYABLE_SVCS(file_data, env);
        if (!dep_svcs)
        {
            return AXIS2_FAILURE;
        }
        status = AXIS2_ARRAY_LIST_ADD(dep_svcs, env, svc);
        if (AXIS2_SUCCESS != status)
        {
            return AXIS2_FAILURE;
        }

    }
    else if (0 == AXIS2_STRCMP(AXIS2_SVC_GRP_ELEMENT, root_element_name))
    {
        axis2_svc_grp_builder_t *grp_builder = NULL;
        grp_builder = axis2_svc_grp_builder_create_with_svc_and_dep_engine(env,
                svcs, dep_engine);
        status = AXIS2_SVC_GRP_BUILDER_POPULATE_SVC_GRP(grp_builder, env, svc_grp);
        AXIS2_SVC_GRP_BUILDER_FREE(grp_builder, env);
    }
    return status;
}

axis2_status_t AXIS2_CALL
axis2_arch_reader_read_module_arch(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *file_name,
    axis2_dep_engine_t *dep_engine,
    axis2_module_desc_t *module_desc)
{
    axis2_arch_reader_impl_t *arch_reader_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *module_xml = NULL;
    axis2_char_t *repos_path = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, file_name, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dep_engine, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_desc, AXIS2_FAILURE);
    arch_reader_impl = AXIS2_INTF_TO_IMPL(arch_reader);

    repos_path = AXIS2_DEP_ENGINE_GET_REPOS_PATH(dep_engine, env);
    module_xml = axis2_strcat(env, repos_path, AXIS2_PATH_SEP_STR,
            AXIS2_MODULE_FOLDER, AXIS2_PATH_SEP_STR, file_name,
            AXIS2_PATH_SEP_STR, AXIS2_MODULE_XML, NULL);
    if (!module_xml)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status = axis2_file_handler_access(module_xml, AXIS2_F_OK);

    if (AXIS2_SUCCESS == status)
    {
        axis2_module_builder_t *module_builder = NULL;
        module_builder =
            axis2_module_builder_create_with_file_and_dep_engine_and_module(env,
                    module_xml, dep_engine, module_desc);
        status = AXIS2_MODULE_BUILDER_POPULATE_MODULE(module_builder, env);
        AXIS2_MODULE_BUILDER_FREE(module_builder, env);
    }
    else
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_MODULE_XML_NOT_FOUND_FOR_THE_MODULE, AXIS2_FAILURE);
        status = AXIS2_FAILURE;
    }
    AXIS2_FREE(env->allocator, module_xml);
    return status;
}

axis2_file_t *AXIS2_CALL
axis2_arch_reader_create_module_arch(
    axis2_arch_reader_t *arch_reader,
    const axis2_env_t *env,
    axis2_char_t *module_name)
{
    axis2_file_t *file = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    file = axis2_file_create(env);
    if (!file)
    {
        return NULL;
    }
    AXIS2_FILE_SET_NAME(file, env, module_name);
    return file;
}
