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
 
#include <axis2_arch_reader.h>
#include <string.h>
#include <axis2_class_loader.h>

/** 
 * @brief
 * To do the common tasks for all Builder
 */ 
typedef struct axis2_arch_reader_impl
{
	axis2_arch_reader_t arch_reader;
    
    	
} axis2_arch_reader_impl_t;

#define AXIS2_INTF_TO_IMPL(arch_reader) \
    ((axis2_arch_reader_impl_t *) arch_reader)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_arch_reader_free (axis2_arch_reader_t *arch_reader, 
                            axis2_env_t **env);



                                
/************************** End of function prototypes ************************/

axis2_arch_reader_t * AXIS2_CALL 
axis2_arch_reader_create_with_input_stream_and_dep_engine (axis2_env_t **env)
{
    axis2_arch_reader_impl_t *arch_reader_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	arch_reader_impl = (axis2_arch_reader_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_arch_reader_impl_t));
	
	
	if(NULL == arch_reader_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    arch_reader_impl->arch_reader.ops = NULL;
    
	arch_reader_impl->arch_reader.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_arch_reader_ops_t));
	if(NULL == arch_reader_impl->arch_reader.ops)
    {
        axis2_arch_reader_free(&(arch_reader_impl->arch_reader), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	arch_reader_impl->arch_reader.ops->free =  axis2_arch_reader_free;
	
	return &(arch_reader_impl->arch_reader);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_arch_reader_free (axis2_arch_reader_t *arch_reader, 
                            axis2_env_t **env)
{
    axis2_arch_reader_impl_t *feature_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(arch_reader, env, AXIS2_FAILURE);
    
    feature_impl = AXIS2_INTF_TO_IMPL(arch_reader);
    
	if(NULL != arch_reader->ops)
        AXIS2_FREE((*env)->allocator, arch_reader->ops);
    
    AXIS2_FREE((*env)->allocator, feature_impl);
    feature_impl = NULL;
    
	return AXIS2_SUCCESS;
}
