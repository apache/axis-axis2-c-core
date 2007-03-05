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

#include <axis2_desc_builder.h>
#include <axis2_string.h>
#include <axis2_class_loader.h>
#include <axis2_utils.h>
#include <axis2_generic_obj.h>
#include <axis2_raw_xml_in_out_msg_recv.h>

/**
 * To do the common tasks for all Builder
 */
typedef struct axis2_desc_builder_impl
{
    axis2_desc_builder_t desc_builder;

    /**
     * Store the full path to configuration file.
     */
    axis2_char_t *file_name;
    axiom_stax_builder_t *builder;

}
axis2_desc_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(desc_builder) \
    ((axis2_desc_builder_impl_t *) desc_builder)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_desc_builder_free(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env);

axiom_node_t *AXIS2_CALL
axis2_build_OM(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env);

struct axis2_flow *AXIS2_CALL
            axis2_desc_builder_process_flow(
                axis2_desc_builder_t *desc_builder,
                const axis2_env_t *env,
                struct axiom_element *flow_element,
                axis2_param_container_t *parent,
                axiom_node_t *node);

struct axis2_handler_desc *AXIS2_CALL
            axis2_desc_builder_process_handler(
                axis2_desc_builder_t *desc_builder,
                const axis2_env_t *env,
                axiom_node_t *handler_element,
                axis2_param_container_t *parent);

axis2_status_t AXIS2_CALL
axis2_desc_builder_process_params(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *params,
    axis2_param_container_t *param_container,
    axis2_param_container_t *parent);

axis2_status_t AXIS2_CALL
axis2_desc_builder_process_op_module_refs(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *module_refs,
    struct axis2_op *op);

struct axis2_msg_recv *AXIS2_CALL
            axis2_desc_builder_load_msg_recv(
                axis2_desc_builder_t *desc_builder,
                const axis2_env_t *env,
                axiom_element_t *recv_element);

axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_short_file_name(
    const axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axis2_char_t *file_name);

axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_file_name_without_prefix(
    const axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axis2_char_t *short_file_name);

axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_value(
    const axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axis2_char_t *in);

/**
 * Populate the Axis2 Operation with details from the actionMapping,
 * outputActionMapping and faultActionMapping elements from the operation
 * element.
 *
 * @param operation
 * @param op_desc
 */
axis2_status_t AXIS2_CALL
axis2_desc_builder_process_action_mappings(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axiom_node_t *op_node,
    axis2_op_t *op_desc);

static axis2_status_t
set_attrs_and_value(
    axis2_param_t *param,
    const axis2_env_t *env,
    axiom_element_t *param_element,
    axiom_node_t *param_node);


/************************** End of function prototypes ************************/

axis2_desc_builder_t *AXIS2_CALL
axis2_desc_builder_create(
    const axis2_env_t *env)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    desc_builder_impl = (axis2_desc_builder_impl_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_desc_builder_impl_t));


    if (NULL == desc_builder_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    desc_builder_impl->file_name = NULL;
    desc_builder_impl->desc_builder.engine = NULL;
    desc_builder_impl->builder = NULL;
    desc_builder_impl->desc_builder.ops = NULL;

    desc_builder_impl->desc_builder.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_desc_builder_ops_t));
    if (NULL == desc_builder_impl->desc_builder.ops)
    {
        axis2_desc_builder_free(&(desc_builder_impl->desc_builder), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
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
    desc_builder_impl->desc_builder.ops->get_short_file_name =
        axis2_desc_builder_get_short_file_name;
    desc_builder_impl->desc_builder.ops->get_file_name_without_prefix =
        axis2_desc_builder_get_file_name_without_prefix;
    desc_builder_impl->desc_builder.ops->get_value =
        axis2_desc_builder_get_value;
    desc_builder_impl->desc_builder.ops->process_action_mappings =
    axis2_desc_builder_process_action_mappings;

    return &(desc_builder_impl->desc_builder);
}

axis2_desc_builder_t *AXIS2_CALL
axis2_desc_builder_create_with_file_and_dep_engine(
    const axis2_env_t *env,
    axis2_char_t *file_name,
    axis2_dep_engine_t *engine)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, file_name, NULL);
    AXIS2_PARAM_CHECK(env->error, engine, NULL);

    desc_builder_impl = (axis2_desc_builder_impl_t *)
            axis2_desc_builder_create(env);
    if (NULL == desc_builder_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    desc_builder_impl->file_name = AXIS2_STRDUP(file_name, env);
    if (!desc_builder_impl->file_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    desc_builder_impl->desc_builder.engine = engine;

    return &(desc_builder_impl->desc_builder);
}

axis2_desc_builder_t *AXIS2_CALL
axis2_desc_builder_create_with_dep_engine(
    const axis2_env_t *env,
    struct axis2_dep_engine *engine)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, engine, NULL);

    desc_builder_impl = (axis2_desc_builder_impl_t *)
            axis2_desc_builder_create(env);
    if (NULL == desc_builder_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    desc_builder_impl->desc_builder.engine = engine;

    return &(desc_builder_impl->desc_builder);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_desc_builder_free(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    desc_builder_impl = AXIS2_INTF_TO_IMPL(desc_builder);

    if (desc_builder_impl->file_name)
    {
        AXIS2_FREE(env->allocator, desc_builder_impl->file_name);
        desc_builder_impl->file_name = NULL;
    }

    if (desc_builder_impl->builder)
    {
        AXIOM_STAX_BUILDER_FREE(desc_builder_impl->builder, env);
        desc_builder_impl->builder = NULL;
    }

    /* we cannot free deployment engine here */
    desc_builder->engine = NULL;

    if (desc_builder->ops)
        AXIS2_FREE(env->allocator, desc_builder->ops);

    if (desc_builder_impl)
    {
        AXIS2_FREE(env->allocator, desc_builder_impl);
        desc_builder_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axiom_node_t *AXIS2_CALL
axis2_build_OM(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;
    axiom_xml_reader_t *reader = NULL;
    axiom_document_t *document = NULL;
    axiom_node_t *root = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    desc_builder_impl = AXIS2_INTF_TO_IMPL(desc_builder);

    if (!desc_builder_impl->file_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_DESC_BUILDER,
                AXIS2_FAILURE);
        return NULL;
    }
    /** create pull parser using the file path to configuration file */
    reader = axiom_xml_reader_create_for_file(env, desc_builder_impl->file_name,
            NULL);

    if (!reader)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_CREATING_XML_STREAM_READER,
                AXIS2_FAILURE)
        return NULL;
    }

    /** create axiom_stax_builder by parsing pull_parser struct */
    desc_builder_impl->builder = axiom_stax_builder_create(env, reader);

    if (!(desc_builder_impl->builder))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_CREATING_XML_STREAM_READER,
                AXIS2_FAILURE)
        return NULL;
    }
    /**
        get the om document form builder 
        document is the container of om model created using builder
    */
    document = AXIOM_STAX_BUILDER_GET_DOCUMENT(desc_builder_impl->builder, env);
    /**
     * In description building we don't want defferred building. So build
     * the whole tree at once
     */
    AXIOM_DOCUMENT_BUILD_ALL(document, env);
    /**
        get root element , building starts hear 
     */
    root = AXIOM_DOCUMENT_GET_ROOT_ELEMENT(document, env);
    return root;
}

axis2_flow_t *AXIS2_CALL
axis2_desc_builder_process_flow(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axiom_element_t *flow_element,
    axis2_param_container_t *parent,
    axiom_node_t *flow_node)
{
    axis2_flow_t *flow = NULL;
    axiom_children_qname_iterator_t *handlers = NULL;
    axis2_qname_t *qchild = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, parent, NULL);
    AXIS2_PARAM_CHECK(env->error, flow_node, NULL);

    flow = axis2_flow_create(env);
    if (!flow)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    if (!flow_element)
    {
        return NULL;
    }

    qchild = axis2_qname_create(env, AXIS2_HANDLERST, NULL, NULL);
    handlers = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(flow_element, env,
            qchild, flow_node);
    if (qchild)
        AXIS2_QNAME_FREE(qchild, env);

    while (AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(handlers , env))
    {
        axiom_node_t *handler_node = NULL;
        axis2_handler_desc_t *handler_desc = NULL;
        axis2_status_t status = AXIS2_FAILURE;

        handler_node = (axiom_node_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(handlers, env);

        handler_desc = axis2_desc_builder_process_handler(desc_builder, env,
                handler_node, parent);
        status = AXIS2_FLOW_ADD_HANDLER(flow, env, handler_desc);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_FLOW_FREE(flow, env);
            return NULL;
        }
    }

    return flow;
}

struct axis2_handler_desc *AXIS2_CALL
            axis2_desc_builder_process_handler(
                axis2_desc_builder_t *desc_builder,
                const axis2_env_t *env,
                axiom_node_t *handler_node,
                struct axis2_param_container *parent)
{
    axis2_handler_desc_t *handler_desc = NULL;
    axiom_attribute_t *name_attrib = NULL;
    axis2_qname_t *attr_qname = NULL;
    axiom_attribute_t *class_attrib = NULL;
    axis2_qname_t *class_qname = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *attrib_value = NULL;
    axiom_element_t *handler_element = NULL;
    axiom_node_t *order_node = NULL;
    axiom_element_t *order_element = NULL;
    axis2_qname_t *order_qname = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, handler_node, NULL);
    AXIS2_PARAM_CHECK(env->error, parent, NULL);

    handler_desc = axis2_handler_desc_create(env, NULL);
    if (!handler_desc)
    {
        return NULL;
    }
    /* Setting Handler name */
    attr_qname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
    handler_element = AXIOM_NODE_GET_DATA_ELEMENT(handler_node, env);
    name_attrib = AXIOM_ELEMENT_GET_ATTRIBUTE(handler_element, env,
            attr_qname);
    if (attr_qname)
    {
        AXIS2_QNAME_FREE(attr_qname, env);
    }

    if (!name_attrib)
    {
        AXIS2_HANDLER_DESC_FREE(handler_desc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                AXIS2_FAILURE);
        return NULL;
    }
    else
    {
        axis2_string_t *handler_name = NULL;
        axis2_char_t *value = NULL;

        value = AXIOM_ATTRIBUTE_GET_VALUE(name_attrib, env);
        handler_name = axis2_string_create(env, value);
        status = AXIS2_HANDLER_DESC_SET_NAME(handler_desc, env, handler_name);
        if (handler_name)
            axis2_string_free(handler_name, env);

        if (AXIS2_SUCCESS != status)
        {
            AXIS2_HANDLER_DESC_FREE(handler_desc, env);
            return NULL;
        }
    }

    /*Setting Handler Class name */
    class_qname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
    class_attrib = AXIOM_ELEMENT_GET_ATTRIBUTE(handler_element, env,
            class_qname);
    if (class_qname)
        AXIS2_QNAME_FREE(class_qname, env);

    if (NULL == class_attrib)
    {
        AXIS2_HANDLER_DESC_FREE(handler_desc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                AXIS2_FAILURE);
        return NULL;
    }
    else
    {
        attrib_value = AXIOM_ATTRIBUTE_GET_VALUE(class_attrib, env);
        status = AXIS2_HANDLER_DESC_SET_CLASS_NAME(handler_desc, env, attrib_value);
        /*if(AXIS2_FAILURE == status)
        {
            AXIS2_HANDLER_DESC_FREE(handler, env);
            return NULL;
        }*/
    }

    /*processing phase Rules (order) */

    order_qname = axis2_qname_create(env, AXIS2_ORDER, NULL, NULL);
    order_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(handler_element,
            env, order_qname, handler_node, &order_node);
    if (order_qname)
        AXIS2_QNAME_FREE(order_qname, env);

    if (NULL == order_element || NULL == order_node)
    {
        AXIS2_HANDLER_DESC_FREE(handler_desc, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                AXIS2_FAILURE);
        return NULL;
    }
    else
    {
        axis2_hash_t *order_itr = NULL;
        axiom_children_qname_iterator_t *params = NULL;
        axis2_qname_t *param_qname = NULL;
        axis2_hash_index_t *index_i = NULL;

        order_itr = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(order_element, env);
        if (!order_itr)
        {
            AXIS2_HANDLER_DESC_FREE(handler_desc, env);
            return NULL;
        }

        index_i = axis2_hash_first(order_itr, env);

        while (index_i)
        {
            axiom_attribute_t *order_attrib = NULL;
            axis2_qname_t *qname = NULL;
            axis2_char_t *name = NULL;
            axis2_char_t *value = NULL;
            void *v = NULL;

            axis2_hash_this(index_i, NULL, NULL, &v);
            order_attrib = (axiom_attribute_t *) v;
            qname = AXIOM_ATTRIBUTE_GET_QNAME(order_attrib, env);
            name = AXIS2_QNAME_GET_LOCALPART(qname, env);

            value = AXIOM_ATTRIBUTE_GET_VALUE(order_attrib, env);
            if (0 == AXIS2_STRCMP(AXIS2_AFTER, name))
            {
                struct axis2_phase_rule *phase_rule = NULL;

                phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
                status = AXIS2_PHASE_RULE_SET_AFTER(phase_rule, env, value);
                if (AXIS2_SUCCESS != status)
                {
                    AXIS2_HANDLER_DESC_FREE(handler_desc, env);
                    return NULL;
                }
            }
            if (0 == AXIS2_STRCMP(AXIS2_BEFORE, name))
            {
                struct axis2_phase_rule *phase_rule = NULL;
                phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
                status = AXIS2_PHASE_RULE_SET_BEFORE(phase_rule, env, value);
                if (AXIS2_SUCCESS != status)
                {
                    AXIS2_HANDLER_DESC_FREE(handler_desc, env);
                    return NULL;
                }
            }
            if (0 == AXIS2_STRCMP(AXIS2_PHASE, name))
            {
                struct axis2_phase_rule *phase_rule = NULL;
                phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
                status = AXIS2_PHASE_RULE_SET_NAME(phase_rule, env, value);
                if (AXIS2_SUCCESS != status)
                {
                    AXIS2_HANDLER_DESC_FREE(handler_desc, env);
                    return NULL;
                }
            }
            if (0 == AXIS2_STRCMP(AXIS2_PHASEFIRST, name))
            {
                axis2_char_t *bool_val = NULL;

                bool_val = axis2_desc_builder_get_value(desc_builder, env,
                        value);
                if (0 == AXIS2_STRCMP(bool_val, "true"))
                {
                    struct axis2_phase_rule *phase_rule = NULL;
                    phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
                    status = AXIS2_PHASE_RULE_SET_FIRST(phase_rule, env,
                            AXIS2_TRUE);
                    if (AXIS2_SUCCESS != status)
                    {
                        AXIS2_HANDLER_DESC_FREE(handler_desc, env);
                        AXIS2_FREE(env->allocator, bool_val);
                        return NULL;
                    }
                }
                else if (0 == AXIS2_STRCMP(bool_val, "false"))
                {
                    struct axis2_phase_rule *phase_rule = NULL;
                    phase_rule = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
                    status = AXIS2_PHASE_RULE_SET_FIRST(phase_rule, env,
                            AXIS2_FALSE);
                    if (AXIS2_SUCCESS != status)
                    {
                        AXIS2_HANDLER_DESC_FREE(handler_desc, env);
                        AXIS2_FREE(env->allocator, bool_val);
                        return NULL;
                    }
                }
                AXIS2_FREE(env->allocator, bool_val);
            }
            index_i = axis2_hash_next(env, index_i);
        }

        param_qname = axis2_qname_create(env, AXIS2_PARAMETERST, NULL, NULL);
        params = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(handler_element,
                env, param_qname, handler_node);
        AXIS2_QNAME_FREE(param_qname, env);
        status = axis2_desc_builder_process_params(desc_builder, env, params,
                axis2_handler_desc_get_param_container(handler_desc, env), parent);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_HANDLER_DESC_FREE(handler_desc, env);
            return NULL;
        }
    }

    status = AXIS2_HANDLER_DESC_SET_PARENT(handler_desc, env, parent);
    if (AXIS2_SUCCESS != status)
    {
        AXIS2_HANDLER_DESC_FREE(handler_desc, env);
        return NULL;
    }

    return handler_desc;
}

static axis2_status_t
set_attrs_and_value(
    axis2_param_t *param,
    const axis2_env_t *env,
    axiom_element_t *param_element,
    axiom_node_t *param_node)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_hash_t *attrs = NULL;
    axiom_child_element_iterator_t *childs = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param_element, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param_node, AXIS2_FAILURE);

    /* Setting attributes */
    attrs = AXIOM_ELEMENT_EXTRACT_ATTRIBUTES(param_element, env, param_node);
    if (attrs)
    {
        axis2_hash_index_t *i = NULL;

        for (i = axis2_hash_first(attrs, env); i; i =
                    axis2_hash_next(env, i))
        {
            void *v = NULL;
            axiom_attribute_t *value = NULL;
            axis2_generic_obj_t *obj = NULL;
            axis2_qname_t *attr_qname = NULL;
            axis2_char_t *attr_name = NULL;

            axis2_hash_this(i, NULL, NULL, &v);
            if (!v)
            {
                AXIS2_PARAM_FREE(param, env);
                return AXIS2_FAILURE;
            }
            obj = axis2_generic_obj_create(env);
            if (!obj)
            {
                AXIS2_PARAM_FREE(param, env);
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
                        AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            value = (axiom_attribute_t *) v;
            AXIS2_GENERIC_OBJ_SET_VALUE(obj, env, value);
            AXIS2_GENERIC_OBJ_SET_FREE_FUNC(obj, env,
                    axiom_attribute_free_void_arg);
            attr_qname = AXIOM_ATTRIBUTE_GET_QNAME(value, env);
            attr_name = AXIS2_QNAME_TO_STRING(attr_qname, env);
            axis2_hash_set(attrs, attr_name, AXIS2_HASH_KEY_STRING, obj);
        }
        AXIS2_PARAM_SET_ATTRIBUTES(param, env, attrs);
    }

    childs = AXIOM_ELEMENT_GET_CHILD_ELEMENTS(param_element, env, param_node);
    if (childs)
    {
        axis2_array_list_t *value_list = NULL;

        value_list = axis2_array_list_create(env, 0);
        AXIS2_PARAM_SET_VALUE_LIST(param, env, value_list);

        while (AXIS2_TRUE == AXIOM_CHILD_ELEMENT_ITERATOR_HAS_NEXT(childs, env))
        {
            axiom_node_t *node = NULL;
            axiom_element_t *element = NULL;
            axis2_param_t *param = NULL;
            axis2_char_t *pname = NULL;

            node = AXIOM_CHILD_ELEMENT_ITERATOR_NEXT(childs, env);
            element = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
            param = axis2_param_create(env, NULL, NULL);
            pname = AXIOM_ELEMENT_GET_LOCALNAME(element, env);
            status = AXIS2_PARAM_SET_NAME(param, env, pname);
            if (AXIS2_SUCCESS != status)
            {
                AXIS2_PARAM_FREE(param, env);
                return status;
            }
            AXIS2_PARAM_SET_PARAM_TYPE(param, env, AXIS2_DOM_PARAM);
            set_attrs_and_value(param, env, element, node);
            AXIS2_ARRAY_LIST_ADD(value_list, env, param);
        }
    }
    else
    {
        axis2_char_t *para_test_value = NULL;
        axis2_char_t *temp = NULL;

        temp = AXIOM_ELEMENT_GET_TEXT(
                    param_element, env, param_node);
        para_test_value = AXIS2_STRDUP(temp, env);
        status = AXIS2_PARAM_SET_VALUE(param, env, para_test_value);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_PARAM_FREE(param, env);
            AXIS2_FREE(env->allocator, para_test_value);
            return status;
        }
        AXIS2_PARAM_SET_PARAM_TYPE(param, env, AXIS2_TEXT_PARAM);
    }
    return AXIS2_SUCCESS;
}

/**
 * Populate the Axis2 Operation with details from the actionMapping,
 * outputActionMapping and faultActionMapping elements from the operation
 * element.
 *
 * @param operation
 * @param op_desc
 */
axis2_status_t AXIS2_CALL
axis2_desc_builder_process_action_mappings(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axiom_node_t *op_node,
    axis2_op_t *op_desc)
{
    axiom_element_t *op_element = NULL;
    axis2_qname_t *qname = NULL;
    axiom_children_qname_iterator_t *action_mappings = NULL;
    axis2_array_list_t *mapping_list = axis2_array_list_create(env, 0);
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op_desc, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op_desc, AXIS2_FAILURE);

    op_element = AXIOM_NODE_GET_DATA_ELEMENT(op_node, env);
    qname = axis2_qname_create(env, AXIS2_ACTION_MAPPING, NULL, NULL);
    if(op_element)
        action_mappings = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(op_element, env, 
            qname,op_node);
            
    AXIS2_QNAME_FREE(qname, env);
    qname = NULL;
    if(!action_mappings)
    {
        if (mapping_list)
        {
            AXIS2_ARRAY_LIST_FREE(mapping_list, env);
            mapping_list = NULL;
        }
        return AXIS2_SUCCESS;
    }
    while (AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(action_mappings, env))
    {
        axiom_element_t *mapping_element = NULL;
        axiom_node_t *mapping_node = NULL;
        axis2_char_t *input_action_string = NULL;
        axis2_char_t *temp_str = NULL;

        /* This is to check whether some one has locked the parmter at the top
         * level
         */
        mapping_node = (axiom_node_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(action_mappings, env);
        mapping_element = AXIOM_NODE_GET_DATA_ELEMENT(mapping_node, env);
        temp_str = AXIOM_ELEMENT_GET_TEXT(mapping_element, env, 
            mapping_node);
        input_action_string = AXIS2_STRTRIM(env, temp_str, NULL);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Input action mapping found %s", 
            input_action_string);
        if(0 != AXIS2_STRCMP("", input_action_string))
        {
            AXIS2_ARRAY_LIST_ADD(mapping_list, env, input_action_string);
        }
        else
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Zero length " \
                "input_action_string found. Not added to the mapping list");
    }
    AXIS2_OP_SET_WSAMAPPING_LIST(op_desc, env, mapping_list); 
    /*qname = axis2_qname_create(env, AXIS2_OUTPUT_ACTION_MAPPING, NULL, NULL);
    child_element = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(mapping_element, 
        env, qname, mapping_node, &child_node);
    temp_str = AXIOM_ELEMENT_GET_TEXT(child_element, env, child_node);
    output_action_string = AXIS2_STRTRIM(env, temp_str, NULL);
    if(child_element && output_action_string)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Output action mapping found %s", 
            output_action_string);
    }
    AXIS2_OP_SET_OUTPUT_ACTION(op_desc, env, mapping_list); */
        
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_desc_builder_process_params(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *params,
    axis2_param_container_t *param_container,
    axis2_param_container_t *parent)
{
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, params, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param_container, AXIS2_FAILURE);
    /*AXIS2_PARAM_CHECK(env->error, parent, AXIS2_FAILURE);*/

    while (AXIS2_FALSE != AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(params, env))
    {
        axiom_element_t *param_element = NULL;
        axiom_node_t *param_node = NULL;
        axis2_param_t *param = NULL;
        axis2_param_t *parent_para = NULL;
        axiom_attribute_t *para_name = NULL;
        axiom_attribute_t *para_locked = NULL;
        axis2_qname_t *att_locked = NULL;
        axis2_qname_t *att_qname = NULL;
        axis2_char_t *pname = NULL;

        /* This is to check whether some one has locked the parmter at the top
         * level
         */
        param_node = (axiom_node_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(params, env);
        param_element = AXIOM_NODE_GET_DATA_ELEMENT(param_node, env);
        param = axis2_param_create(env, NULL, NULL);
        /* TODO Setting param_element. Do not set element like following.
         * break it and set 
         */
        /*status = AXIS2_PARAM_SET_ELEMENT(param, env, param_node);
        if(AXIS2_SUCCESS != status)
        {
            AXIS2_PARAM_FREE(param, env);
            return status;
        }*/

        /* Setting paramter name */
        att_qname = axis2_qname_create(env, AXIS2_ATTNAME, NULL, NULL);
        para_name = AXIOM_ELEMENT_GET_ATTRIBUTE(param_element, env,
                att_qname);
        AXIS2_QNAME_FREE(att_qname, env);
        if (!para_name)
        {
            AXIS2_PARAM_FREE(param, env);
            return AXIS2_FAILURE;
        }
        pname = AXIOM_ATTRIBUTE_GET_VALUE(para_name, env);
        status = AXIS2_PARAM_SET_NAME(param, env, pname);
        if (AXIS2_SUCCESS != status)
        {
            AXIS2_PARAM_FREE(param, env);
            return status;
        }

        /* Setting paramter Value (the chiled elemnt of the paramter) */
        set_attrs_and_value(param, env, param_element, param_node);

        /* Setting locking attrib */
        att_locked = axis2_qname_create(env, AXIS2_ATTLOCKED, NULL, NULL);
        para_locked = AXIOM_ELEMENT_GET_ATTRIBUTE(param_element, env,
                att_locked);
        AXIS2_QNAME_FREE(att_locked, env);
        if (parent)
        {
            axis2_char_t *param_name = NULL;

            param_name = AXIS2_PARAM_GET_NAME(param, env);
            parent_para = AXIS2_PARAM_CONTAINER_GET_PARAM(parent, env,
                    param_name);
        }
        if (para_locked)
        {
            axis2_char_t *locked_value = NULL;
            locked_value = AXIOM_ATTRIBUTE_GET_VALUE(para_locked, env);
            if (0 == AXIS2_STRCMP("true", locked_value))
            {
                axis2_char_t *param_name = NULL;
                axis2_bool_t is_param_locked = AXIS2_FALSE;
                /*if the parameter is locked at some levle paramer value replace
                 * by that   
                 */
                param_name = AXIS2_PARAM_GET_NAME(param, env);
                is_param_locked = AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(parent,
                        env, param_name);
                if (parent && AXIS2_TRUE == is_param_locked)
                {
                    AXIS2_PARAM_FREE(param, env);
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_CONFIG_NOT_FOUND, AXIS2_FAILURE);
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


        if (parent)
        {
            axis2_char_t *name = NULL;
            axis2_bool_t bvalue = AXIS2_FALSE;

            name = AXIS2_PARAM_GET_NAME(param, env);
            bvalue = AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(parent, env, name);
            if (parent_para || AXIS2_FALSE == bvalue)
            {
                status = AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container, env,
                        param);
                if (AXIS2_SUCCESS != status)
                {
                    AXIS2_PARAM_FREE(param, env);
                    return status;
                }
            }
        }
        else
        {
            status = AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container, env,
                    param);
            if (AXIS2_SUCCESS != status)
            {
                AXIS2_PARAM_FREE(param, env);
                return status;
            }
        }
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_desc_builder_process_op_module_refs(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axiom_children_qname_iterator_t *module_refs,
    axis2_op_t *op)
{
    axis2_desc_builder_impl_t *desc_builder_impl = NULL;
    axiom_element_t *moduleref = NULL;
    axiom_attribute_t *module_ref_attrib = NULL;
    axis2_qname_t *qref = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
    desc_builder_impl = AXIS2_INTF_TO_IMPL(desc_builder);

    while (module_refs && AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(module_refs,
            env))
    {
        moduleref = (axiom_element_t *)
                AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(module_refs, env);
        qref = axis2_qname_create(env, AXIS2_REF, NULL, NULL);
        module_ref_attrib = AXIOM_ELEMENT_GET_ATTRIBUTE(moduleref, env, qref);
        AXIS2_QNAME_FREE(qref, env);
        if (module_ref_attrib)
        {
            axis2_char_t *ref_name = NULL;
            axis2_qname_t *ref_qname = NULL;
            axis2_module_desc_t *module_desc = NULL;

            ref_name = AXIOM_ATTRIBUTE_GET_VALUE(module_ref_attrib, env);
            ref_qname = axis2_qname_create(env, ref_name, NULL, NULL);
            module_desc = AXIS2_DEP_ENGINE_GET_MODULE(desc_builder->engine, env,
                    ref_qname);
            if (NULL == module_desc)
            {
                AXIS2_QNAME_FREE(ref_qname, env);
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MODULE_NOT_FOUND,
                        AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            else
            {
                status = AXIS2_OP_ADD_MODULE_QNAME(op, env, ref_qname);
                AXIS2_QNAME_FREE(ref_qname, env);
                if (AXIS2_SUCCESS != status)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MODULE_NOT_FOUND,
                            AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }
        }
    }
    return AXIS2_SUCCESS;
}

axis2_msg_recv_t *AXIS2_CALL
axis2_desc_builder_load_msg_recv(
    axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    struct axiom_element *recv_element)
{
    axiom_attribute_t *recv_name = NULL;
    axis2_char_t *class_name = NULL;
    axis2_msg_recv_t *msg_recv = NULL;
    axis2_qname_t *class_qname = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_char_t *repos_name = NULL;
    axis2_char_t *dll_name = NULL;
    axis2_char_t *temp_path = NULL;
    axis2_char_t *temp_path2 = NULL;
    axis2_char_t *temp_path3 = NULL;
    axis2_conf_t *conf = NULL;
    axis2_char_t *msg_recv_dll_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, recv_element, NULL);

    class_qname = axis2_qname_create(env, AXIS2_CLASSNAME, NULL, NULL);
    recv_name = AXIOM_ELEMENT_GET_ATTRIBUTE(recv_element, env, class_qname);
    AXIS2_QNAME_FREE(class_qname, env);
    class_name = AXIOM_ATTRIBUTE_GET_VALUE(recv_name, env);

    conf = AXIS2_DEP_ENGINE_GET_AXIS2_CONF(desc_builder->engine, env);
    if (!conf)
    {
        return NULL;
    }
    impl_info_param = AXIS2_CONF_GET_PARAM(conf, env, class_name);

    if (!impl_info_param)
    {
        dll_desc = axis2_dll_desc_create(env);
        msg_recv_dll_name =
            AXIS2_DLL_DESC_CREATE_PLATFORM_SPECIFIC_DLL_NAME(dll_desc, env,
                    class_name);
        repos_name = AXIS2_DEP_ENGINE_GET_REPOS_PATH(desc_builder->engine, env);
        temp_path = AXIS2_STRACAT(repos_name, AXIS2_PATH_SEP_STR, env);
        temp_path2 = AXIS2_STRACAT(temp_path, AXIS2_LIB_FOLDER, env);
        temp_path3 = AXIS2_STRACAT(temp_path2, AXIS2_PATH_SEP_STR, env);
        dll_name = AXIS2_STRACAT(temp_path3, msg_recv_dll_name, env);
        AXIS2_FREE(env->allocator, temp_path);
        AXIS2_FREE(env->allocator, temp_path2);
        AXIS2_FREE(env->allocator, temp_path3);
        AXIS2_DLL_DESC_SET_NAME(dll_desc, env, dll_name);
        AXIS2_FREE(env->allocator, dll_name);
        AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, AXIS2_MSG_RECV_DLL);
        impl_info_param = axis2_param_create(env, class_name, NULL);
        AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
        axis2_param_set_value_free(impl_info_param, env, axis2_dll_desc_free_void_arg);
        /* set the impl_info_param(which contain dll_desc as value) so that
         * loaded msg_recv loader lib can be re-used in future
         */
        AXIS2_CONF_ADD_PARAM(conf, env, impl_info_param);
    }

    axis2_class_loader_init(env);
    msg_recv = (axis2_msg_recv_t *) axis2_class_loader_create_dll(env,
            impl_info_param);
    return msg_recv;
}

struct axis2_msg_recv *AXIS2_CALL
            axis2_desc_builder_load_default_msg_recv(
                const axis2_env_t *env)
{
    axis2_msg_recv_t *msg_recv = NULL;

    msg_recv = axis2_raw_xml_in_out_msg_recv_create(env);
    return msg_recv;
}

axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_short_file_name(
    const axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axis2_char_t *file_name)
{
    const axis2_char_t *separator = NULL;
    axis2_char_t *value = NULL;
    axis2_char_t *file_name_l = NULL;
    axis2_char_t *short_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, file_name, NULL);

    file_name_l = AXIS2_STRDUP(file_name, env);
    if (!file_name_l)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    separator = ".";

    value = AXIS2_STRSTR(file_name_l, separator);

    value[0] = AXIS2_EOLN;
    short_name = file_name_l;

    return short_name;
}

axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_file_name_without_prefix(
    const axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axis2_char_t *short_file_name)
{
    axis2_char_t *file_name_l = NULL;
    axis2_char_t *short_name = NULL;
    int len = 0;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, short_file_name, NULL);
    file_name_l = AXIS2_STRDUP(short_file_name, env);
    if (!file_name_l)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    len = AXIS2_STRLEN(AXIS2_LIB_PREFIX);
    short_name = &file_name_l[len];
    return short_name;

}

axis2_char_t *AXIS2_CALL
axis2_desc_builder_get_value(
    const axis2_desc_builder_t *desc_builder,
    const axis2_env_t *env,
    axis2_char_t *in)
{
    const axis2_char_t *separator = ":";
    axis2_char_t *value = NULL;
    axis2_char_t *in_l = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, in, NULL);

    in_l = AXIS2_STRDUP(in, env);
    if (!in_l)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    value = AXIS2_STRSTR(in_l, separator);
    value = value + 1;

    return value;
}
