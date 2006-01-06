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
 
#ifndef AXIS2_CLASS_LOADER_H
#define AXIS2_CLASS_LOADER_H

/**
  * @file axis2_class_loader.h
  * @brief axis2 class loader interface
  */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_error.h>
#include <axis2.h>
#include <axis2_dll_desc.h>
#include <axis2_svc_skeleton.h>
#include <axis2_param.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_class_loader Class Loader 
 * @ingroup axis2_util
 * @{
 */
    
axis2_status_t AXIS2_CALL
axis2_class_loader_init(axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_class_loader_delete_dll (axis2_env_t **env,
                                    struct axis2_dll_desc *dll_desc);

void *AXIS2_CALL
axis2_class_loader_create_dll (axis2_env_t **env,
                                axis2_param_t *impl_info_param);
    
 
/** @} */
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_CLASS_LOADER_H */
