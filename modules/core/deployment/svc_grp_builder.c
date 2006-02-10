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
 
#include <axis2_svc_grp_builder.h>

/** 
 * @brief
 */
typedef struct axis2_svc_grp_builder_impl
{
	axis2_svc_grp_builder_t svc_grp_builder;
    axis2_om_node_t *svc_grp;
    	
} axis2_svc_grp_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(svc_grp_builder) \
    ((axis2_svc_grp_builder_impl_t *) svc_grp_builder)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_svc_grp_builder_free (axis2_svc_grp_builder_t *svc_grp_builder, 
                            axis2_env_t **env);


axis2_status_t AXIS2_CALL
axis2_svc_grp_builder_populate_svc_grp(axis2_svc_grp_builder_t *grp_builder,
                                        axis2_env_t **env,
                                        axis2_svc_grp_t *svc_grp);

/**
 * To get the list og modules that is requird to be engage globally
 * @param module_refs  <code>axis2_om_children_qname_iterator_t</code>
 */
axis2_status_t AXIS2_CALL
axis2_svc_grp_builder_process_module_refs(axis2_svc_grp_builder_t *grp_builder,
                                            axis2_env_t **env,
                                   axis2_om_children_qname_iterator_t *module_refs ,
                                            axis2_svc_grp_t *svc_grp);
                                            
/************************** End of function prototypes ************************/

axis2_svc_grp_builder_t * AXIS2_CALL 
axis2_svc_grp_builder_create (axis2_env_t **env)
{
    axis2_svc_grp_builder_impl_t *svc_grp_builder_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	svc_grp_builder_impl = (axis2_svc_grp_builder_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_svc_grp_builder_impl_t));
	
	
	if(NULL == svc_grp_builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    svc_grp_builder_impl->svc_grp_builder.ops = NULL;
    svc_grp_builder_impl->svc_grp = NULL;
    svc_grp_builder_impl->svc_grp_builder.desc_builder = NULL;
    
	svc_grp_builder_impl->svc_grp_builder.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_grp_builder_ops_t));
	if(NULL == svc_grp_builder_impl->svc_grp_builder.ops)
    {
        axis2_svc_grp_builder_free(&(svc_grp_builder_impl->svc_grp_builder), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	svc_grp_builder_impl->svc_grp_builder.ops->free = axis2_svc_grp_builder_free;
    svc_grp_builder_impl->svc_grp_builder.ops->populate_svc_grp = 
        axis2_svc_grp_builder_populate_svc_grp;
    svc_grp_builder_impl->svc_grp_builder.ops->process_module_refs = 
        axis2_svc_grp_builder_process_module_refs;
    
	return &(svc_grp_builder_impl->svc_grp_builder);
}

axis2_svc_grp_builder_t * AXIS2_CALL 
axis2_svc_grp_builder_create_with_svc_and_dep_engine (axis2_env_t **env,
                                                axis2_om_node_t *svc_grp,
                                                axis2_dep_engine_t *dep_engine)
{
    axis2_svc_grp_builder_impl_t *grp_builder_impl = NULL;
    
    grp_builder_impl = (axis2_svc_grp_builder_impl_t *) 
        axis2_svc_grp_builder_create(env);
    if(!grp_builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        grp_builder_impl = NULL;
    }
    grp_builder_impl->svc_grp_builder.desc_builder = 
        axis2_desc_builder_create_with_dep_engine(env, dep_engine);
    if(!grp_builder_impl->svc_grp_builder.desc_builder)
    {
        axis2_svc_grp_builder_free(&(grp_builder_impl->svc_grp_builder), env);
        return NULL;
    }
    grp_builder_impl->svc_grp = svc_grp;
    
    return &(grp_builder_impl->svc_grp_builder);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_svc_grp_builder_free (axis2_svc_grp_builder_t *svc_grp_builder, 
                            axis2_env_t **env)
{
    axis2_svc_grp_builder_impl_t *grp_builder_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    grp_builder_impl = AXIS2_INTF_TO_IMPL(svc_grp_builder);
    
    if(grp_builder_impl->svc_grp_builder.desc_builder)
    {
        AXIS2_DESC_BUILDER_FREE(grp_builder_impl->svc_grp_builder.desc_builder,
            env);
        grp_builder_impl->svc_grp_builder.desc_builder = NULL;
    }
	if(NULL != svc_grp_builder->ops)
    {
        AXIS2_FREE((*env)->allocator, svc_grp_builder->ops);
        svc_grp_builder->ops = NULL;
    }
    
    if(grp_builder_impl)
    {
        AXIS2_FREE((*env)->allocator, grp_builder_impl);
        grp_builder_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_grp_builder_populate_svc_grp(axis2_svc_grp_builder_t *grp_builder,
                                        axis2_env_t **env,
                                        axis2_svc_grp_t *svc_grp)
{
    axis2_svc_grp_builder_impl_t *grp_builder_impl = NULL;
    axis2_om_children_qname_iterator_t *itr = NULL;
    axis2_om_children_qname_iterator_t *module_ref_itr = NULL;
    axis2_om_children_qname_iterator_t *svc_itr = NULL;
    axis2_qname_t *qparamst = NULL;
    axis2_qname_t *qmodulest = NULL;
    axis2_qname_t *qsvc_element = NULL;
    axis2_om_element_t *svc_grp_element = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_conf_t *parent = NULL;
    
    grp_builder_impl = AXIS2_INTF_TO_IMPL(grp_builder);
    
    /* Processing service level paramters */
    svc_grp_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(grp_builder_impl->svc_grp, env);
    qparamst = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
    
    itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(svc_grp_element, env, qparamst,
        grp_builder_impl->svc_grp);
    
    parent = AXIS2_SVC_GRP_GET_PARENT(svc_grp, env);
    status = AXIS2_DESC_BUILDER_PROCESS_PARAMS(grp_builder_impl->svc_grp_builder.
        desc_builder, env, itr, svc_grp->param_container, parent->param_container);

    /* processing servicewide modules required to engage gloabbly */
    qmodulest = axis2_qname_create(env, AXIS2_MODULEST, NULL, NULL);
    module_ref_itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(svc_grp_element, env,
        qmodulest, grp_builder_impl->svc_grp);
    axis2_svc_grp_builder_process_module_refs(grp_builder, env, module_ref_itr, 
        svc_grp);
    qsvc_element = axis2_qname_create(env, AXIS2_SVC_ELEMENT, NULL, NULL);
    svc_itr = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(svc_grp_element, env,
        qsvc_element, grp_builder_impl->svc_grp);
    while(AXIS2_TRUE == AXIS2_OM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(svc_itr, env))
    {
        axis2_om_node_t *svc_node = NULL;
        axis2_om_element_t *svc_element = NULL;
        axis2_om_attribute_t *svc_name_att = NULL;
        axis2_char_t *svc_name = NULL;
        axis2_qname_t *qattname = NULL;
        
        svc_node = (axis2_om_node_t *) AXIS2_OM_CHILDREN_QNAME_ITERATOR_NEXT(
            svc_itr, env);
        svc_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(svc_node, env);
        qattname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        svc_name_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(svc_element, env, qattname);
        svc_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(svc_name_att, env);
        if(NULL == svc_name)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SVC_NAME_ERROR,
                AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        else
        {
            axis2_svc_t *axis_svc = NULL;
            axis2_arch_file_data_t *file_data = NULL;
            axis2_array_list_t *deployable_svcs = NULL;
            axis2_svc_builder_t *svc_builder = NULL;
            
            file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(grp_builder->
                desc_builder->engine, env);
            axis_svc = AXIS2_ARCH_FILE_DATA_GET_SVC(file_data, env, svc_name);
            if(NULL == axis_svc)
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
            AXIS2_ARRAY_LIST_ADD(deployable_svcs, env, axis_svc);
            AXIS2_SVC_SET_PARENT(axis_svc, env, svc_grp);
            svc_builder = axis2_svc_builder_create_with_dep_engine_and_svc(env,
                grp_builder->desc_builder->engine, axis_svc);
            status = AXIS2_SVC_BUILDER_POPULATE_SVC(svc_builder, env, svc_node);
            AXIS2_SVC_BUILDER_FREE(svc_builder, env);
            
        }
    }
    return status;
}

axis2_status_t AXIS2_CALL
axis2_svc_grp_builder_process_module_refs(axis2_svc_grp_builder_t *grp_builder,
                                            axis2_env_t **env,
                                   axis2_om_children_qname_iterator_t *module_refs ,
                                            axis2_svc_grp_t *svc_grp)
{
    axis2_svc_grp_builder_impl_t *grp_builder_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_refs, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
    grp_builder_impl = AXIS2_INTF_TO_IMPL(grp_builder);
    
    while (AXIS2_TRUE == AXIS2_OM_CHILDREN_ITERATOR_HAS_NEXT(module_refs, env))
    {
        axis2_om_node_t *module_ref_node = NULL;
        axis2_om_element_t *module_ref_element = NULL;
        axis2_om_attribute_t *module_ref_att = NULL;
        axis2_qname_t *qref = NULL;
        
        module_ref_node = (axis2_om_node_t *) AXIS2_OM_CHILDREN_ITERATOR_NEXT(
            module_refs, env);
        module_ref_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(module_ref_node, env);
        qref = axis2_qname_create(env, AXIS2_REF, NULL, NULL);
        module_ref_att = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(module_ref_element, env,
            qref);
        if(NULL != module_ref_att)
        {
            axis2_char_t *ref_name = NULL;
            axis2_qname_t *qrefname = NULL;
            axis2_module_desc_t *module = NULL;
            
            
            ref_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(module_ref_att, env);
            qrefname = axis2_qname_create(env, ref_name, NULL, NULL);
            module = AXIS2_DEP_ENGINE_GET_MODULE(grp_builder->desc_builder->
                engine, env, qrefname);
            if(NULL == module)
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MODULE_NOT_FOUND,
                    AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }   
            else
            {
                AXIS2_SVC_GRP_ADD_MODULEREF(svc_grp, env, qrefname);
            }
            AXIS2_QNAME_FREE(qrefname, env);            
        }
        AXIS2_QNAME_FREE(qref, env);
    }
    
    return AXIS2_SUCCESS;
}
