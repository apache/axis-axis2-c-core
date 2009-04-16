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

#include <rp_signed_encrypted_parts_builder.h>
#include <neethi_operator.h>
#include <neethi_policy.h>
#include <neethi_exactlyone.h>
#include <neethi_all.h>
#include <neethi_engine.h>

static rp_header_t *AXIS2_CALL rp_signed_encrypted_parts_builder_build_header(
    axiom_element_t *element,
    const axutil_env_t *env);

static axis2_status_t AXIS2_CALL rp_signed_encrypted_parts_builder_set_properties(
    axiom_node_t *node,
    axiom_element_t *element,
    axis2_char_t *local_name,
    rp_signed_encrypted_parts_t *signed_encrypted_parts,
    const axutil_env_t *env);

/**
 * Builts EncryptedParts or SignedParts assertion
 * @param env Pointer to environment struct
 * @param node Assertion node
 * @param element Assertion element
 * @param is_signed boolean showing whether signing or encryption
 * @returns neethi assertion created. NULL if failure.
 */
AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
rp_signed_encrypted_parts_builder_build(
    const axutil_env_t *env,
    axiom_node_t *parts,
    axiom_element_t *parts_ele, 
    axis2_bool_t is_signed)
{
    rp_signed_encrypted_parts_t *signed_encrypted_parts = NULL;
    axiom_children_iterator_t *children_iter = NULL;
    neethi_assertion_t *assertion = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    signed_encrypted_parts = rp_signed_encrypted_parts_create(env);
    if (!signed_encrypted_parts)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Cannot create signed_encrypted_parts.");
        return NULL;
    }

    rp_signed_encrypted_parts_set_signedparts(signed_encrypted_parts, env, is_signed);

    children_iter = axiom_element_get_children(parts_ele, env, parts);
    if (children_iter)
    {
        while (axiom_children_iterator_has_next(children_iter, env))
        {
            axiom_node_t *node = NULL;
            axiom_element_t *ele = NULL;
            axis2_char_t *local_name = NULL;
            node = axiom_children_iterator_next(children_iter, env);
            if (node)
            {
                if (axiom_node_get_node_type(node, env) == AXIOM_ELEMENT)
                {
                    ele = (axiom_element_t *) axiom_node_get_data_element(node, env);
                    if (ele)
                    {
                        local_name = axiom_element_get_localname(ele, env);
                        if (local_name)
                        {
                            status = rp_signed_encrypted_parts_builder_set_properties
                                (node, ele, local_name, signed_encrypted_parts, env);
                            if (status != AXIS2_SUCCESS)
                            {
                                rp_signed_encrypted_parts_free (signed_encrypted_parts, env);
                                signed_encrypted_parts = NULL;
                                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                                    "[neethi] Cannot create signed_encrypted_parts. "
                                    "Error in processing child element %s", local_name);
                                    return NULL;
                            }
                        }
                    }
                }
            }
        }
    }
    assertion = neethi_assertion_create_with_args(
        env, (AXIS2_FREE_VOID_ARG)rp_signed_encrypted_parts_free, 
        signed_encrypted_parts, ASSERTION_TYPE_SIGNED_ENCRYPTED_PARTS);
    return assertion;
}

static axis2_status_t AXIS2_CALL
rp_signed_encrypted_parts_builder_set_properties(
    axiom_node_t *node,
    axiom_element_t *element,
    axis2_char_t *local_name,
    rp_signed_encrypted_parts_t * signed_encrypted_parts,
    const axutil_env_t *env)
{
    axis2_char_t *ns = NULL;
    axutil_qname_t *node_qname = NULL;

    node_qname = axiom_element_get_qname(element, env, node);
    if(!node_qname)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Cannot get qname from element %s.", local_name);
        return AXIS2_FAILURE;
    }

    ns = axutil_qname_get_uri(node_qname, env);
    if(!ns)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Cannot get namespace from element %s.", local_name);
        return AXIS2_FAILURE;
    }

    /* process assertions common for WS-SecPolicy 1.1 and 1.2 */
    if(!(axutil_strcmp(ns, RP_SP_NS_11) && axutil_strcmp(ns, RP_SP_NS_12)))
    {
        /* this assertion is in WS-SecurityPolicy namespace */
        if(!strcmp(local_name, RP_BODY))
        {
            rp_signed_encrypted_parts_set_body(signed_encrypted_parts, env, AXIS2_TRUE);
            return AXIS2_SUCCESS;
        }
        else if(!strcmp(local_name, RP_HEADER))
        {
            rp_header_t *header = NULL;
            header = rp_signed_encrypted_parts_builder_build_header(element, env);
            if(!header)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "[neethi] Failed to process Header Assertion.");
                return AXIS2_FAILURE;
            }

            return rp_signed_encrypted_parts_add_header(signed_encrypted_parts, env, header);
        }
    }

    /* process assertions specific to WS-SecPolicy 1.2 */
    if(!axutil_strcmp(ns, RP_SP_NS_12))
    {
        if(!strcmp(local_name, RP_ATTACHMENTS))
        {
            rp_signed_encrypted_parts_set_attachments(signed_encrypted_parts, env, AXIS2_TRUE);
            return AXIS2_SUCCESS;
        }
    }
    
    /* either namespace or assertion is not understood */
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
        "[neethi] Unknown Assertion %s with namespace %s", local_name, ns);
    return AXIS2_FAILURE;
}

static rp_header_t *AXIS2_CALL
rp_signed_encrypted_parts_builder_build_header(
    axiom_element_t *element,
    const axutil_env_t *env)
{
    rp_header_t *header = NULL;
    axis2_char_t *name = NULL;
    axis2_char_t *nspace = NULL;

    name = axiom_element_get_attribute_value_by_name(element, env, RP_NAME);
    nspace = axiom_element_get_attribute_value_by_name(element, env, RP_NAMESPACE);
    if (!nspace)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Header assertion should have namespace associated with it.");
        return NULL;
    }

    header = rp_header_create(env);
    if (!header)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Cannot create rp_header structure. Insufficient memory.");
        return NULL;
    }

    if (name)
    {
        rp_header_set_name(header, env, name);
    }

    rp_header_set_namespace(header, env, nspace);
    return header;
}
