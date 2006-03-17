/**
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

#ifndef AXIS2_TYPES_H
#define AXIS2_TYPES_H

#include <axis2_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_types Types
 * @ingroup axis2_util 
 * @{
 */

    AXIS2_DECLARE(int)
    axis2_atoi (const char *s);

#define AXIS2_ATOI(s) axis2_atoi(s)

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_TYPES_H */
