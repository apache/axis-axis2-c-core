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

/*
typedef struct _actions actions, *actions_ptr;
struct _actions{
        axis2_char_t *items  ;
        axis2_char_t *user  ;
        axis2_char_t *passwordC_callback_lass  ;
        axis2_char_t *encryption_prop_file;
        axis2_char_t *signature_prop_file ;
        axis2_char_t *signature_key_identifier  ;
        axis2_char_t *encryption_key_identifier  ;
        axis2_char_t *encryption_user  ;
        axis2_char_t *signature_parts  ;
        axis2_char_t *encryption_parts  ;
        axis2_char_t *optimize_parts  ;
        axis2_char_t *encryption_sym_algorithm  ;
        axis2_char_t *embedded_key_callback_class  ;
        axis2_char_t *encryption_key_transport_algorithm  ;
        axis2_char_t *embedded_key_name  ;
        axis2_char_t *time_to_live  ;

};
*/

AXIS2_EXTERN actions_ptr AXIS2_CALL
oxs_ctx_create_actions_ptr(const axis2_env_t *env)
{
    actions_ptr actions= NULL;
    actions = (actions_ptr) AXIS2_MALLOC(env->allocator,sizeof(actions_ptr));

    return actions;
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


