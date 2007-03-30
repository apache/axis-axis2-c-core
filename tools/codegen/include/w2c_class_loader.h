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
  
#ifndef W2C_CLASS_LOADER_H
#define W2C_CLASS_LOADER_H

/**
 * @file w2c_class_loader.h
 * @brief handles reflection in C
 */

#include <stdio.h>
#include <axutil_utils.h>
#include <axutil_dll_desc.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_class_loader handles reflection in C
 * @ingroup w2c_utils
 * @{
 */  
/* predefined paths for the codegen libraries -relative to $AXIS2C_HOME*/ 
#define W2C_CLASS_LOADER_WRITER_PATH "/lib/codegen/"
#define W2C_CLASS_LOADER_EXTENSION_PATH "/lib/codegen/"
#define W2C_CLASS_LOADER_EMITTER_PATH "/lib/codegen/"

/**
 * creates/ returns a object from class name.
 * @param env pointer to environment struct. MUST NOT be NULL
 * @param class name
 * @param inter_path path relative to $AXIS2C_HOME
 * @param pdll_desc double pointer to the dll description
 * @return the object that was created
 */
AXIS2_EXTERN void* AXIS2_CALL
w2c_class_loader_get_object_from_class_name
                         (const axutil_env_t *env,
                          axis2_char_t *class_name,
                          axis2_char_t *inter_path,
                          axutil_dll_desc_t **pdll_desc);

/**
 * free the loaded class.
 * @param env pointer to environment struct. MUST NOT be NULL
 * @param dll_desc dll description
 * @return AXIS2_SUCCESS on success or else AXIS2_FAILURE on failure
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
w2c_class_loader_free_loaded_class(
                          const axutil_env_t *env,
                          axutil_dll_desc_t *dll_desc);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_CLASS_LOADER_H */
