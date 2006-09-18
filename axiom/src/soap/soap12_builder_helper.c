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

#include "axiom_soap12_builder_helper.h"
#include <axis2_array_list.h>
#include "_axiom_soap_body.h"
#include "_axiom_soap_fault.h"
#include "_axiom_soap_envelope.h"
#include <axiom_soap_message.h>
#include "_axiom_soap_fault_code.h"
#include "_axiom_soap_fault_sub_code.h"
#include "_axiom_soap_fault_node.h"
#include "_axiom_soap_fault_detail.h"
#include "_axiom_soap_fault_reason.h"
#include "_axiom_soap_fault_role.h"
#include "_axiom_soap_fault_value.h"
#include "_axiom_soap_fault_text.h"

/********************* impl struct ********************************************/

typedef struct axiom_soap12_builder_helper_impl_t
{
    axiom_soap12_builder_helper_t builder_helper;

    axiom_soap_builder_t *soap_builder;

    axis2_bool_t code_present;

    axis2_bool_t reason_present;

    axis2_bool_t node_present;

    axis2_bool_t role_present;

    axis2_bool_t detail_present;

    axis2_bool_t subcode_value_present;

    axis2_bool_t value_present;

    axis2_bool_t sub_code_present;

    axis2_bool_t sub_sub_code_present;

    axis2_bool_t code_processing;

    axis2_bool_t sub_code_processing;

    axis2_bool_t reason_processing;

    axis2_array_list_t *detail_element_names;


}
axiom_soap12_builder_helper_impl_t;

/******************** Macro ***************************************************/

#define AXIS2_INTF_TO_IMPL(builder_helper) ((axiom_soap12_builder_helper_impl_t*)builder_helper)

/******************** function prototypes *************************************/

axis2_status_t AXIS2_CALL
axiom_soap12_builder_helper_free(axiom_soap12_builder_helper_t *builder_helper,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axiom_soap12_builder_helper_handle_event
(axiom_soap12_builder_helper_t *builder_helper,
        const axis2_env_t *env,
        axiom_node_t *om_element_node,
        int element_level);

/******************** function implementations ********************************/

AXIS2_EXTERN axiom_soap12_builder_helper_t* AXIS2_CALL
axiom_soap12_builder_helper_create(const axis2_env_t *env,
        axiom_soap_builder_t *soap_builder)
{
    axiom_soap12_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, soap_builder, NULL);

    builder_helper_impl = (axiom_soap12_builder_helper_impl_t*)AXIS2_MALLOC(
                env->allocator, sizeof(axiom_soap12_builder_helper_impl_t));
    if (!builder_helper_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    builder_helper_impl->code_present = AXIS2_FALSE;
    builder_helper_impl->detail_present = AXIS2_FALSE;
    builder_helper_impl->reason_present = AXIS2_FALSE;
    builder_helper_impl->role_present = AXIS2_FALSE;
    builder_helper_impl->sub_code_present = AXIS2_FALSE;
    builder_helper_impl->reason_processing = AXIS2_FALSE;
    builder_helper_impl->code_processing = AXIS2_FALSE;
    builder_helper_impl->sub_code_processing = AXIS2_FALSE;
    builder_helper_impl->detail_element_names = NULL;
    builder_helper_impl->node_present = AXIS2_FALSE;
    builder_helper_impl->soap_builder = soap_builder;
    builder_helper_impl->sub_sub_code_present = AXIS2_FALSE;
    builder_helper_impl->value_present = AXIS2_FALSE;
    builder_helper_impl->subcode_value_present = AXIS2_FALSE;
    builder_helper_impl->builder_helper.ops = NULL;

    builder_helper_impl->builder_helper.ops = (axiom_soap12_builder_helper_ops_t*) AXIS2_MALLOC(
                env->allocator, sizeof(axiom_soap12_builder_helper_ops_t));

    if (!(builder_helper_impl->builder_helper.ops))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, builder_helper_impl);
        return NULL;
    }

    builder_helper_impl->builder_helper.ops->free_fn =
        axiom_soap12_builder_helper_free;

    builder_helper_impl->builder_helper.ops->handle_event =
        axiom_soap12_builder_helper_handle_event;

    return &(builder_helper_impl->builder_helper);
}

axis2_status_t AXIS2_CALL
axiom_soap12_builder_helper_free(axiom_soap12_builder_helper_t *builder_helper,
        const axis2_env_t *env)
{
    axiom_soap12_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);

    if (builder_helper_impl->detail_element_names)
    {
        AXIS2_ARRAY_LIST_FREE(builder_helper_impl->detail_element_names, env);
        builder_helper_impl->detail_element_names = NULL;
    }
    if (builder_helper_impl->builder_helper.ops)
    {
        AXIS2_FREE(env->allocator, builder_helper_impl->builder_helper.ops);
        builder_helper_impl->builder_helper.ops = NULL;
    }

    AXIS2_FREE(env->allocator, builder_helper_impl);
    builder_helper_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axiom_soap12_builder_helper_handle_event(axiom_soap12_builder_helper_t *builder_helper,
        const axis2_env_t *env,
        axiom_node_t *om_ele_node,
        int element_level)
{
    axiom_soap12_builder_helper_impl_t *builder_helper_impl = NULL;

    axis2_char_t* ele_localname = NULL;
    axiom_element_t *om_ele = NULL;

    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_ele_node, AXIS2_FAILURE);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);

    om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(om_ele_node, env);
    if (!om_ele)
        return AXIS2_FAILURE;

    ele_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
    if (!ele_localname)
        return AXIS2_FAILURE;

    soap_envelope = AXIOM_SOAP_BUILDER_GET_SOAP_ENVELOPE(builder_helper_impl->soap_builder, env);
    if (!soap_envelope)
        return AXIS2_FAILURE;

    soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    if (!soap_body)
        return AXIS2_FAILURE;

    soap_fault = AXIOM_SOAP_BODY_GET_FAULT(soap_body, env);
    if (!soap_fault)
        return AXIS2_FAILURE;

    if (element_level == 4)
    {
        if (AXIS2_STRCMP(AXIOM_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME, ele_localname) == 0)
        {
            if (builder_helper_impl->code_present)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MULTIPLE_CODE_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);

                return AXIS2_FAILURE;
            }
            else
            {
                axiom_soap_fault_code_t* soap_fault_code = NULL;
                soap_fault_code = axiom_soap_fault_code_create(env);
                if (!soap_fault_code)
                    return AXIS2_FAILURE;

                axiom_soap_fault_code_set_base_node(soap_fault_code , env, om_ele_node);


                axiom_soap_fault_code_set_builder(soap_fault_code, env,
                        builder_helper_impl->soap_builder);

                axiom_soap_fault_set_code(soap_fault, env, soap_fault_code);

                builder_helper_impl->code_present = AXIS2_TRUE;
                builder_helper_impl->code_processing = AXIS2_TRUE;
            }
        }
        else if (AXIS2_STRCMP(AXIOM_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME, ele_localname) == 0)
        {
            if (!(builder_helper_impl->code_processing) && !(builder_helper_impl->sub_code_processing))
            {
                if (builder_helper_impl->code_present)
                {
                    if (builder_helper_impl->reason_present)
                    {
                        AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_MULTIPLE_REASON_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);

                        return AXIS2_FAILURE;
                    }
                    else
                    {
                        axiom_soap_fault_reason_t *fault_reason = NULL;
                        fault_reason = axiom_soap_fault_reason_create(env);
                        if (!fault_reason)
                            return AXIS2_FAILURE;

                        axiom_soap_fault_reason_set_base_node(fault_reason, env, om_ele_node);

                        axiom_soap_fault_set_reason(soap_fault, env, fault_reason);

                        axiom_soap_fault_reason_set_builder(fault_reason , env,
                                builder_helper_impl->soap_builder);

                        builder_helper_impl->reason_present = AXIS2_TRUE;
                        builder_helper_impl->reason_processing = AXIS2_TRUE;
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }
            else
            {
                if (builder_helper_impl->code_processing)
                {
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
                else
                {
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }
        }
        else if (AXIS2_STRCMP(ele_localname, AXIOM_SOAP12_SOAP_FAULT_NODE_LOCAL_NAME) == 0)
        {
            if (!(builder_helper_impl->reason_processing))
            {
                if (builder_helper_impl->reason_present &&
                        !(builder_helper_impl->role_present) &&
                        !(builder_helper_impl->detail_present))
                {
                    if (builder_helper_impl->node_present)
                    {
                        AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_MULTIPLE_NODE_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                        return AXIS2_FAILURE;
                    }
                    else
                    {
                        axiom_soap_fault_node_t *soap_fault_node = NULL;
                        soap_fault_node = axiom_soap_fault_node_create(env);
                        if (!soap_fault_node)
                            return AXIS2_FAILURE;

                        axiom_soap_fault_node_set_base_node(soap_fault_node, env, om_ele_node);

                        axiom_soap_fault_set_node(soap_fault, env, soap_fault_node);

                        builder_helper_impl->node_present = AXIS2_TRUE;
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED, AXIS2_FALSE);


                    return AXIS2_FAILURE;
                }
            }
            else
            {
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_SOAP_FAULT_REASON_ELEMENT_SHOULD_HAVE_A_TEXT, AXIS2_FALSE);
                return AXIS2_FAILURE;
            }

        }
        else if (AXIS2_STRCMP(ele_localname, AXIOM_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME) == 0)
        {
            if (!(builder_helper_impl->reason_processing))
            {
                if (builder_helper_impl->reason_present && !(builder_helper_impl->detail_present))
                {
                    if (builder_helper_impl->role_present)
                    {
                        AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_MULTIPLE_ROLE_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);

                        return AXIS2_FAILURE;
                    }
                    else
                    {
                        axiom_soap_fault_role_t *soap_fault_role = NULL;
                        soap_fault_role = axiom_soap_fault_role_create(env);
                        if (!soap_fault_role)
                            return AXIS2_FAILURE;

                        axiom_soap_fault_role_set_base_node(soap_fault_role, env, om_ele_node);

                        axiom_soap_fault_set_role(soap_fault, env, soap_fault_role);
                        builder_helper_impl->role_present = AXIS2_TRUE;
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }
            else
            {

                AXIS2_ERROR_SET(env->error,  AXIS2_ERROR_SOAP_FAULT_ROLE_ELEMENT_SHOULD_HAVE_A_TEXT, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
        }
        else if (AXIS2_STRCMP(ele_localname, AXIOM_SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME) == 0)
        {
            if (!(builder_helper_impl->reason_processing))
            {
                if (builder_helper_impl->reason_present)
                {
                    if (builder_helper_impl->detail_present)
                    {
                        AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_MULTIPLE_DETAIL_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                        return AXIS2_FAILURE;
                    }
                    else
                    {

                        axiom_soap_fault_detail_t *soap_fault_detail = NULL;
                        soap_fault_detail = axiom_soap_fault_detail_create(env);
                        if (!soap_fault_detail)
                            return AXIS2_FAILURE;

                        axiom_soap_fault_detail_set_base_node(soap_fault_detail, env, om_ele_node);

                        axiom_soap_fault_set_detail(soap_fault, env, soap_fault_detail);
                        builder_helper_impl->detail_present = AXIS2_TRUE;
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED, AXIS2_FAILURE);

                    return AXIS2_FAILURE;

                }
            }
            else
            {
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_SOAP_FAULT_REASON_ELEMENT_SHOULD_HAVE_A_TEXT, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }

        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_UNSUPPORTED_ELEMENT_IN_SOAP_FAULT_ELEMENT, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    else if (element_level == 5)
    {
        axiom_node_t *parent_node = NULL;
        axiom_element_t *parent_ele = NULL;
        axis2_char_t *parent_localname = NULL;
        parent_node = AXIOM_NODE_GET_PARENT(om_ele_node, env);
        if (!parent_node)
            return AXIS2_FAILURE;

        parent_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(parent_node, env);
        if (!parent_ele)
            return AXIS2_FAILURE;

        parent_localname = AXIOM_ELEMENT_GET_LOCALNAME(parent_ele, env);
        if (!parent_localname)
            return AXIS2_FAILURE;

        if (AXIS2_STRCMP(parent_localname,
                AXIOM_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME) == 0)
        {
            if (AXIS2_STRCMP(ele_localname, AXIOM_SOAP12_SOAP_FAULT_VALUE_LOCAL_NAME) == 0)
            {
                if (!(builder_helper_impl->value_present))
                {
                    axiom_soap_fault_value_t *soap_fault_value = NULL;
                    axiom_soap_fault_code_t *parent_fcode = NULL;

                    soap_fault_value = axiom_soap_fault_value_create(env);
                    if (!soap_fault_value)
                        return AXIS2_FAILURE;

                    axiom_soap_fault_value_set_base_node(soap_fault_value, env, om_ele_node);

                    parent_fcode = AXIOM_SOAP_FAULT_GET_CODE(soap_fault, env);
                    if (!parent_fcode)
                        return AXIS2_FAILURE;

                    axiom_soap_fault_code_set_value(parent_fcode, env, soap_fault_value);

                    builder_helper_impl->value_present = AXIS2_TRUE;
                    builder_helper_impl->code_processing = AXIS2_FALSE;

                }
                else
                {
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_MULTIPLE_VALUE_ENCOUNTERED_IN_CODE_ELEMENT, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }
            else if (AXIS2_STRCMP(ele_localname,
                    AXIOM_SOAP12_SOAP_FAULT_SUB_CODE_LOCAL_NAME) == 0)
            {
                if (!(builder_helper_impl->sub_code_present))
                {
                    if (builder_helper_impl->value_present)
                    {

                        axiom_soap_fault_sub_code_t *fault_subcode = NULL;
                        axiom_soap_fault_code_t *fault_code = NULL;

                        fault_subcode = axiom_soap_fault_sub_code_create(env);
                        if (!fault_subcode)
                            return AXIS2_FAILURE;

                        axiom_soap_fault_sub_code_set_base_node(fault_subcode, env, om_ele_node);


                        fault_code = AXIOM_SOAP_FAULT_GET_CODE(soap_fault, env);
                        if (!fault_code)
                            return AXIS2_FAILURE;

                        axiom_soap_fault_code_set_sub_code(fault_code, env, fault_subcode);

                        axiom_soap_fault_sub_code_set_builder(fault_subcode, env,
                                builder_helper_impl->soap_builder);

                        builder_helper_impl->sub_code_present = AXIS2_TRUE;
                        builder_helper_impl->sub_code_processing = AXIS2_TRUE;
                    }
                    else
                    {
                        AXIS2_ERROR_SET(env->error,
                                AXIS2_ERROR_SOAP_FAULT_VALUE_SHOULD_BE_PRESENT_BEFORE_SUB_CODE, AXIS2_FAILURE);
                        return AXIS2_FAILURE;
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MULTIPLE_SUB_CODE_VALUES_ENCOUNTERED, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
            }
            else
            {
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_THIS_LOCALNAME_NOT_SUPPORTED_INSIDE_THE_CODE_ELEMENT, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
        }
        else if (AXIS2_STRCMP(parent_localname, AXIOM_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME) == 0)
        {
            if (AXIS2_STRCMP(ele_localname, AXIOM_SOAP12_SOAP_FAULT_TEXT_LOCAL_NAME) == 0)
            {
                axiom_soap_fault_text_t *soap_fault_text = NULL;
                axiom_soap_fault_reason_t *fault_reason = NULL;

                soap_fault_text = axiom_soap_fault_text_create(env);
                if (!soap_fault_text)
                    return AXIS2_FAILURE;

                axiom_soap_fault_text_set_base_node(soap_fault_text, env, om_ele_node);

                fault_reason = AXIOM_SOAP_FAULT_GET_REASON(soap_fault, env);
                if (!fault_reason)
                    return AXIS2_FAILURE;

                AXIOM_SOAP_FAULT_REASON_ADD_SOAP_FAULT_TEXT(fault_reason, env, soap_fault_text);
                /*****************
                AXIOM_NODE_SET_BUILD_STATUS(om_ele_node, env, AXIS2_FALSE);
                ******************/
                builder_helper_impl->reason_processing = AXIS2_FALSE;

                AXIOM_SOAP_BUILDER_SET_BOOL_PROCESSING_MANDATORY_FAULT_ELEMENTS(builder_helper_impl->soap_builder, env, AXIS2_FALSE);

            }
            else
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_REASON_ELEMENT, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }

        }
        else if (AXIS2_STRCMP(parent_localname, AXIOM_SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME) == 0)
        {
            AXIOM_SOAP_BUILDER_SET_PROCESSING_DETAIL_ELEMENTS(builder_helper_impl->soap_builder, env, AXIS2_TRUE);

            if (!(builder_helper_impl->detail_element_names))
            {
                builder_helper_impl->detail_element_names = axis2_array_list_create(env, 20);
            }

            AXIS2_ARRAY_LIST_ADD(builder_helper_impl->detail_element_names, env, ele_localname);

        }
        else
        {

            return AXIS2_FAILURE;
        }

    }
    else if (element_level > 5)
    {
        axiom_node_t *parent_node = NULL;
        axiom_element_t *parent_ele = NULL;
        axis2_char_t *parent_localname = NULL;

        parent_node = AXIOM_NODE_GET_PARENT(om_ele_node, env);
        if (!parent_node)
            return AXIS2_FAILURE;

        parent_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(parent_node, env);
        if (!parent_ele)
            return AXIS2_FAILURE;

        parent_localname = AXIOM_ELEMENT_GET_LOCALNAME(parent_ele, env);
        if (!parent_localname)
            return AXIS2_FAILURE;



        if (AXIS2_STRCMP(parent_localname, AXIOM_SOAP12_SOAP_FAULT_SUB_CODE_LOCAL_NAME) == 0)
        {
            if (AXIS2_STRCMP(ele_localname, AXIOM_SOAP12_SOAP_FAULT_VALUE_LOCAL_NAME) == 0)
            {
                if (builder_helper_impl->subcode_value_present)
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MULTIPLE_SUB_CODE_VALUES_ENCOUNTERED, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }
                else
                {

                    axiom_soap_fault_sub_code_t *sub_code = NULL;
                    axiom_soap_fault_code_t *code = NULL;
                    axiom_soap_fault_value_t *value = NULL;
                    code = AXIOM_SOAP_FAULT_GET_CODE(soap_fault, env);

                    if (!code)
                    {
                        AXIS2_LOG_DEBUG(env->log , AXIS2_LOG_SI, "fault code null when it should not be null");
                        return AXIS2_FAILURE;
                    }

                    sub_code = AXIOM_SOAP_FAULT_CODE_GET_SUB_CODE(code, env);
                    if (!sub_code)
                    {
                        AXIS2_LOG_DEBUG(env->log , AXIS2_LOG_SI, "fault subcode null when it should not be null");
                        return AXIS2_FAILURE;
                    }
                    value = axiom_soap_fault_value_create(env);
                    if (!value)
                        return AXIS2_FAILURE;

                    axiom_soap_fault_value_set_base_node(value, env, om_ele_node);

                    axiom_soap_fault_sub_code_set_value(sub_code, env, value);

                    builder_helper_impl->subcode_value_present = AXIS2_TRUE;
                    builder_helper_impl->sub_sub_code_present = AXIS2_FALSE;
                    builder_helper_impl->sub_code_processing = AXIS2_FALSE;
                }
            }
            else if (AXIS2_STRCMP(ele_localname, AXIOM_SOAP12_SOAP_FAULT_SUB_CODE_LOCAL_NAME) == 0)
            {
                if (builder_helper_impl->subcode_value_present)
                {
                    if (!(builder_helper_impl->sub_sub_code_present))
                    {
                        axiom_soap_fault_code_t *fault_code = NULL;
                        axiom_soap_fault_sub_code_t *parent_subcode = NULL;
                        axiom_soap_fault_sub_code_t *subcode = NULL;

                        subcode = axiom_soap_fault_sub_code_create(env);
                        if (!subcode)
                            return AXIS2_FAILURE;

                        axiom_soap_fault_sub_code_set_base_node(subcode, env, om_ele_node);

                        fault_code = AXIOM_SOAP_FAULT_GET_CODE(soap_fault, env);
                        if (!fault_code)
                        {
                            AXIS2_LOG_DEBUG(env->log , AXIS2_LOG_SI, "fault code null when it should not be null");
                            return AXIS2_FAILURE;
                        }
                        parent_subcode = AXIOM_SOAP_FAULT_CODE_GET_SUB_CODE(fault_code, env);
                        if (!parent_subcode)
                        {
                            AXIS2_LOG_DEBUG(env->log , AXIS2_LOG_SI, "fault subcode null when it should not be null");
                            return AXIS2_FAILURE;
                        }

                        axiom_soap_fault_sub_code_set_sub_code(parent_subcode, env, subcode);

                        builder_helper_impl->subcode_value_present = AXIS2_FALSE;
                        builder_helper_impl->sub_sub_code_present = AXIS2_TRUE;
                        builder_helper_impl->sub_code_processing = AXIS2_TRUE;
                    }
                    else
                    {
                        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_MULTIPLE_SUB_CODE_VALUES_ENCOUNTERED, AXIS2_FAILURE);

                        return AXIS2_FAILURE;
                    }
                }
                else
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOAP_FAULT_VALUE_SHOULD_BE_PRESENT_BEFORE_SUB_CODE, AXIS2_FAILURE);

                    return AXIS2_FAILURE;

                }
            }
            else
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_THIS_LOCALNAME_IS_NOT_SUPPORTED_INSIDE_THE_SUB_CODE_ELEMENT, AXIS2_FAILURE);

                return AXIS2_FAILURE;
            }
        }
        else if (AXIOM_SOAP_BUILDER_IS_PROCESSING_DETAIL_ELEMENTS(builder_helper_impl->soap_builder, env))
        {

            int detail_element_level = 0;
            axis2_bool_t local_name_exists = AXIS2_FALSE;
            int i = 0;
            if (!(builder_helper_impl->detail_element_names))
                return AXIS2_FAILURE;

            for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(builder_helper_impl->detail_element_names, env) ; i++)
            {
                if (AXIS2_STRCMP(parent_localname, AXIS2_ARRAY_LIST_GET(builder_helper_impl->detail_element_names, env, i)) == 0)
                {
                    local_name_exists = AXIS2_TRUE;
                    detail_element_level = i + 1;
                }
            }
            if (local_name_exists)
            {
                AXIS2_ARRAY_LIST_ADD(builder_helper_impl->detail_element_names, env, ele_localname);
            }
            else
            {
                return AXIS2_FAILURE;
            }
        }
    }
    return AXIS2_SUCCESS;
}
