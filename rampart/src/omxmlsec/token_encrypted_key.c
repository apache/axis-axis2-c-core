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
#include <oxs_token_encrypted_key.h>
#include <axiom_attribute.h>
#include <axiom_element.h>

/*TODO These names should be changed to oxs_token_build_XXX_node*/

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_build_encrypted_key_element(const axis2_env_t *env,
                        axiom_node_t *parent )
{
    axiom_node_t *encrypted_key_node = NULL;
    axiom_element_t *encrypted_key_ele = NULL;
    int ret; 
    axiom_namespace_t *ns_obj = NULL;

    ns_obj = axiom_namespace_create (env, OXS_EncNs,
                                              OXS_xenc);

    encrypted_key_ele = axiom_element_create(env, parent, OXS_NodeEncryptedKey, ns_obj, &encrypted_key_node );
    if(!encrypted_key_ele)
    {   
        oxs_error(ERROR_LOCATION,
                    OXS_ERROR_ELEMENT_FAILED,"Error creating encrypted key element");
        return NULL;
    }  
    
    
    
    return encrypted_key_node; 
     
}


AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_get_encrypted_key_node(const axis2_env_t *env,
                            axiom_node_t *parent)
{
    axiom_node_t *enc_key_node = NULL;

    /*TODO*/
    return enc_key_node;
}
