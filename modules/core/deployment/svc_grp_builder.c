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

#include <axis2_svc_grp_builder.h>

struct axis2_svc_grp_builder
{
    axiom_node_t *svc_grp;
    axis2_desc_builder_t *desc_builder;
};

AXIS2_EXTERN axis2_svc_grp_builder_t *AXIS2_CALL
axis2_svc_grp_builder_create(
    const axis2_env_t *env)
{
    axis2_svc_grp_builder_t *svc_grp_builder = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    svc_grp_builder = (axis2_svc_grp_builder_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_svc_grp_builder_t));


    if (NULL == svc_grp_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    svc_grp_builder->svc_grp = NULL;
    svc_grp_builder->desc_builder = NULL;

    return svc_grp_builder;
}

AXIS2_EXTERN axis2_svc_grp_builder_t *AXIS2_CALL
axis2_svc_grp_builder_create_with_svc_and_dep_engine(
    const axis2_env_t *env,
    axiom_node_t *svc_grp,
    axis2_dep_engine_t *dep_engine)
{
    axis2_svc_grp_builder_t *svc_grp_builder = NULL;

    svc_grp_builder = (axis2_svc_grp_builder_t *)
            axis2_svc_grp_builder_create(env);
    if (!svc_grp_builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        svc_grp_builder = NULL;
    }
    svc_grp_builder->desc_builder =
        axis2_desc_builder_create_with_dep_engine(env, dep_engine);
    if (!svc_grp_builder->desc_builder)
    {
        axis2_svc_grp_builder_free(svc_grp_builder, env);
        return NULL;
    }
    svc_grp_builder->svc_grp = svc_grp;

    return svc_grp_builder;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_grp_builder_free(
    axis2_svc_grp_builder_t *svc_grp_builder,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (svc_grp_builder->desc_builder)
    {
        AXIS2_DESC_BUILDER_FREE(svc_grp_builder->desc_builder,
                env);
    }
    if (svc_grp_builder)
    {
        AXIS2_FREE(env->allocator, svc_grp_builder);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_grp_builder_populate_svc_grp(
    axis2_svc_grp_builder_t *svc_grp_builder,
    const axis2_env_t *env,
    axis2_svc_grp_t *svc_grp)
{
    axiom_children_qname_iterator_t *itr = NULL;
    axiom_children_qname_iterator_t *module_ref_itr = NULL;
    axiom_children_qname_iterator_t *svc_itr = NULL;
    axis2_qname_t *qparamst = NULL;
    axis2_qname_t *qmodulest = NULL;
    axis2_qname_t *qsvc_element = NULL;
    axiom_element_t *svc_grp_element = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_conf_t *parent = NULL;

    /* Processing service level paramters */
    svc_grp_element = AXIOM_NODE_GET_DATA_ELEMENT(svc_grp_builder->svc_grp, env);
    qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);

    itr = axiom_element_get_children_with_qname(svc_grp_element, env, qparamst,
            svc_grp_builder->svc_grp);
    
    if (qparamst)
    {
        AXIS2_QNAME_FREE(qparamst, env);
        qparamst = NULL;
    }

    parent =  axis2_svc_grp_get_parent(svc_grp, env);
    status = AXIS2_DESC_BUILDER_PROCESS_PARAMS(svc_grp_builder->desc_builder, env, itr, 
            axis2_svc_grp_get_param_container(svc_grp, env), 
            axis2_conf_get_param_container(parent, env));

    /* Processing service modules required to be engaged globally */
    qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
    module_ref_itr = axiom_element_get_children_with_qname(svc_grp_element, env,
            qmodulest, svc_grp_builder->svc_grp);

    if (qmodulest)
    {
        AXIS2_QNAME_FREE(qmodulest, env);
        qmodulest = NULL;
    }

    axis2_svc_grp_builder_process_module_refs(svc_grp_builder, env, module_ref_itr,
            svc_grp);
    qsvc_element = axis2_qname_create(env, AXIS2_SVC_ELEMENT, NULL, NULL);
    svc_itr = axiom_element_get_children_with_qname(svc_grp_element, env,
            qsvc_element, svc_grp_builder->svc_grp);

    if (qsvc_element)
    {
        AXIS2_QNAME_FREE(qsvc_element, env);
        qsvc_element = NULL;
    }

    while (AXIS2_TRUE == axiom_children_qname_iterator_has_next(svc_itr, env))
    {
        axiom_node_t *svc_node = NULL;
        axiom_element_t *svc_element = NULL;
        axiom_attribute_t *svc_name_att = NULL;
        axis2_char_t *svc_name = NULL;
        axis2_qname_t *qattname = NULL;

        svc_node = (axiom_node_t *) axiom_children_qname_iterator_next(
                    svc_itr, env);
        svc_element = AXIOM_NODE_GET_DATA_ELEMENT(svc_node, env);
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        svc_name_att = axiom_element_get_attribute(svc_element, env, qattname);

        if (qattname)
        {
            AXIS2_QNAME_FREE(qattname, env);
            qattname = NULL;
        }

        svc_name = axiom_attribute_get_value(svc_name_att, env);
        if (NULL == svc_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_NAME_ERROR,
                    AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        else
        {
            axis2_svc_t *axis_svc = NULL;
            axis2_arch_file_data_t *file_data = NULL;
            axis2_array_list_t *deployable_svcs = NULL;
            axis2_svc_builder_t *svc_builder = NULL;

            file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(
                axis2_desc_builder_get_dep_engine(svc_grp_builder->desc_builder, env), 
                env);
            axis_svc = AXIS2_ARCH_FILE_DATA_GET_SVC(file_data, env, svc_name);
            if (NULL == axis_svc)
            {
                axis2_qname_t *qsvc_name = NULL;

                qsvc_name = axis2_qname_create(env, svc_name, NULL, NULL);
                axis_svc = axis2_svc_create_with_qname(env, qsvc_name);
                AXIS2_QNAME_FREE(qsvc_name, env);
                AXIS2_ARCH_FILE_DATA_ADD_SVC(file_data, env, axis_svc);

            }
            /* the service that has to be deployed */

            deployable_svcs = AXIS2_ARCH_FILE_DATA_GET_DEPLOYABLE_SVCS(file_data,
                    env);
            axis2_array_list_add(deployable_svcs, env, axis_svc);
            AXIS2_SVC_SET_PARENT(axis_svc, env, svc_grp);
            svc_builder = axis2_svc_builder_create_with_dep_engine_and_svc(env,
                axis2_desc_builder_get_dep_engine(svc_grp_builder->desc_builder, env), 
                axis_svc);
            status = AXIS2_SVC_BUILDER_POPULATE_SVC(svc_builder, env, svc_node);
            AXIS2_SVC_BUILDER_FREE(svc_builder, env);

        }
    }
    return status;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_svc_grp_builder_process_module_refs(
    axis2_svc_grp_builder_t *svc_grp_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *module_refs ,
    axis2_svc_grp_t *svc_grp)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, module_refs, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svc_grp, AXIS2_FAILURE);

    while (AXIS2_TRUE == axiom_children_qname_iterator_has_next(module_refs, env))
    {
        axiom_node_t *module_ref_node = NULL;
        axiom_element_t *module_ref_element = NULL;
        axiom_attribute_t *module_ref_att = NULL;
        axis2_qname_t *qref = NULL;

        module_ref_node = (axiom_node_t *) axiom_children_qname_iterator_next(
                    module_refs, env);
        module_ref_element = AXIOM_NODE_GET_DATA_ELEMENT(module_ref_node, env);
        qref = axis2_qname_create(env, AXIS2_REF, NULL, NULL);
        module_ref_att = axiom_element_get_attribute(module_ref_element, env,
                qref);
        if (module_ref_att)
        {
            axis2_char_t *ref_name = NULL;
            axis2_qname_t *qrefname = NULL;
            axis2_module_desc_t *module = NULL;


            ref_name = axiom_attribute_get_value(module_ref_att, env);
            qrefname = axis2_qname_create(env, ref_name, NULL, NULL);
            module = AXIS2_DEP_ENGINE_GET_MODULE(
                axis2_desc_builder_get_dep_engine(svc_grp_builder->desc_builder, env), 
                env, qrefname);
            if (NULL == module)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MODULE_NOT_FOUND,
                        AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            else
            {
                 axis2_svc_grp_add_module_ref(svc_grp, env, qrefname);
            }
            AXIS2_QNAME_FREE(qrefname, env);
        }
        AXIS2_QNAME_FREE(qref, env);
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_desc_builder_t *AXIS2_CALL
axis2_svc_grp_builder_get_desc_builder(const axis2_svc_grp_builder_t *svc_grp_builder,
    const axis2_env_t *env)
{
    return svc_grp_builder->desc_builder;
}

