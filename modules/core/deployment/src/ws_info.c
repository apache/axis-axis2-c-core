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
 
#include <axis2_ws_info.h>
#include <string.h>

/** 
 * @brief
 */
typedef struct axis2_ws_info_impl
{
	axis2_ws_info_t ws_info;
    
    	
} axis2_ws_info_impl_t;

#define AXIS2_INTF_TO_IMPL(ws_info) \
    ((axis2_ws_info_impl_t *) ws_info)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_ws_info_free (axis2_ws_info_t *ws_info, 
                            axis2_env_t **env);



                                
/************************** End of function prototypes ************************/

axis2_ws_info_t * AXIS2_CALL 
axis2_ws_info_create (axis2_env_t **env)
{
    axis2_ws_info_impl_t *ws_info_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	ws_info_impl = (axis2_ws_info_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_ws_info_impl_t));
	
	
	if(NULL == ws_info_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    ws_info_impl->ws_info.ops = NULL;
    
	ws_info_impl->ws_info.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_ws_info_ops_t));
	if(NULL == ws_info_impl->ws_info.ops)
    {
        axis2_ws_info_free(&(ws_info_impl->ws_info), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	ws_info_impl->ws_info.ops->free =  axis2_ws_info_free;
	
	return &(ws_info_impl->ws_info);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_ws_info_free (axis2_ws_info_t *ws_info, 
                            axis2_env_t **env)
{
    axis2_ws_info_impl_t *feature_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(ws_info, env, AXIS2_FAILURE);
    
    feature_impl = AXIS2_INTF_TO_IMPL(ws_info);
    
	if(NULL != ws_info->ops)
        AXIS2_FREE((*env)->allocator, ws_info->ops);
    
    AXIS2_FREE((*env)->allocator, feature_impl);
    feature_impl = NULL;
    
	return AXIS2_SUCCESS;
}
