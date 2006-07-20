/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <stdio.h>
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_token_data_reference.h>
#include <axiom_attribute.h>
#include <axiom_element.h>


AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_build_data_reference_element(const axis2_env_t *env,
                        axiom_node_t *parent,
                        axis2_char_t* uri
                    )
{
    axiom_node_t *data_reference_node = NULL;
    axiom_element_t *data_reference_ele = NULL;
    axiom_attribute_t *uri_attr = NULL;
    int ret; 
    axiom_namespace_t *ns_obj = NULL;

    ns_obj = axiom_namespace_create (env, OXS_EncNs,
                                              OXS_xenc);

    data_reference_ele = axiom_element_create(env, parent, OXS_NodeDataReference, ns_obj, &data_reference_node );
    if(!data_reference_ele)
    {   
        oxs_error(ERROR_LOCATION,
                    OXS_ERROR_ELEMENT_FAILED,"Error creating encryption method element");
        return NULL;
    }  

    uri_attr =  axiom_attribute_create (env, OXS_AttrURI, uri, NULL);

    ret = AXIOM_ELEMENT_ADD_ATTRIBUTE(data_reference_ele, env, uri_attr, data_reference_node);  
   
    return data_reference_node; 
     
}

