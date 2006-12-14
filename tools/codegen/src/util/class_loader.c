/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_class_loader.h>
#include <w2c_class_loader.h>
#include <w2c_string.h>
#include <w2c_messages.h>

AXIS2_EXTERN void* AXIS2_CALL
w2c_class_loader_get_object_from_class_name
                         (const axis2_env_t *env,
                          char *class_name,
                          axis2_char_t *inter_path,
                          axis2_dll_desc_t **pdll_desc)
{
    axis2_char_t *dll_name = NULL;
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_char_t *file_path = NULL;
    void *obj = NULL;

	AXIS2_ENV_CHECK(env, NULL);
	AXIS2_PARAM_CHECK(env->error, class_name, NULL);

    file_path = getenv("AXIS2C_HOME");
    file_path = AXIS2_STRDUP( file_path, env);
    dll_desc = axis2_dll_desc_create(env);
    *pdll_desc = dll_desc;

    dll_name=
       AXIS2_DLL_DESC_CREATE_PLATFORM_SPECIFIC_DLL_NAME(
                 dll_desc,  env, class_name );
    if ( NULL == dll_name )
    {
        w2c_messages_print_n_log_error ( env, "utils.classloadFailed");
        return NULL;
    }
    file_path = w2c_string_add_string(file_path, inter_path, env );
    file_path = w2c_string_add_string(file_path, dll_name, env );

    AXIS2_DLL_DESC_SET_NAME(dll_desc, env, file_path);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, -1); /* not a standard type */

    impl_info_param = axis2_param_create(env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);

    axis2_class_loader_init(env);
	obj = axis2_class_loader_create_dll(env, impl_info_param);

    AXIS2_FREE ( env->allocator, file_path);
    return obj;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
w2c_class_loader_free_loaded_class(
                          const axis2_env_t *env,
                          axis2_dll_desc_t *dll_desc )
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dll_desc, AXIS2_FAILURE);

    axis2_class_loader_delete_dll(env, dll_desc);
    /*AXIS2_DLL_DESC_FREE ( dll_desc, env );*/

    return AXIS2_SUCCESS;
}
