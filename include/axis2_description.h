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

#ifndef AXIS2_DESCRIPTION_H
#define AXIS2_DESCRIPTION_H

/**
 * @file axis2_axis2_description.h
 * @brief axis2 DESCRIPTION
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_desc description
 * @ingroup axis2
 * @{
 * @}
 */

/*********************************** Constansts********************************/

/**
 * Field EXECUTION_CHAIN_KEY
 */
#define AXIS2_EXECUTION_CHAIN_KEY  "EXECUTION_CHAIN_KEY"

/**
 * Field EXECUTION_OUT_CHAIN_KEY
 */
#define AXIS2_EXECUTION_OUT_CHAIN_KEY "EXECUTION_OUT_CHAIN_KEY"

/**
 * Field EXECUTION_FAULT_CHAIN_KEY
 */
#define AXIS2_EXECUTION_FAULT_CHAIN_KEY "EXECUTION_FAULT_CHAIN_KEY"

/**
 * Field MODULEREF_KEY
 */
#define AXIS2_MODULEREF_KEY  "MODULEREF_KEY"

/**
 * Field OP_KEY
 */
#define AXIS2_OP_KEY  "OP_KEY"

/**
 * Field CLASSLOADER_KEY
 */
#define AXIS2_CLASSLOADER_KEY  "CLASSLOADER_KEY"

/**
 * Field CONTEXTPATH_KEY
 */
#define AXIS2_CONTEXTPATH_KEY  "CONTEXTPATH_KEY"

/**
 * Field PROVIDER_KEY
 */
#define AXIS2_MESSAGE_RECEIVER_KEY  "PROVIDER_KEY"

/**
 * Field STYLE_KEY
 */
#define AXIS2_STYLE_KEY  "STYLE_KEY"

/**
 * Field PARAMETER_KEY
 */
#define AXIS2_PARAMETER_KEY  "PARAMETER_KEY"

/**
 * Field INFLOW_KEY
 */
#define AXIS2_INFLOW_KEY  "INFLOW_KEY"

/**
 * Field OUTFLOW_KEY
 */
#define AXIS2_OUTFLOW_KEY  "OUTFLOW_KEY"

/**
 * Field IN_FAULTFLOW_KEY
 */
#define AXIS2_IN_FAULTFLOW_KEY  "IN_FAULTFLOW_KEY"

/**
 * Field OUT_FAULTFLOW_KEY
 */
#define AXIS2_OUT_FAULTFLOW_KEY  "OUT_FAULTFLOW_KEY"

/**
 * Field PHASES_KEY
 */
#define AXIS2_PHASES_KEY  "PHASES_KEY"

/**
 * Field SERVICE_CLASS
 */
#define AXIS2_SERVICE_CLASS  "ServiceClass"

/**
 * Field SERVICE_CLASS_NAME
 */
#define AXIS2_SERVICE_CLASS_NAME "SERVICE_CLASS_NAME"


/*********************************** Constants*********************************/


#ifdef __cplusplus
}
#endif

#endif /* AXIS2_DESCRIPTION_H */
