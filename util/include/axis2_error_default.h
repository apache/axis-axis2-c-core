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

#ifndef AXIS2_ERROR_DEFAULT_H
#define AXIS2_ERROR_DEFAULT_H

#include <axis2_utils_defines.h>
#include <axis2_allocator.h>
#include <axis2_error.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_error Error
 * @ingroup axis2_util 
 * @{
 */


  /**
    * Creates an error struct
    * @param allocator allocator to be used. Mandatory, cannot be NULL    
    * @return pointer to the newly created error struct 
    */
    AXIS2_EXTERN axis2_error_t *AXIS2_CALL axis2_error_create (axis2_allocator_t * allocator);


/** @} */
    
#ifdef __cplusplus
}
#endif


#endif                          /* AXIS2_ERROR_DEFAULT_H */
