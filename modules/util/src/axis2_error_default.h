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

#ifndef AXIS2_ERROR_DEFAULT_H
#define AXIS2_ERROR_DEFAULT_H

#include <axis2_defines.h>
#include <axis2_allocator.h>
#include <axis2_error.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_error;
    struct axis2_error_ops;

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
    AXIS2_DECLARE(axis2_error_t *) axis2_error_create (axis2_allocator_t * allocator);

#define AXIS2_ERROR_FREE(error) ((error->ops)->free(error))
#define AXIS2_ERROR_GET_MESSAGE(error) ((error)->ops->axis2_error_ops_get_message())

/** @} */
    
#ifdef __cplusplus
}
#endif


#endif                          /* AXIS2_ERROR_DEFAULT_H */
