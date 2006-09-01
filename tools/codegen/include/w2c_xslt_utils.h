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
  
#ifndef W2C_XSLT_UTILS_H
#define W2C_XSLT_UTILS_H

/**
 * @file w2c_xslt_utils.h
 * @brief contains Axis2/C xslt utility functions
 */

#include <stdio.h>
#include <axis2_stream.h>
#include <axis2_utils.h>
#include <axiom.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_xslt_utils xslt utilities
 * @ingroup w2c_utils
 * @{
 */    

/**
 * add attribute to the parse node.
 * @param env           double pointer to environment struct. MUST NOT be NULL
 * @param om_node       node to add attrib
 * @param attribute     name of the attribute
 * @param attrib_value  valu of the attribute
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
w2c_xslt_utils_add_attribute(
    const axis2_env_t *env,
    axiom_node_t *om_node,
    axis2_char_t *attribute,
    axis2_char_t *attrib_value);

/**
 * add a child node.
 * @param env           double pointer to environment struct. MUST NOT be NULL
 * @param element_name  element name
 * @param parent_node   parent node
 * @return  new om node
 */
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
w2c_xslt_utils_add_child_node(
    const axis2_env_t *env,
    axis2_char_t *element_name,
    axiom_node_t *parent_node);

/**
 * serialize to a buffer
 * @param env           double pointer to environment struct. MUST NOT be NULL
 * @param node          node to serialize
 * @return  serialized bufffer
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_xslt_utils_serialize(
        const axis2_env_t *env,
        axiom_node_t *root);

/**
 * add text to the node
 * @param env           double pointer to environment struct. MUST NOT be NULL
 * @param node          node to add the text
 * @param text          text to add
 * @return same node passed
 */
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
w2c_xslt_utils_add_text(
    const axis2_env_t *env,
    axiom_node_t *node,
    axis2_char_t *text);

/**
 * copy all from 'from' to 'to'.
 * @param env           double pointer to environment struct. MUST NOT be NULL
 * @param to            node get copied
 * @param from          node to copy
 * @return AXIS2_SUCCESS in sucesss AXIS2_FAILURE in failure
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
    w2c_xslt_utils_copy_node_tree(
    const axis2_env_t *env,
    axiom_node_t *to,
    axiom_node_t *from );


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_XSLT_UTILS_H */
