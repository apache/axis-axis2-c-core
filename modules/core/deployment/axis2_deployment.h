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
 
#ifndef AXIS2_DEPLOYMENT_H
#define AXIS2_DEPLOYMENT_H

/**
 * @file axis2_axis2_deployment.h
 * @brief axis2 deployment
 */
 
#include <axis2.h>
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

/*********************************** Constansts********************************/

/**
 * DeployCons interface is to keep constent value required for Deployemnt
 */

#define AXIS2_SVC 0 /* if it is a service */
#define AXIS2_MODULE 1 /* if it is a module */

#define AXIS2_META_INF "META-INF"
#define AXIS2_SVC_XML "services.xml"
#define AXIS2_SVC_WSDL_NAME "service.wsdl"
#define AXIS2_SVC_WSDL_WITH_FOLDER "META-INF/service.wsdl"
#define AXIS2_MODULE_XML "modules/module.xml"
#define AXIS2_PARAMETERST "parameter" /* paramater start tag */
#define AXIS2_LISTENERST "listener" /* paramater start tag */
#define AXIS2_HANDLERST "handler"
#define AXIS2_MODULEST "module"
#define AXIS2_MODULECONFIG "moduleConfig"
#define AXIS2_PHASEST "phase"
#define AXIS2_PHASE_ORDER "phaseOrder"
#define AXIS2_TYPEMAPPINGST "typeMapping" /* typeMapping start tag */
#define AXIS2_BEANMAPPINGST "beanMapping" /* beanMapping start tag */
#define AXIS2_OPERATIONST "operation" /* operation start tag */
#define AXIS2_INFLOWST "inflow" /* inflow start tag */
#define AXIS2_OUTFLOWST "outflow" /* outflowr start tag */
#define AXIS2_IN_FAILTFLOW "INfaultflow" /* faultflow start tag */
#define AXIS2_OUT_FAILTFLOW "Outfaultflow" /* faultflow start tag */

/* #define AXIS2_FOLDE_NAME "D:/Axis 2.0/projects/Deployement/test-data" */
#define AXIS2_MODULE_PATH "/modules/"
#define AXIS2_SVC_PATH "/services/"

/* for jws file extension */
#define AXIS2_JWS_EXTENSION ".jws"

#define AXIS2_SVCTAG "service"
#define AXIS2_CONFIG "axisconfig"

/* for serviemetadat */
#define AXIS2_STYLENAME "style"
#define AXIS2_CONTEXTPATHNAME "contextPath"

/* for parameters */
#define AXIS2_ATTNAME "name"
#define AXIS2_ATTLOCKED "locked"
#define AXIS2_TYPE "type"

/* for operatiins */
#define AXIS2_ATQNAME "name"
#define AXIS2_ATUSE "use"
#define AXIS2_MEP "mep"

/* for handlers */
#define AXIS2_REF "ref"
#define AXIS2_CLASSNAME "class"
#define AXIS2_BEFORE "before"
#define AXIS2_AFTER "after"
#define AXIS2_PHASE "phase"
#define AXIS2_PHASEFIRST "phaseFirst"
#define AXIS2_PHASELAST "phaseLast"
#define AXIS2_ORDER "order"  /* to resolve the order tag */
#define AXIS2_DESCRIPTION "description"

#define AXIS2_TRANSPORTSTAG "transports"
#define AXIS2_TRANSPORTTAG "transport"
#define AXIS2_TRANSPORTSENDER "transportSender"
#define AXIS2_TRANSPORTRECEIVER "transportReceiver"

#define AXIS2_MESSAGERECEIVER "messageReceiver"


#define AXIS2_HOTDEPLOYMENT "hotdeployment"
#define AXIS2_HOTUPDATE "hotupdate"
#define AXIS2_EXTRACTSVCARCHIVE "extractServiceArchive"
#define AXIS2_DISPATCHER "dispatcher"

#define AXIS2_STORAGE "axisStorage"

/* element in a services.xml */
#define AXIS2_SVC_ELEMENT "service"
#define AXIS2_SVC_GRP_ELEMENT "serviceGroup"

#define AXIS2_SERVER_XML_FILE "axis2.xml"

#define AXIS2_MODULE_FOLDER "modules"
#define AXIS2_SERVICE_FOLDER "services"
#define AXIS2_LIB_FOLDER "lib"
	
/*********************************** Constants*********************************/	
	
	
#ifdef __cplusplus
}
#endif

#endif /* AXIS2_DEPLOYMENT_H */
