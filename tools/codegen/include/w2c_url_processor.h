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
  
#ifndef W2C_URL_PROCESSOR_H
#define W2C_URL_PROCESSOR_H

/**
 * @file w2c_url_processor.h
 * @brief handles Axis2/C w2c url processing
 */

#include <stdio.h>
#include <axis2_utils.h>

#ifdef __cplusplus

extern "C"
{
#endif

/** 
 * @defgroup w2c_url_procesor w2c url processing
 * @ingroup w2c_utils
 * @{
 */    

#define W2C_URL_PROCESSOR_DEFAULT_PACKAGENAME "org.apache.axis2" 

/**
 * create a package name from namespace considering all syntax issues.
 * @param env double pointer to environment struct. MUST NOT be NULL
 * @param ns namespace to feed
 * @return made package name
 */

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_url_processor_make_package_name ( 
    const axis2_env_t *env,
    axis2_char_t *ns);


    
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_URL_PROCESSOR_H */
