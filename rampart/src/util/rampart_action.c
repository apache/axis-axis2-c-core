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

/**
* Collection class for actions
*/
#include <stdio.h>
#include <rampart_util.h>
#include <axis2_util.h>
#include <rampart_action.h>
#include <rampart_constants.h>


AXIS2_EXTERN actions_ptr AXIS2_CALL
oxs_ctx_create_actions_ptr(const axis2_env_t *env)
{
    actions_ptr actions= NULL;
    actions = (actions_ptr) AXIS2_MALLOC(env->allocator,sizeof(actionss));
    oxs_ctx_reset_actions_ptr(env, actions);
    return actions;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_reset_actions_ptr(const axis2_env_t *env, actions_ptr actions)
{
    actions->encryption_user = NULL;
    actions->encryption_sym_algorithm = NULL; 
    actions->encryption_key_transport_algorithm = NULL;
    
    return AXIS2_SUCCESS;

}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_free_actions_ptr(const axis2_env_t *env, actions_ptr actions)
{
    return AXIS2_SUCCESS;
}

/*TODO populate all if found*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_populate_actions_ptr(const axis2_env_t *env, actions_ptr actions, axis2_param_t *param_action  )
{
    if(!actions){
        actions = oxs_ctx_create_actions_ptr(env);
    }   
    if(!param_action){
        /*error*/
        return AXIS2_FAILURE;
    }
    actions->encryption_user =(axis2_char_t*) rampart_get_action_params(env, param_action, 
                                RAMPART_ACTION_ENCRYPTION_USER );
    actions->encryption_sym_algorithm =(axis2_char_t*) rampart_get_action_params(env, param_action, 
                                RAMPART_ACTION_ENCRYPTION_SYM_ALGORITHM );
    actions->encryption_key_transport_algorithm =(axis2_char_t*) rampart_get_action_params(env, param_action, 
                                RAMPART_ACTION_ENCRYPTION_KEY_TRANSFORM_ALGORITHM );


    return AXIS2_SUCCESS;
}


