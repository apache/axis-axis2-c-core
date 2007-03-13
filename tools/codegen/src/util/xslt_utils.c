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
 
#include <w2c_xslt_utils.h>
#include <axis2_qname.h>

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
w2c_xslt_utils_add_attribute(
    const axis2_env_t *env,
    axiom_node_t *om_node,
    axis2_char_t *attribute,
    axis2_char_t *attrib_value)
{
    axiom_element_t *om_ele = NULL;
    axiom_attribute_t *attri = NULL;
    AXIS2_ENV_CHECK ( env, AXIS2_FAILURE );

    om_ele = AXIOM_NODE_GET_DATA_ELEMENT ( om_node, env );
    attri = axiom_attribute_create ( env, attribute, attrib_value, NULL );
    AXIOM_ELEMENT_ADD_ATTRIBUTE ( om_ele, env, attri, om_node );
    
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
w2c_xslt_utils_add_child_node(
    const axis2_env_t *env,
    axis2_char_t *element_name,
    axiom_node_t *parent_node)
{
    axiom_node_t *child_node = NULL;
    axiom_element_t *child_ele = NULL;
	AXIS2_ENV_CHECK ( env, NULL );

    child_ele = axiom_element_create ( env,
                    parent_node,
                    element_name,
                    NULL,
                    &child_node );
    return child_node;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_xslt_utils_serialize(
                const axis2_env_t *env,
                axiom_node_t *root)
{
    axiom_output_t *om_output = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axis2_char_t *buffer = NULL;

    xml_writer = axiom_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0,
                                      AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create (env, xml_writer);

    AXIOM_NODE_SERIALIZE (root, env, om_output);
    buffer = (axis2_char_t*)AXIOM_XML_WRITER_GET_XML(xml_writer, env);
    return buffer;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
w2c_xslt_utils_add_text(
    const axis2_env_t *env,
    axiom_node_t *node,
    axis2_char_t *text)
{
   axiom_element_t *ele = NULL;
   
   ele = AXIOM_NODE_GET_DATA_ELEMENT ( node, env);
   AXIOM_ELEMENT_SET_TEXT( ele, env, text, node);
   return node;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
w2c_xslt_utils_copy_node_tree(
     const axis2_env_t *env,
     axiom_node_t *to,
     axiom_node_t *from )
{
    axiom_element_t *to_ele = NULL;
    axiom_element_t *from_ele = NULL;
    axiom_node_t *to_child = NULL;
    axiom_node_t *from_child = NULL;
    axiom_element_t *from_child_ele = NULL;
    axiom_children_iterator_t *it = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *text = NULL;

    axis2_hash_t *attr_ht = NULL;
    axis2_hash_index_t *hi = NULL;
    void *val = NULL;
    axiom_attribute_t *om_attri = NULL;

    to_ele = AXIOM_NODE_GET_DATA_ELEMENT ( to, env );
    from_ele = AXIOM_NODE_GET_DATA_ELEMENT ( from, env );

    it = AXIOM_ELEMENT_GET_CHILDREN ( from_ele, env, from );
    axiom_children_iterator_reset( it, env);

    while ( (from_child = axiom_children_iterator_next( it, env )) != NULL )
    {
        if ( AXIOM_NODE_GET_NODE_TYPE ( from_child, env ) == AXIOM_TEXT )
        {
            text = AXIOM_ELEMENT_GET_TEXT ( from_ele, env, from );
            text = AXIS2_STRDUP ( text, env);
            AXIOM_ELEMENT_SET_TEXT ( to_ele, env, text, to);
        }
        else if ( AXIOM_NODE_GET_NODE_TYPE ( from_child, env ) == AXIOM_ELEMENT )
        {
            from_child_ele = AXIOM_NODE_GET_DATA_ELEMENT ( from_child, env );
            qname = AXIOM_ELEMENT_GET_QNAME ( from_child_ele, env, from_child );
            qname = AXIS2_QNAME_CLONE ( qname, env );
    
            axiom_element_create_with_qname ( env, to, qname, &to_child );
            w2c_xslt_utils_copy_node_tree ( env, to_child, from_child );
        }
    }

    attr_ht = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(from_ele, env);
    if(NULL != attr_ht)
    {
        for( hi = axis2_hash_first(attr_ht, env);  hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);
            if(NULL != val)
            {
                om_attri = (axiom_attribute_t *)val;
                om_attri = axiom_attribute_clone( om_attri, env );
                AXIOM_ELEMENT_ADD_ATTRIBUTE ( to_ele, env, om_attri, to);
            }
        }
    }
    return AXIS2_SUCCESS;
}
