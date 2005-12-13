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

#ifndef AXIS2_H
#define AXIS2_H

/**
 * @file axis2.h
 * @brief Axis2c specific global declarations
 */

#include <stdlib.h>
#include <stdio.h>
#include <axis2_env.h>

/** This macro is called to check whether structure on which function is called
 *  is NULL and to check whether the environment structure passed is valid.
 * @param object structure on which function is called
 * @param env environment to be checked for validity
 * @param error_return If function return a status it should pass here 
 *        AXIS2_FAILURE. If function return a type pointer it should
 *        pass NULL
 * @return If function return a status code return AXIS2_SUCCESS. Else if
 *         function return a type pointer return NULL
 */
#define AXIS2_FUNC_PARAM_CHECK(object, env, error_return) \
    AXIS2_ENV_CHECK(env, error_return);\
    if (!object) \
    { \
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM); \
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE); \
        return error_return; \
    } \
    else \
    { \
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_SUCCESS); \
    }
 
/**This macro is called to check whether an object is NULL.
 * if object is NULL error number and status code is set
 * @param object object to be check for NULL
 * @param error_return If function return a status it should pass here 
 *        AXIS2_FAILURE. If function return a type pointer it should
 *        pass NULL
 * @return If function return a status code return AXIS2_SUCCESS. Else if
 *         function return a type pointer return NULL
 */
#define AXIS2_PARAM_CHECK(error, object, error_return) \
    if (!object) \
    { \
        AXIS2_ERROR_SET_ERROR_NUMBER(error, AXIS2_ERROR_INVALID_NULL_PARAM); \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_FAILURE); \
        return error_return; \
    } \
    else \
    { \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_SUCCESS); \
    } 

/**This macro is used to handle error situation. 
 * @param error_number Error number for the error occured
 * @param error_return If function return a status it should pass here 
 *        AXIS2_FAILURE. If function return a type pointer it should
 *        pass NULL
 * @return If function return a status code return AXIS2_SUCCESS. Else if
 *         function return a type pointer return NULL
 */    
#define AXIS2_ERROR_SET(error, error_number, error_return) \
    { \
        AXIS2_ERROR_SET_ERROR_NUMBER(error, error_number); \
        AXIS2_ERROR_SET_STATUS_CODE(error, AXIS2_FAILURE); \
        return error_return; \
    }      

#ifdef __cplusplus
extern "C"
{
#endif


/** @defgroup axis2 Axis2/C project
  *  @{
 */

typedef enum axis2_op_type
{
    /** axis2_wsdl_op */
    AXIS2_WSDL_OP = 0,
    /** axis2_op */
    AXIS2_OP
    
}axis2_op_type_t;

typedef enum axis2_svc_type
{
    /** axis2_wsdl_svc */
    AXIS2_WSDL_SVC = 0,
    /** axis2_svc */
    AXIS2_SVC
    
}axis2_svc_type_t;

/******************************************************************************/
/********************Axis2 specific constants**********************************/
/******************************************************************************/
/**
 * Field SOAP_STYLE_RPC_ENCODED
 */
/* static const int SOAP_STYLE_RPC_ENCODED = 1000;*/

/**
 * Field SOAP_STYLE_RPC_LITERAL
 */
/*static const int SOAP_STYLE_RPC_LITERAL = 1001; */

/**
 * Field SOAP_STYLE_DOC_LITERAL_WRAPPED
 */
/* static const int SOAP_STYLE_DOC_LITERAL_WRAPPED = 1002; */

/**
 * Field APPLICATION_SCOPE
 */
/* static const axis2_char_t * APPLICATION_SCOPE = "application"; */

/**
 * Field SESSION_SCOPE
 */
/* static const axis2_char_t * SESSION_SCOPE = "session"; */

/**
 * Field GLOBAL_SCOPE
 */
/* static const axis2_char_t * MESSAGE_SCOPE = "message"; */

/**
 * Field PHASE_SERVICE
 */
/* static const axis2_char_t * PHASE_SERVICE = "service"; */

/**
 * Field PHASE_TRANSPORT
 */
/* static const axis2_char_t * PHASE_TRANSPORT = "transport"; */

/**
 * Field PHASE_GLOBAL
 */
/* static const axis2_char_t * PHASE_GLOBAL = "global"; */

/**
 * Field SESSION_CONTEXT_PROPERTY
 */
/* static const axis2_char_t * SESSION_CONTEXT_PROPERTY = "SessionContext"; */

/**
 * Field TRANSPORT_TCP
 */
/* static const axis2_char_t * TRANSPORT_TCP = "tcp"; */

/**
 * Field TRANSPORT_HTTP
 */
/* static const axis2_char_t * TRANSPORT_HTTP = "http"; */



/* static const axis2_char_t * TRANSPORT_MAIL = "mail"; */

/* static const axis2_char_t * TRANSPORT_JMS = "jms"; */

/* static const axis2_char_t * TRANSPORT_LOCAL = "local"; */

/**
 * Field LISTSERVICES
 */
/* static const axis2_char_t * REQUEST_URL_PREFIX = "/services"; */

/* static const axis2_char_t * LISTSERVICES = "listServices"; */

/* static const axis2_char_t * LIST_SERVICE_FOR_MODULE_ENGAMNET = "listop"; */


/**
 * List service for adminpagse
 */
/* static const axis2_char_t * ADMIN_LISTSERVICES = "listService"; */

/* static const axis2_char_t * LIST_MODULES = "listModules"; */

/* static const axis2_char_t * LIST_GLOABLLY_ENGAGED_MODULES = "globalModules"; */

/* static const axis2_char_t * LIST_PHASES = "listPhases"; */

/* static const axis2_char_t * ENGAGE_GLOBAL_MODULE = "engagingglobally"; */
/* static const axis2_char_t * ENGAGE_MODULE_TO_SERVICE = "engageToService"; */

/* static const axis2_char_t * ENGAGE_MODULE_TO_SERVICE_GROUP = "engageToServiceGroup"; */

/* static const axis2_char_t * ADMIN_LOGIN = "adminlogin"; */

/* static const axis2_char_t * LIST_CONTEXTS = "listContexts"; */
/* static const axis2_char_t * LOGOUT = "logout"; */

/* static const axis2_char_t * VIEW_GLOBAL_HANDLERS = "viewGlobalHandlers"; */
/* static const axis2_char_t * SELECT_SERVICE = "selectService"; */
/* static const axis2_char_t * EDIR_SERVICE_PARA = "editServicepara"; */
/* static const axis2_char_t * SELECT_SERVICE_FOR_PARA_EDIT = "selectServiceParaEdit"; */
/* static const axis2_char_t * VIEW_SERVICE_HANDLERS = "viewServiceHandlers"; */
/* static const axis2_char_t * LIST_SERVIC_GROUPS = "listServciceGroups"; */

/**
 * Field SERVICE_MAP
 */
/* static const axis2_char_t * SERVICE_MAP = "servicemap"; */
/* static const axis2_char_t * SERVICE_GROUP_MAP = "serviceGroupmap"; */

/* static const axis2_char_t * CONFIG_CONTEXT = "config_context"; */

/* static const axis2_char_t * SERVICE = "service"; */

/* static const axis2_char_t * OPEARTION_MAP = "opmap"; */
/**
 * Field Available modules
 */
/* static const axis2_char_t * MODULE_MAP = "modulemap"; */

/* static const axis2_char_t * SELECT_SERVICE_TYPE = "SELECT_SERVICE_TYPE"; */

/* static const axis2_char_t * GLOBAL_HANDLERS = "axisconfig"; */
/* static const axis2_char_t * SERVICE_HANDLERS = "serviceHandlers"; */

/* static const axis2_char_t * PHASE_LIST = "phaseList"; */

/* static const axis2_char_t * LIST_OPS_FOR_THE_SERVICE = "listOperations"; */

/* static const axis2_char_t * REMOVE_SERVICE = "removeService"; */

/* static const axis2_char_t * ENGAGE_STATUS = "engagestatus"; */

/**
 * Errorness servcie
 */
/* static const axis2_char_t * ERROR_SERVICE_MAP = "errprservicemap"; */
/* static const axis2_char_t * ERROR_MODULE_MAP = "errormodulesmap"; */

/* static const axis2_char_t * IS_FAULTY = "Fault"; */

/* static const axis2_char_t * MODULE_ADDRESSING = "addressing"; */

/* static const axis2_char_t * USER_NAME = "userName"; */
/* static const axis2_char_t * PASSWORD = "password"; */

/**
 * Field SINGLE_SERVICE
 */
/* static const axis2_char_t * SINGLE_SERVICE = "singleservice"; */
/* static const axis2_char_t * WSDL_CONTENT = "wsdl"; */

/**
 * Field METHOD_NAME_ESCAPE_CHARACTOR
 */
/* static const char METHOD_NAME_ESCAPE_CHARACTOR = '?'; */

/* static const axis2_char_t * LOGGED = "Logged"; */


/* static const char SERVICE_NAME_SPLIT_CHAR =':'; */


/*********************Configuration *******************************************/

/* static const axis2_char_t * ENABLE_REST = "enableREST"; */
/* static const axis2_char_t * ENABLE_REST_THROUGH_GET="restThroughGet"; */
/* globally enable MTOM */
/* static const axis2_char_t * ENABLE_MTOM = "enableMTOM"; */
/* static const axis2_char_t * ATTACHMENT_TEMP_DIR = "attachmentDIR"; */
/* static const axis2_char_t * CACHE_ATTACHMENTS = "cacheAttachments"; */
/* static const axis2_char_t * FILE_SIZE_THRESHOLD = "sizeThreshold"; */

/******************************************************************************/

/* static const axis2_char_t * VALUE_TRUE = "true"; */
/* static const axis2_char_t * VALUE_FALSE = "false"; */
/* static const axis2_char_t * CONTAINER_MANAGED = "ContainerManaged"; */
/* static const axis2_char_t * RESPONSE_WRITTEN = "CONTENT_WRITTEN"; */
#define WSA_ACTION "wsamapping"


/* static const axis2_char_t * TESTING_PATH = "target/test-resources/"; */

/* static const axis2_char_t * TESTING_REPOSITORY = "target/test-resources/samples"; */


/*************************** REST_WITH_GET ************************************/

/* static const axis2_char_t * GET_PARAMETER_OP = "op"; */
/* static const axis2_char_t * GET_PARAMETER_URL = "http://ws.apache.org/goGetWithREST"; */

/******************************************************************************/


/* static const axis2_char_t * AXIS2_NAMESPACE_PREFIX = "axis2"; */
/* static const axis2_char_t * AXIS2_NAMESPACE_URI = "http://ws.apache.org/namespaces/axis2"; */

/* static const axis2_char_t * SERVICE_GROUP_ID = "ServiceGroupId"; */    

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_H */
