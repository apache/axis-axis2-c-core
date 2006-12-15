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

#ifndef AXIS2_CONST_H
#define AXIS2_CONST_H

/**
 * @file axis2.h
 * @brief Axis2c specific global declarations
 */

#include <axis2_env.h>
#include <axis2_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** @defgroup axis2 Axis2/C project
  * @{
  * @}
  */
    
/** \mainpage Axis2/C API Documentation
  *
  * \section intro_sec Introduction
  *
  * This is the API documetation of Axis2/C, a SOAP engine written in C.
  * This implementation is based on the popular Axis2 architecture. 
  * <p>We welcome your feedback on this implementation and documentation.
  * Please send your feedback to <a href="mailto:axis-c-user@ws.apache.org">
  * axis-c-user@ws.apache.org</a> and please remember to prefix the subject
  * of the mail with [Axis2].
  *
  */    
    
/******************************************************************************/
/********************Axis2 specific constants**********************************/
/******************************************************************************/


/**
 * Field SOAP_STYLE_RPC_ENCODED
 */
#define AXIOM_SOAP_STYLE_RPC_ENCODED 1000

/**
 * Field SOAP_STYLE_RPC_LITERAL
 */
/*#define AXIOM_SOAP_STYLE_RPC_LITERAL 1001 */

/**
 * Field SOAP_STYLE_DOC_LITERAL_WRAPPED
 */
#define AXIOM_SOAP_STYLE_DOC_LITERAL_WRAPPED 1002

#define AXIS2_SCOPE "scope"

/**
 * Field APPLICATION_SCOPE
 */
#define AXIS2_APPLICATION_SCOPE "application"

/**
 * Field SESSION_SCOPE
 */
#define AXIS2_SESSION_SCOPE "session"

/**
 * Field GLOBAL_SCOPE
 */
#define AXIS2_MESSAGE_SCOPE "message"

/**
 * Field PHASE_SERVICE
 */
#define AXIS2_PHASE_SERVICE "service"

/**
 * Field PHASE_TRANSPORT
 */
#define AXIS2_PHASE_TRANSPORT "transport"

/**
 * Field PHASE_GLOBAL
 */
#define AXIS2_PHASE_GLOBAL "global"

/**
 * Field SESSION_CONTEXT_PROPERTY
 */
#define AXIS2_SESSION_CONTEXT_PROPERTY "SessionContext"

/**
 * Field TRANSPORT_TCP
 */
#define AXIS2_TRANSPORT_TCP "tcp"

/**
 * Field TRANSPORT_HTTP
 */
#define AXIS2_TRANSPORT_HTTP "http"



#define AXIS2_TRANSPORT_MAIL "mail"

#define AXIS2_TRANSPORT_JMS "jms"

#define AXIS2_TRANSPORT_LOCAL "local"

/**
 * Field LISTSERVICES
 */
#define AXIS2_REQUEST_URL_PREFIX "/services"

#define AXIS2_LISTSERVICES "listServices"

#define AXIS2_LIST_SERVICE_FOR_MODULE_ENGAMNET "listop"


/**
 * List service for adminpagse
 */
#define AXIS2_ADMIN_LISTSERVICES "listService"

#define AXIS2_LIST_MODULES "listModules"

#define AXIS2_LIST_GLOABLLY_ENGAGED_MODULES "globalModules"

#define AXIS2_LIST_PHASES "listPhases"

#define AXIS2_ENGAGE_GLOBAL_MODULE "engagingglobally"
#define AXIS2_ENGAGE_MODULE_TO_SERVICE "engageToService"

#define AXIS2_ENGAGE_MODULE_TO_SERVICE_GROUP "engageToServiceGroup"

#define AXIS2_ADMIN_LOGIN "adminlogin"

#define AXIS2_LIST_CONTEXTS "listContexts"
#define AXIS2_LOGOUT "logout"

#define AXIS2_VIEW_GLOBAL_HANDLERS "viewGlobalHandlers"
#define AXIS2_SELECT_SERVICE "selectService"
#define AXIS2_EDIR_SERVICE_PARA "editServicepara"
#define AXIS2_SELECT_SERVICE_FOR_PARA_EDIT "selectServiceParaEdit"
#define AXIS2_VIEW_SERVICE_HANDLERS "viewServiceHandlers"
#define AXIS2_LIST_SERVIC_GROUPS "listServciceGroups"

/**
 * Field SERVICE_MAP
 */
#define AXIS2_SERVICE_MAP "servicemap"
#define AXIS2_SERVICE_GROUP_MAP "serviceGroupmap"

#define AXIS2_CONFIG_CONTEXT "config_context"
#define AXIS2_ACTION_MAPPING "actionMapping"
#define AXIS2_OUTPUT_ACTION_MAPPING "outputActionMapping"
#define AXI2_FAULT_ACTION_MAPPING "faultActionMapping"
    
#define AXIS2_SERVICE "service"

#define AXIS2_OPEARTION_MAP "opmap"
/**
 * Field Available modules
 */
#define AXIS2_MODULE_MAP "modulemap"

#define AXIS2_SELECT_SERVICE_TYPE "SELECT_SERVICE_TYPE"

#define AXIS2_GLOBAL_HANDLERS "axisconfig"
#define AXIS2_SERVICE_HANDLERS "serviceHandlers"

#define AXIS2_PHASE_LIST "phaseList"

#define AXIS2_LIST_OPS_FOR_THE_SERVICE "listOperations"

#define AXIS2_REMOVE_SERVICE "removeService"

#define AXIS2_ENGAGE_STATUS "engagestatus"

/**
 * Errorness servcie
 */
#define AXIS2_ERROR_SERVICE_MAP "errprservicemap"
#define AXIS2_ERROR_MODULE_MAP "errormodulesmap"

#define AXIS2_IS_FAULTY "Fault"

#define AXIS2_MODULE_ADDRESSING "addressing"

#define AXIS2_USER_NAME "userName"
#define AXIS2_PASSWORD "password"

/**
 * Field SINGLE_SERVICE
 */
#define AXIS2_SINGLE_SERVICE "singleservice"
#define AXIS2_WSDL_CONTENT "wsdl"

/**
 * Field METHOD_NAME_ESCAPE_CHARACTOR
 */
/* static const char METHOD_NAME_ESCAPE_CHARACTOR '?' */

#define AXIS2_LOGGED "Logged"


/* static const char SERVICE_NAME_SPLIT_CHAR':' */


/*********************Configuration *******************************************/

#define AXIS2_ENABLE_REST "enableREST"
#define AXIS2_ENABLE_REST_THROUGH_GET "restThroughGet"
/* globally enable MTOM */
#define AXIS2_ENABLE_MTOM "enableMTOM"
#define AXIS2_ATTACHMENT_TEMP_DIR "attachmentDIR"
#define AXIS2_CACHE_ATTACHMENTS "cacheAttachments"
#define AXIS2_FILE_SIZE_THRESHOLD "sizeThreshold"

/******************************************************************************/

#define AXIS2_VALUE_TRUE "true"
#define AXIS2_VALUE_FALSE "false"
#define AXIS2_CONTAINER_MANAGED "ContainerManaged"
#define AXIS2_RESPONSE_WRITTEN "CONTENT_WRITTEN"

#define AXIS2_TESTING_PATH "target/test-resources/"

#define AXIS2_TESTING_REPOSITORY "target/test-resources/samples"


/*************************** REST_WITH_GET ************************************/

#define AXIS2_GET_PARAMETER_OP "op"
#define AXIS2_GET_PARAMETER_URL "http://ws.apache.org/goGetWithREST"

/******************************************************************************/


#define AXIS2_NAMESPACE_PREFIX "axis2"
#define AXIS2_NAMESPACE_URI "http://ws.apache.org/namespaces/axis2"

#define AXIS2_SVC_GRP_ID "ServiceGroupId"    



#ifdef __cplusplus
}
#endif

#endif /* AXIS2_CONST_H */
