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
#include <oxs_token_encrypted_type.h>
#include <axiom_attribute.h>
#include <axiom_element.h>

/*TODO These names should be changed to oxs_token_build_XXX_node*/

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_build_encrypted_type_element(const axis2_env_t *env,
        axiom_node_t *parent,
        axis2_char_t* encrypted_type,
        axis2_char_t* type_attribute,
        axis2_char_t* id
                                      )
{
    axiom_node_t *encrypted_type_node = NULL;
    axiom_element_t *encrypted_type_ele = NULL;
    axiom_attribute_t *type_attr = NULL
    axiom_attribute_t *id_attr = NULL;
    int ret;
    axiom_namespace_t *ns_obj = NULL;

    ns_obj = axiom_namespace_create(env, OXS_EncNs,
            OXS_xenc);

    encrypted_type_ele = axiom_element_create(env, parent, encrypted_type, ns_obj, &encrypted_type_node);
    if (!encrypted_type_ele)
    {
        oxs_error(ERROR_LOCATION,
                OXS_ERROR_ELEMENT_FAILED, "Error creating encrypted type element");
        return NULL;
    }

    if (type_attribute)
    {
        type_attr =  axiom_attribute_create(env, OXS_AttrType, type_attribute, NULL);
        ret = AXIOM_ELEMENT_ADD_ATTRIBUTE(encrypted_type_ele, env, type_attr, encrypted_type_node);
    }

    if (id)
    {
        id_attr = axiom_attribute_create(env, OXS_AttrId, id, NULL);
        ret = AXIOM_ELEMENT_ADD_ATTRIBUTE(encrypted_type_ele, env, id_attr, encrypted_type_node);
    }
    else
    {
        id_attr = axiom_attribute_create(env, OXS_AttrId, "EncDataId-54321", NULL);
        ret = AXIOM_ELEMENT_ADD_ATTRIBUTE(encrypted_type_ele, env, id_attr, encrypted_type_node);
    }

    return encrypted_type_node;

}

