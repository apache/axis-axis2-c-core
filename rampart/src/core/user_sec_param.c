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

#include <rampart/user_sec_param.h>
#include <axis2_env.h>
#include <axis2_string.h>
#include <axis2_utils.h>

/********************************** Function prototypes *****************/

axis2_status_t AXIS2_CALL 
user_sec_param_free (user_sec_param_t * sec_param,
                   const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
user_sec_param_get_user(user_sec_param_t *user_sec_param,
                    const axis2_env_t *env);
                                            
/*************************************** user_sec_param struct *********************/

typedef struct user_sec_param_t
{
    /** this should be first member */
    user_sec_param_t user_sec_param;
    /** username*/
    axis2_char_t *user;
    /** password */
    axis2_char_t *password;
    /**  items */
    axis2_char_t *items;
}user_sec_param_impl_t;


/************************* Macro ****************************************/

#define USER_SEC_PARAM_INTF_TO_IMPL(qname) ((user_sec_param_impl_t*)user_sec_param)

/************************************************************************/

AXIS2_EXTERN user_sec_param_t *AXIS2_CALL
user_sec_param_create (const axis2_env_t *env,
                    const axis2_char_t * user,
                    const axis2_char_t * password,
                    const axis2_char_t * items)
{
        user_sec_param_impl_t *usp= NULL;
        AXIS2_ENV_CHECK(env, NULL);

    /* assign properties */
    usp->user = NULL;
    usp->password = NULL;
    usp->items = NULL;

    usp->user = (axis2_char_t *)AXIS2_STRDUP (user, env);
    usp->password = (axis2_char_t *)AXIS2_STRDUP (password, env);
    usp->items = (axis2_char_t *)AXIS2_STRDUP (items, env);

    usp->user_sec_param.ops->free = user_sec_param_free;  
    usp->user_sec_param.ops->get_user = user_sec_param_get_user;
    return &(usp->user_sec_param);
}


axis2_status_t AXIS2_CALL 
user_sec_param_free ( user_sec_param_t * user_sec_param,
                   const axis2_env_t *env)
{   
       user_sec_param_impl_t *user_sec_param_impl = NULL;
        AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
       user_sec_param_impl = AXIS2_INTF_TO_IMPL(user_sec_param);
        
        if (user_sec_param_impl->user)
        {
            AXIS2_FREE (env->allocator, user_sec_param_impl->user);
            user_sec_param_impl->user = NULL;
        }
        if (user_sec_param_impl->password)
        {
            AXIS2_FREE (env->allocator, user_sec_param_impl->password);
            user_sec_param_impl->password = NULL;
        }
        if (user_sec_param_impl->items)
        {
            AXIS2_FREE (env->allocator, user_sec_param_impl->items);
            user_sec_param_impl->items = NULL;
        }
	    AXIS2_FREE (env->allocator, user_sec_param_impl);
		return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
user_sec_param_get_user(user_sec_param_t *user_sec_param,
                    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return USER_SEC_PARAM_INTF_TO_IMPL(user_sec_param)->user;
}
