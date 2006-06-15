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

#ifndef USER_SEC_PARAM_H
#define USER_SEC_PARAM_H

/**
 * @file user_sec_param.h
 * @brief User Security Parameter interface
 */

#include <axis2_utils_defines.h>
#include <axis2_env.h>


#ifdef __cplusplus
extern "C"
{
#endif

struct user_sec_param ;
struct user_sec_param_ops ;

             
AXIS2_DECLARE_DATA typedef struct user_sec_param_ops
{
    /**
     *  Free a user_sec_param struct
     *  @return Status code
     */
    axis2_status_t (AXIS2_CALL *
	free) (struct user_sec_param * user_sec_param,
           const axis2_env_t *env);                                   
										   
	axis2_char_t* (AXIS2_CALL *
		get_user)(struct user_sec_param * user_sec_param,
				const axis2_env_t *env);	
} user_sec_param_ops_t;


typedef struct user_sec_param
{
	/** Parameter related ops */
	user_sec_param_ops_t *ops;
}user_sec_param_t;


/**
 *	creates a user_sec_param struct
 *  returns a pointer to a user_sec_param struct
 *  @user   optional
 *  @password      optional  
 *  @items      optional
 * @return a pointer to newly created user_sec_param struct
 */

AXIS2_EXTERN user_sec_param_t *AXIS2_CALL
user_sec_param_create (const axis2_env_t *env,
                    const axis2_char_t * user,
                    const axis2_char_t * password,
                    const axis2_char_t * items);

#define USER_SEC_PARAM_FREE(user_sec_param , env) \
        ((user_sec_param )->ops->free(user_sec_param ,env))
        
#define USER_SEC_PARAM_GET_USER(user_sec_param, env) \
        ((user_sec_param)->ops->get_user(user_sec_param, env))

#ifdef __cplusplus
}
#endif

#endif    /* USER_SEC_PARAM_H */
