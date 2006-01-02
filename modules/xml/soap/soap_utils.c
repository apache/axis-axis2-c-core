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
 
 #include <axis2_om_element.h>
 #include <axis2_om_node.h>
 #include <axis2_soap_utils.h>
 
AXIS2_DECLARE(axis2_om_node_t *)
axis2_soap_utils_get_child_with_name(axis2_env_t **env,
                                     axis2_om_node_t *om_node,
                                     axis2_char_t *localname)
{
    axis2_om_children_iterator_t *iter = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_om_node_t *iter_node = NULL;
    axis2_om_element_t *iter_ele = NULL;
    axis2_char_t *name = NULL;
    AXIS2_PARAM_CHECK((*env)->error, om_node, NULL);
    AXIS2_PARAM_CHECK((*env)->error, localname, NULL);
    if(AXIS2_OM_NODE_GET_NODE_TYPE(om_node, env) != AXIS2_OM_ELEMENT)
        return NULL;
    om_ele = (axis2_om_element_t*) 
            AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
            
    iter = AXIS2_OM_ELEMENT_GET_CHILDREN(om_ele, env, om_node);
    while(AXIS2_OM_CHILDREN_ITERATOR_HAS_NEXT(iter, env))
    {
        iter_node = AXIS2_OM_CHILD_ELEMENT_ITERATOR_NEXT(iter, env);
        if(AXIS2_OM_NODE_GET_NODE_TYPE(iter_node, env) == AXIS2_OM_ELEMENT)
        {
            iter_ele = (axis2_om_element_t*) 
                        AXIS2_OM_NODE_GET_DATA_ELEMENT(iter_node, env);
            
            name = AXIS2_OM_ELEMENT_GET_LOCALNAME(iter_ele, env);
            if(AXIS2_STRCMP(name, localname) == 0)
            {
                AXIS2_OM_CHILDREN_ITERATOR_FREE(iter , env);
                   return iter_node;
            }
       }
    }
    AXIS2_OM_CHILDREN_ITERATOR_FREE(iter , env);
    return NULL;
}                                     
                                     
                                     
                                     
AXIS2_DECLARE(axis2_status_t)
axis2_soap_utils_set_new_node(axis2_env_t **env,
                              axis2_om_node_t *parent,
                              axis2_om_node_t **my_node,
                              axis2_om_node_t  *new_node)
{
    if(*my_node)
    {
        AXIS2_OM_NODE_FREE_TREE(*my_node, env);
    }
    AXIS2_OM_NODE_ADD_CHILD(new_node, env, parent);
    *my_node = new_node;
    return AXIS2_SUCCESS;
}


