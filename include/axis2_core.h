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
 
#ifndef AXIS2_CORE_H
#define AXIS2_CORE_H

/**
 * @file axis2_core.h
 * @brief axis2 DESCRIPTION CORE
 * Contain declaration of all structs used in
 * description, context and engine modules.
 */
 
#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C" 
{
#endif

/************************ Message exchange patterns ***************************/
	
/* Default message excange pattern	*/
#define MEP_URI_IN_OUT "http://www.w3.org/2004/08/wsdl/in-out"
	
#define MEP_URI_IN_OPTIONAL_OUT "http://www.w3.org/2004/08/wsdl/in-opt-out"

#define MEP_URI_IN_ONLY "http://www.w3.org/2004/08/wsdl/in-only"

#define MEP_URI_ROBUST_IN_ONLY "http://www.w3.org/2004/08/wsdl/robust-in-only"

#define MEP_URI_IN_OPTIONAL_OUT "http://www.w3.org/2004/08/wsdl/in-opt-out"
	
#define MEP_URI_OUT_ONLY "http://www.w3.org/2004/08/wsdl/out-only"
	
#define MEP_URI_ROBUST_OUT_ONLY "http://www.w3.org/2004/08/wsdl/robust-out-only"

#define MEP_URI_OUT_IN "http://www.w3.org/2004/08/wsdl/out-in"	
	
#define MEP_URI_OUT_OPTIONAL_IN "http://www.w3.org/2004/08/wsdl/out-opt-in"	
	
/************************ End of message exchange patters *********************/	
	
	
/** @cond */
struct axis2_description_param_ops_s;
struct axis2_description_param_s;
/** @endcond */
typedef struct axis2_description_param_ops_s axis2_description_param_ops_t;
typedef struct axis2_description_param_s axis2_description_param_t;



/** @cond */
struct axis2_description_param_include_ops_s;
struct axis2_description_param_include_s;
/** @endcond */		
typedef struct axis2_description_param_include_ops_s 
		axis2_description_param_include_ops_t;	
typedef struct axis2_description_param_include_s 
		axis2_description_param_include_t;


/** @cond */
struct axis2_description_operation_s;
struct axis2_description_operation_ops_s;
/** @endcond */
typedef struct axis2_description_operation_s axis2_description_operation_t;
typedef struct axis2_description_operation_ops_s 
		axis2_description_operation_ops_t;


/** @cond */
struct axis2_description_service_s;
struct axis2_description_service_ops_s;
/** @endcond */
typedef struct axis2_description_service_s axis2_description_service_t;
typedef struct axis2_description_service_ops_s 
		axis2_description_service_ops_t;


/** @cond */
struct axis2_description_servicegroup_s;
struct axis2_description_servicegroup_ops_s;
/** @endcond */	
typedef struct axis2_description_servicegroup_s axis2_description_servicegroup_t;
typedef struct axis2_description_servicegroup_ops_s 
		axis2_description_servicegroup_ops_t;


/** @cond */
struct axis2_context_msg_ctx_s;
struct axis2_context_msg_ctx_ops_s;
/** @endcond */
typedef struct axis2_context_msg_ctx_s axis2_context_msg_ctx_t;
typedef struct axis2_context_msg_ctx_ops_s axis2_context_msg_ctx_ops_t;
	
/** @cond */
struct axis2_engine_msg_receiver_s;
struct axis2_engine_msg_receiver_ops_s;
/** @endcond */
typedef struct axis2_engine_msg_receiver_s axis2_engine_msg_receiver_t;
typedef struct axis2_engine_msg_receiver_ops_s axis2_engine_msg_receiver_ops_t;
	
/** @cond */
struct axis2_engine_config_s;
struct axis2_engine_config_ops_s;
/** @endcond */
typedef struct axis2_engine_config_s axis2_engine_config_t;
typedef struct axis2_engine_config_ops_s axis2_engine_config_ops_t;
	
#ifdef __cplusplus
}
#endif

#endif /* AXIS2_CORE_H */
