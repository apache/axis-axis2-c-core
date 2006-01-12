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
 
#include <axis2_desc_builder.h>
#include <string.h>
#include <axis2_class_loader.h>

/** 
 * @brief
 * To do the common tasks for all Builder
 */ 
typedef struct axis2_desc_builder_impl
{
	axis2_desc_builder_t desc_builder;
    
    /**
     * Store the full path to configuration file.
     */
    axis2_char_t *file_name;
    	
} axis2_desc_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(desc_builder) \
    ((axis2_desc_builder_impl_t *) desc_builder)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_desc_builder_free (axis2_desc_builder_t *desc_builder, 
                            axis2_env_t **env);

axis2_om_node_t *AXIS2_CALL
axis2_build_OM(axis2_desc_builder_t *desc_builder,
                axis2_env_t **env);


struct axis2_flow *AXIS2_CALL
axis2_desc_builder_process_flow(axis2_desc_builder_t *desc_builder,
                                axis2_env_t **env,
                                struct axis2_om_element *flow_element,
                                axis2_param_container_t *parent,
                                axis2_om_node_t *node);

struct axis2_handler_desc *AXIS2_CALL
axis2_desc_builder_process_handler(axis2_desc_builder_t *desc_builder,
                                    axis2_env_t **env,
                                    axis2_om_node_t *handler_element,
                                    axis2_param_container_t *parent);


axis2_status_t AXIS2_CALL
axis2_desc_builder_process_params(axis2_desc_builder_t *desc_builder,
                                axis2_env_t **env,
                                axis2_om_children_qname_iterator_t *params,
                                axis2_param_container_t *param_container,
                                axis2_param_container_t *parent );


axis2_status_t AXIS2_CALL
axis2_desc_builder_process_op_module_refs(axis2_desc_builder_t *desc_builder,
                          axis2_env_t **env,
                          axis2_om_children_qname_iterator_t *module_refs, 
                          struct axis2_op *op);
                              
struct axis2_msg_recv *AXIS2_CALL
axis2_desc_builder_load_msg_recv(axis2_desc_builder_t *desc_builder,
                                    axis2_env_t **env,
                                    axis2_om_element_t *recv_element);

struct axis2_msg_recv *AXIS2_CALL
axis2_desc_builder_load_default_msg_recv(axis2_desc_builder_t *desc_builder,
                                            axis2_env_t **env);


axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_short_file_name(axis2_desc_builder_t *desc_builder,
                                        axis2_env_t **env,
                                        axis2_char_t *file_name);


axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_value(axis2_desc_builder_t *desc_builder,
                                axis2_env_t **env,
                                axis2_char_t *in);

                                
/************************** End of function prototypes ************************/

axis2_desc_builder_t * AXIS2_CALL 
axis2_desc_builder_create (axis2_env_t **env)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	desc_builder_impl = (axis2_desc_builder_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_desc_builder_impl_t));
	
	
	if(NULL == desc_builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    desc_builder_impl->file_name = NULL;
    desc_builder_impl->desc_builder.engine = NULL;
    
    desc_builder_impl->desc_builder.ops = NULL;
    
	desc_builder_impl->desc_builder.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_desc_builder_ops_t));
	if(NULL == desc_builder_impl->desc_builder.ops)
    {
        axis2_desc_builder_free(&(desc_builder_impl->desc_builder), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	desc_builder_impl->desc_builder.ops->free =  axis2_desc_builder_free;
    desc_builder_impl->desc_builder.ops->build_OM = axis2_build_OM;
    desc_builder_impl->desc_builder.ops->process_flow = 
            axis2_desc_builder_process_flow;
    desc_builder_impl->desc_builder.ops->process_handler = 
            axis2_desc_builder_process_handler;
    desc_builder_impl->desc_builder.ops->process_params = 
            axis2_desc_builder_process_params;
    desc_builder_impl->desc_builder.ops->process_op_module_refs = 
            axis2_desc_builder_process_op_module_refs;
    desc_builder_impl->desc_builder.ops->load_msg_recv = 
            axis2_desc_builder_load_msg_recv;
    desc_builder_impl->desc_builder.ops->load_default_msg_recv = 
            axis2_desc_builder_load_default_msg_recv;
    desc_builder_impl->desc_builder.ops->get_short_file_name = 
            axis2_desc_builder_get_short_file_name;
    desc_builder_impl->desc_builder.ops->get_value = 
            axis2_desc_builder_get_value;  
	
	return &(desc_builder_impl->desc_builder);
}

axis2_desc_builder_t * AXIS2_CALL 
axis2_desc_builder_create_with_file_and_dep_engine (
                                        axis2_env_t **env, 
                                        axis2_char_t *file_name, 
                                        struct axis2_dep_engine *engine)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, file_name, NULL);
    AXIS2_PARAM_CHECK((*env)->error, engine, NULL);
	
	desc_builder_impl = (axis2_desc_builder_impl_t *) 
        axis2_desc_builder_create(env);
	if(NULL == desc_builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    desc_builder_impl->file_name = AXIS2_STRDUP(file_name, env);
    if(!desc_builder_impl->file_name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    desc_builder_impl->desc_builder.engine = engine;
    
	return &(desc_builder_impl->desc_builder);
}

axis2_desc_builder_t * AXIS2_CALL 
axis2_desc_builder_create_with_dep_engine (
                                        axis2_env_t **env,
                                        struct axis2_dep_engine *engine)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, engine, NULL);
	
	desc_builder_impl = (axis2_desc_builder_impl_t *)
        axis2_desc_builder_create(env);
	if(NULL == desc_builder_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    desc_builder_impl->desc_builder.engine = engine;
    
	return &(desc_builder_impl->desc_builder);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_desc_builder_free (axis2_desc_builder_t *desc_builder, 
                            axis2_env_t **env)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(desc_builder, env, AXIS2_FAILURE);
    
    desc_builder_impl = AXIS2_INTF_TO_IMPL(desc_builder);
    
    if(!desc_builder_impl->file_name)
    {
        AXIS2_FREE((*env)->allocator, desc_builder_impl->file_name);
        desc_builder_impl->file_name = NULL;
    }
    desc_builder->engine = NULL;
    
	if(NULL != desc_builder->ops)
        AXIS2_FREE((*env)->allocator, desc_builder->ops);
    
    AXIS2_FREE((*env)->allocator, desc_builder_impl);
    desc_builder_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_om_node_t *AXIS2_CALL
axis2_build_OM(axis2_desc_builder_t *desc_builder,
                axis2_env_t **env) 
{
    struct axis2_desc_builder_impl *builder_impl = NULL;
    struct axis2_om_stax_builder *builder = NULL;
    struct axis2_xml_reader *reader = NULL;
    struct axis2_om_document *document = NULL;
    struct axis2_om_node *root = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(desc_builder, env, NULL);
    
    builder_impl = AXIS2_INTF_TO_IMPL(desc_builder);
    
    if(!builder_impl->file_name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_DESC_BUILDER,
            AXIS2_FAILURE);
        return NULL;
    }
    /** create pull parser using the file path to configuration file */
    reader = axis2_xml_reader_create_for_file(env, builder_impl->file_name,
        NULL);

    if(!reader)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CREATING_XML_STREAM_READER,
            AXIS2_FAILURE)
        return NULL;
    }
    
    /** create axis2_om_stax_builder by parsing pull_parser struct */
    builder = axis2_om_stax_builder_create (env, reader);

    if(!builder)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CREATING_XML_STREAM_READER,
            AXIS2_FAILURE)
        return NULL;
    }
    /**
        create an om document
        document is the container of om model created using builder
    */
    document = axis2_om_document_create (env, NULL, builder);
    /** 
     * In description building we don't want defferred building. So build
     * the whole tree at once
     */
    AXIS2_OM_DOCUMENT_BUILD_ALL(document, env);
    /**
        get root element , building starts hear 
     */
    root = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT (document, env);
    return root;
}

struct axis2_flow *AXIS2_CALL
axis2_desc_builder_process_flow(axis2_desc_builder_t *desc_builder,
                                axis2_env_t **env,
                                axis2_om_element_t *flow_element,
                                axis2_param_container_t *parent,
                                axis2_om_node_t *flow_node)
{
    struct axis2_flow *flow = NULL;
    axis2_om_children_qname_iterator_t *handlers = NULL;
    axis2_qname_t *child = NULL;
        
    AXIS2_FUNC_PARAM_CHECK(desc_builder, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, parent, NULL);
    AXIS2_PARAM_CHECK((*env)->error, flow_node, NULL);
        
    flow = axis2_flow_create(env);
    if(!flow)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    if(NULL == flow_element)
    {
        return flow;
    }
    
    child = axis2_qname_create(env, AXIS2_HANDLERST, NULL, NULL); 
    handlers = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(flow_element, env, 
        child, flow_node);
    while(AXIS2_TRUE == AXIS2_OM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(handlers ,env))
    {
        axis2_om_node_t *handler_node = NULL;
        struct axis2_handler_desc *handler = NULL;
        axis2_status_t status = AXIS2_FAILURE;
            
        handler_node = (axis2_om_node_t *)
            AXIS2_OM_CHILDREN_QNAME_ITERATOR_NEXT(handlers, env);
        
        handler = axis2_desc_builder_process_handler(desc_builder, env, 
            handler_node, parent);
        status = AXIS2_FLOW_ADD_HANDLER(flow, env, handler);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_FLOW_FREE(flow, env);
            return NULL;
        }
    }
    
    return flow;
}

struct axis2_handler_desc *AXIS2_CALL
axis2_desc_builder_process_handler(axis2_desc_builder_t *desc_builder,
                                    axis2_env_t **env,
                                    axis2_om_node_t *handler_node,
                                    struct axis2_param_container *parent)
{
    axis2_handler_desc_t *handler = NULL;
    struct axis2_om_attribute *name_attrib = NULL;
    axis2_qname_t *attr_qname = NULL;
    struct axis2_om_attribute *class_attrib = NULL;
    axis2_qname_t *class_qname = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *attrib_value = NULL;
    axis2_om_element_t *handler_element = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(desc_builder, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, handler_element, NULL);
    AXIS2_PARAM_CHECK((*env)->error, parent, NULL);
    
    handler = axis2_handler_desc_create_with_qname(env, NULL);
    if(!handler)
    {
        return NULL;
    }
    /* Setting Handler name */
    attr_qname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
    if(!attr_qname)
    {
        AXIS2_HANDLER_DESC_FREE(handler, env);
        return NULL;
    }
    handler_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(handler_node, env);
    name_attrib = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(handler_element, env, 
        attr_qname);
    if(NULL == name_attrib)
    {
        AXIS2_HANDLER_DESC_FREE(handler, env);
        AXIS2_QNAME_FREE(attr_qname, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
            AXIS2_FAILURE);
        return NULL;
    }
    else
    {
        axis2_qname_t *handler_qname = NULL;
        axis2_char_t *value = NULL;
        
        value = AXIS2_OM_ATTRIBUTE_GET_VALUE(name_attrib, env);
        handler_qname = axis2_qname_create(env, value, NULL, NULL); 
        if(!handler_qname)
        {
            AXIS2_HANDLER_DESC_FREE(handler, env);
            AXIS2_QNAME_FREE(attr_qname, env);
            return NULL;
        }
        status = AXIS2_HANDLER_DESC_SET_QNAME(handler, env, handler_qname);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_HANDLER_DESC_FREE(handler, env);
            AXIS2_QNAME_FREE(attr_qname, env);
            AXIS2_QNAME_FREE(handler_qname, env);
            return NULL;
        }  
        AXIS2_QNAME_FREE(handler_qname, env);        
    }

    /*Setting Handler Class name */
    
    class_qname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
    class_attrib = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(handler_element, env,
        class_qname);
        
    AXIS2_QNAME_FREE(class_qname, env);
    
    if(NULL == class_attrib)
    {
        AXIS2_HANDLER_DESC_FREE(handler, env);
        AXIS2_QNAME_FREE(attr_qname, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
            AXIS2_FAILURE);
        return NULL;   
    }
    else
    {   
        attrib_value = AXIS2_OM_ATTRIBUTE_GET_VALUE(class_attrib, env);
        status = AXIS2_HANDLER_DESC_SET_CLASS_NAME(handler, env, attrib_value);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_HANDLER_DESC_FREE(handler, env);
            AXIS2_QNAME_FREE(attr_qname, env);
            return NULL;
        }
    }

    /*processing phase Rules (order) */
    axis2_om_node_t *order_node = NULL;
    axis2_om_element_t *order_element = NULL;
    axis2_qname_t *order_qname = NULL;
    
    order_qname = axis2_qname_create(env, AXIS2_ORDER, NULL, NULL);
    order_element = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(handler_element,
        env, order_qname, handler_node, &order_node);
    if(NULL == (&order_element) || NULL == order_node)
    {
        AXIS2_HANDLER_DESC_FREE(handler, env);
        AXIS2_QNAME_FREE(attr_qname, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_HANDLER_STATE, 
            AXIS2_FAILURE);
        return NULL;
    }
    else
    {
        axis2_hash_t *order_itr = NULL;
        axis2_om_children_qname_iterator_t *params = NULL;
        axis2_qname_t *param_qname = NULL;
        axis2_hash_index_t *index_i = NULL;
        
        order_itr = AXIS2_OM_ELEMENT_GET_ALL_ATTRIBUTES(order_element, env);
        if(!order_itr)
        {
            AXIS2_HANDLER_DESC_FREE(handler, env);
            AXIS2_QNAME_FREE(attr_qname, env);
            return NULL;
        }
        
        index_i = axis2_hash_first (order_itr, env);
    
        while(NULL != index_i)
        {
            axis2_om_attribute_t *order_attrib = NULL;
            axis2_qname_t *qname = NULL;
            axis2_char_t *name = NULL;
            axis2_char_t *value = NULL;
            void *v = NULL;

            axis2_hash_this (index_i, NULL, NULL, &v);
            order_attrib = (axis2_om_attribute_t *) v;
            qname = AXIS2_OM_ATTRIBUTE_GET_QNAME(order_attrib, env);
            name = AXIS2_QNAME_GET_LOCALPART(qname, env);
            value = AXIS2_OM_ATTRIBUTE_GET_VALUE(order_attrib, env);
            if(0 == AXIS2_STRCMP(AXIS2_AFTER, name))
            {
                struct axis2_phase_rule *phase_rule = NULL;
                phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler, env);
                status = AXIS2_PHASE_RULE_SET_AFTER(phase_rule, env, value);
                if(AXIS2_FAILURE == status)
                {
                    AXIS2_HANDLER_DESC_FREE(handler, env);
                    AXIS2_QNAME_FREE(attr_qname, env);
                    return NULL;
                }
            }
            if(0 == AXIS2_STRCMP(AXIS2_BEFORE, name))
            {
                struct axis2_phase_rule *phase_rule = NULL;
                phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler, env);
                status = AXIS2_PHASE_RULE_SET_BEFORE(phase_rule, env, value);
                if(AXIS2_FAILURE == status)
                {
                    AXIS2_HANDLER_DESC_FREE(handler, env);
                    AXIS2_QNAME_FREE(attr_qname, env);
                    return NULL;
                }
            }
            if(0 == AXIS2_STRCMP(AXIS2_PHASE, name))
            {
                struct axis2_phase_rule *phase_rule = NULL;
                phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler, env);
                status = AXIS2_PHASE_RULE_SET_NAME(phase_rule, env, value);
                if(AXIS2_FAILURE == status)
                {
                    AXIS2_HANDLER_DESC_FREE(handler, env);
                    AXIS2_QNAME_FREE(attr_qname, env);
                    return NULL;
                }
            }
            if(0 == AXIS2_STRCMP(AXIS2_PHASEFIRST, name))
            {
                axis2_char_t *bool_val = axis2_desc_builder_get_value(
                    desc_builder, env, value);
                
                if(0 == AXIS2_STRCMP(bool_val, "true"))
                {
                    struct axis2_phase_rule *phase_rule = NULL;
                    phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler, env);
                    status = AXIS2_PHASE_RULE_SET_PHASE_FIRST(phase_rule, env,
                        AXIS2_TRUE);
                    if(AXIS2_FAILURE == status)
                    {
                        AXIS2_HANDLER_DESC_FREE(handler, env);
                        AXIS2_QNAME_FREE(attr_qname, env);
                        AXIS2_FREE((*env)->allocator, bool_val);
                        return NULL;
                    }
                }
                else if(0 == AXIS2_STRCMP(bool_val, "false"))
                {
                    struct axis2_phase_rule *phase_rule = NULL;
                    phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler, env);
                    status = AXIS2_PHASE_RULE_SET_PHASE_FIRST(phase_rule, env,
                        AXIS2_FALSE);
                    if(AXIS2_FAILURE == status)
                    {
                        AXIS2_HANDLER_DESC_FREE(handler, env);
                        AXIS2_QNAME_FREE(attr_qname, env);
                        AXIS2_FREE((*env)->allocator, bool_val);
                        return NULL;
                    }
                }
                AXIS2_FREE((*env)->allocator, bool_val);
            }
            index_i = axis2_hash_next (env, index_i);
        }
        
        param_qname = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
        params = AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(handler_element,
            env, param_qname, handler_node);
        status = axis2_desc_builder_process_params(desc_builder, env, params, 
            handler->param_container, parent);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_HANDLER_DESC_FREE(handler, env);
            AXIS2_QNAME_FREE(attr_qname, env);
            AXIS2_QNAME_FREE(param_qname, env);
            return NULL;   
        }      
    }
    
    status = AXIS2_HANDLER_DESC_SET_PARENT(handler, env, parent);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_HANDLER_DESC_FREE(handler, env);
        AXIS2_QNAME_FREE(attr_qname, env);
        return NULL; 
    }
    
    return handler;
}

axis2_status_t AXIS2_CALL
axis2_desc_builder_process_params(axis2_desc_builder_t *desc_builder,
                                axis2_env_t **env,
                                axis2_om_children_qname_iterator_t *params,
                                axis2_param_container_t *param_container,
                                axis2_param_container_t *parent )
{
    axis2_status_t status = AXIS2_FAILURE;
        
    AXIS2_FUNC_PARAM_CHECK(desc_builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, params, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param_container, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, parent, AXIS2_FAILURE);
    
    while(AXIS2_FALSE != AXIS2_OM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(params, env))
    {
        axis2_om_element_t *param_element = NULL;
        axis2_om_node_t *param_node = NULL;
        axis2_om_element_t *para_value = NULL;
        axis2_om_node_t *para_node = NULL;
        axis2_param_t *param = NULL;
        axis2_param_t *parent_para = NULL;
        axis2_om_attribute_t *para_name = NULL;
        axis2_om_attribute_t *para_locked = NULL;  
        axis2_qname_t *att_locked = NULL;
        axis2_qname_t *att_qname = NULL;
        axis2_char_t *pname = NULL;
        
        /* this is to check whether some one has locked the parmter at the top 
         * level
         */
        param_node = (axis2_om_node_t *)
            AXIS2_OM_CHILDREN_QNAME_ITERATOR_NEXT(params, env);
        param_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(param_node, env);
        param = axis2_param_create(env, NULL, NULL);
        /* setting param_element */
        status = AXIS2_PARAM_SET_ELEMENT(param, env, param_node);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_PARAM_FREE(param, env);
            return status;
        }
        /* setting paramter Name */
        att_qname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        para_name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(param_element, env, 
            att_qname);
        AXIS2_QNAME_FREE(att_qname, env);
        if(!para_name)
        {
            printf("para_name is null\n");
            AXIS2_PARAM_FREE(param, env);
            return AXIS2_FAILURE;
        }
        pname = AXIS2_OM_ATTRIBUTE_GET_VALUE(para_name, env);
        printf("pname:%s\n", pname);
        status = AXIS2_PARAM_SET_NAME(param, env, pname);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_PARAM_FREE(param, env);
            return AXIS2_FAILURE;
        }
        
        /* setting paramter Value (the chiled elemnt of the paramter) */
        para_value = AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT(param_element, env,
            param_node, &para_node);
        if(NULL != para_value)
        {
            status = AXIS2_PARAM_SET_VALUE(param, env, param_element);
            if(AXIS2_FAILURE == status)
            {
                AXIS2_PARAM_FREE(param, env);
                return AXIS2_FAILURE;
            }
            AXIS2_PARAM_SET_PARAM_TYPE(param, env, AXIS2_DOM_PARAM);
        }
        else
        {
            axis2_char_t *para_test_value = AXIS2_OM_ELEMENT_GET_TEXT(
                param_element, env, param_node);
            status = AXIS2_PARAM_SET_VALUE(param, env, para_test_value);
            if(AXIS2_FAILURE == status)
            {
                AXIS2_PARAM_FREE(param, env);
                return AXIS2_FAILURE;
            }
            AXIS2_PARAM_SET_PARAM_TYPE(param, env, AXIS2_TEXT_PARAM);
        }
        /* setting locking attrib */
        att_locked = axis2_qname_create(env, AXIS2_ATTLOCKED, NULL, NULL);
        para_locked = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(param_element, env, 
            att_locked);
        if(NULL != parent)
        {
            parent_para = AXIS2_PARAM_CONTAINER_GET_PARAM(parent, env, 
                AXIS2_PARAM_GET_NAME(param, env));
            
        }
        if(NULL != para_locked)
        {
            axis2_char_t *locked_value = NULL;
            
            locked_value = AXIS2_OM_ATTRIBUTE_GET_VALUE(para_locked, env);
            if(0 == AXIS2_STRCMP("true", locked_value))
            {
                /*if the parameter is locked at some levle paramer value replace
                 * by that   
                 */
                if(NULL != parent && AXIS2_TRUE == 
                        AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(parent, env, 
                            AXIS2_PARAM_GET_NAME(param, env)))
                {
                    AXIS2_PARAM_FREE(param, env);
                    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CONF_NOT_FOUND,
                        AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
                else
                {
                    AXIS2_PARAM_SET_LOCKED(param, env, AXIS2_TRUE);
                }
            }
            else
            {
                AXIS2_PARAM_SET_LOCKED(param, env, AXIS2_FALSE);
            }
        }
        if(NULL != parent)
        {
            if(NULL != parent_para || AXIS2_FALSE == 
                    AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(parent, env, 
                        AXIS2_PARAM_GET_NAME(param, env)))
            {
                status = AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container, env, 
                    param);
            }
        }
        else
        {
            status = AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container, env, 
                param);   
        }   
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_desc_builder_process_op_module_refs(axis2_desc_builder_t *desc_builder,
                          axis2_env_t **env,
                          struct axis2_om_children_qname_iterator *module_refs, 
                          struct axis2_op *op)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;
    struct axis2_om_element *moduleref = NULL;
    struct axis2_om_attribute *module_ref_attrib = NULL;
    axis2_qname_t *ref = NULL;
    axis2_status_t status = AXIS2_FAILURE;
        
    AXIS2_FUNC_PARAM_CHECK(desc_builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_refs, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op, AXIS2_FAILURE);
    
    desc_builder_impl = AXIS2_INTF_TO_IMPL(desc_builder);
    
    while(AXIS2_TRUE == AXIS2_OM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(module_refs,
        env))
    {
        moduleref = (struct axis2_om_element *)
            AXIS2_OM_CHILDREN_QNAME_ITERATOR_NEXT(module_refs, env);
        ref = axis2_qname_create(env, AXIS2_REF, NULL, NULL);
        module_ref_attrib = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(moduleref, env, ref);
        if(NULL != module_ref_attrib)
        {
            axis2_char_t *ref_name = NULL;
            axis2_qname_t *ref_qname = NULL;
            
            ref_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(module_ref_attrib, env);
            ref_qname = axis2_qname_create(env, ref_name, NULL, NULL);
            if( NULL == AXIS2_DEP_ENGINE_GET_MODULE(desc_builder->engine, env,
                ref_qname))
            {
                AXIS2_QNAME_FREE(ref_qname, env);
                AXIS2_QNAME_FREE(ref, env);
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MODULE_NOT_FOUND, 
                    AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            else
            {
                status = AXIS2_OP_ADD_MODULE(op, env, ref_qname);
                if(AXIS2_FAILURE == status)
                {
                    AXIS2_QNAME_FREE(ref_qname, env);
                    AXIS2_QNAME_FREE(ref, env);
                    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MODULE_NOT_FOUND, 
                        AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }  
        }
    }
    return AXIS2_SUCCESS;
}

struct axis2_msg_recv *AXIS2_CALL
axis2_desc_builder_load_msg_recv(axis2_desc_builder_t *desc_builder,
                                    axis2_env_t **env,
                                    struct axis2_om_element *recv_element)
{
    struct axis2_om_attribute *recv_name = NULL;
    axis2_char_t *class_name = NULL;
    axis2_msg_recv_t *msg_recv = NULL;
    axis2_qname_t *class_qname = NULL;
    axis2_param_t *impl_info_param = NULL;
    struct axis2_dll_desc *dll_desc = NULL;
     
    class_qname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
    recv_name = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(recv_element, env, class_qname);
    class_name = AXIS2_OM_ATTRIBUTE_GET_VALUE(recv_name, env);
    dll_desc = axis2_dll_desc_create(env);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, env, class_name);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, AXIS2_MSG_RECV_DLL);
    axis2_class_loader_init(env);
    impl_info_param = axis2_param_create(env, NULL, NULL);
    
    AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc); 
    msg_recv = (axis2_msg_recv_t *) axis2_class_loader_create_dll(env, 
        impl_info_param);
    
    return msg_recv;
}

struct axis2_msg_recv *AXIS2_CALL
axis2_desc_builder_load_default_msg_recv(axis2_desc_builder_t *desc_builder,
                                            axis2_env_t **env)
{
    axis2_char_t *default_msg_recv = "axis2_raw_xml_in_out_msg_recv";
    axis2_msg_recv_t *msg_recv = NULL;
    axis2_param_t *impl_info_param = NULL;
    struct axis2_dll_desc *dll_desc = NULL;
    
    /**
     * Setting default Message Recive as Message Receiver
     */
    dll_desc = axis2_dll_desc_create(env);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, env, default_msg_recv);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, AXIS2_MSG_RECV_DLL);
    axis2_class_loader_init(env);
    impl_info_param = axis2_param_create(env, NULL, NULL);
    
    AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc); 
    msg_recv = (axis2_msg_recv_t *) axis2_class_loader_create_dll(env, 
        impl_info_param);
    
    return msg_recv;
}

axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_short_file_name(axis2_desc_builder_t *desc_builder,
                                        axis2_env_t **env,
                                        axis2_char_t *file_name) 
{
    axis2_char_t *separator = NULL;
    axis2_char_t *value = NULL;
    axis2_char_t *file_name_l = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(desc_builder, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, file_name, NULL);
    
    file_name_l = AXIS2_STRDUP(file_name, env);
    if(!file_name_l)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    while(NULL == value)
    {
        value = strpbrk(file_name_l, separator);
    }
    value[0] = AXIS2_EOLN;
    value = file_name_l;
    
    return value;
}

axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_value(axis2_desc_builder_t *desc_builder,
                                axis2_env_t **env,
                                axis2_char_t *in) 
{
    axis2_char_t *separator = ":";
    axis2_char_t *value = NULL;
    axis2_char_t *in_l = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(desc_builder, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, in, NULL);
    
    in_l = AXIS2_STRDUP(in, env);
    if(!in_l)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    value = strpbrk(in_l, separator);
    value = value + 1;
    
    return value;
}
