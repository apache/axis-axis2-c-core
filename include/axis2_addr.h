
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

#ifndef AXIS2_ADDR_H
#define AXIS2_ADDR_H

#ifdef __cplusplus
extern "C"
{
#endif

    /**
      * @defgroup axis2_addr WS-Addressing
      * @ingroup axis2
      * @{
      * @}
      */

    /**
      * @defgroup axis2_addr_consts WS-Addressing related constants
      * @ingroup axis2_addr
      * @{
      */

    /** @file axis2_addr.h */

    /* ====================== Common Message Addressing Properties =========== */
 
    /** WS-Addressing Message ID */
#define AXIS2_WSA_MESSAGE_ID "MessageID"

    /** WS-Addressing Relates To */
#define AXIS2_WSA_RELATES_TO "RelatesTo"

    /** WS-Addressing Relates To Relationship Type */
#define AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE "RelationshipType"

    /** WS-Addressing To */
#define AXIS2_WSA_TO "To"

    /** WS-Addressing From */
#define AXIS2_WSA_FROM "From"

    /** WS-Addressing Reply To */
#define AXIS2_WSA_REPLY_TO "ReplyTo"

    /** WS-Addressing Fault To */
#define AXIS2_WSA_FAULT_TO "FaultTo"

    /** WS-Addressing Action */
#define AXIS2_WSA_ACTION "Action"

    /** WS-Addressing Mapping */
#define AXIS2_WSA_MAPPING "wsamapping"

    /* ====================== Common EPR Elements ============================ */

    /** End Pointer Reference Address */
#define EPR_ADDRESS "Address"

    /** End Pointer Reference Reference Parameters */
#define EPR_REFERENCE_PARAMETERS "ReferenceParameters"

    /** End Pointer Reference Service Name */
#define EPR_SERVICE_NAME "ServiceName"

    /** End Pointer Reference Reference Properties */
#define EPR_REFERENCE_PROPERTIES "ReferenceProperties"

    /** End Pointer Reference Port Type */
#define EPR_PORT_TYPE "PortType"

    /** End Pointer Reference Port Name */
#define EPR_SERVICE_NAME_PORT_NAME "PortName"

    /* ====================== Addressing Submission Version Constants ======== */

    /** WS-Addressing Namespace for Submission Version */
#define AXIS2_WSA_NAMESPACE_SUBMISSION "http://schemas.xmlsoap.org/ws/2004/08/addressing"

    /** WS-Addressing Relates To Relationship Type Default Value for Submission Version */
#define AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE_SUBMISSION "wsa:Reply"

    /** WS-Addressing Anonymous URL for Submission Version */
#define AXIS2_WSA_ANONYMOUS_URL_SUBMISSION "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous"

    /** WS-Addressing None URL for Submission Version */
#define AXIS2_WSA_NONE_URL_SUBMISSION "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/none"

    /* =====================Addressing 1.0 Final Version Constants =========== */

    /** WS-Addressing Namespace for 1.0 Final Version */
#define AXIS2_WSA_NAMESPACE "http://www.w3.org/2005/08/addressing"

    /** WS-Addressing Relates To Relationship Type Default Value for 1.0 Final Version */
#define AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE "http://www.w3.org/2005/08/addressing/reply"

    /** WS-Addressing Anonymous URL for 1.0 Final Version */
#define AXIS2_WSA_ANONYMOUS_URL "http://www.w3.org/2005/08/addressing/anonymous"

    /** WS-Addressing None URL for 1.0 Final Version */
#define AXIS2_WSA_NONE_URL "http://www.w3.org/2005/08/addressing/none"

    /* ======================================================================= */

    /** WS-Addressing Is Reference Parameter Attribute */
#define AXIS2_WSA_IS_REFERENCE_PARAMETER_ATTRIBUTE "IsReferenceParameter"

    /** WS-Addressing Type Attribute Value */
#define AXIS2_WSA_TYPE_ATTRIBUTE_VALUE "true"

    /** WS-Addressing Interface Name */
#define AXIS2_WSA_INTERFACE_NAME "InterfaceName"

    /** WS-Addressing Service/Endpoint Name */
#define AXIS2_WSA_SERVICE_NAME_ENDPOINT_NAME "EndpointName"

    /** WS-Addressing Policies */
#define AXIS2_WSA_POLICIES "Policies"

    /** WS-Addressing Metadata */
#define AXIS2_WSA_METADATA "Metadata"

    /* ======================================================================= */

    /** WS-Addressing Version */
#define AXIS2_WSA_VERSION "WSAddressingVersion"

    /** WS-Addressing Default Prefix */
#define AXIS2_WSA_DEFAULT_PREFIX "wsa"

    /** WS-Addressing Prefixes for faults*/
#define AXIS2_WSA_PREFIX_FAULT_TO AXIS2_WSA_DEFAULT_PREFIX":"AXIS2_WSA_FAULT_TO

    /** WS-Addressing Prefixes for faults*/
#define AXIS2_WSA_PREFIX_REPLY_TO AXIS2_WSA_DEFAULT_PREFIX":"AXIS2_WSA_REPLY_TO

    /** WS-Addressing Prefixes for faults*/
#define AXIS2_WSA_PREFIX_TO AXIS2_WSA_DEFAULT_PREFIX":"AXIS2_WSA_TO 

    /** WS-Addressing Prefixes for faults*/
#define AXIS2_WSA_PREFIX_MESSAGE_ID AXIS2_WSA_DEFAULT_PREFIX":"AXIS2_WSA_MESSAGE_ID

    /** WS-Addressing Prefixes for faults*/
#define AXIS2_WSA_PREFIX_ACTION AXIS2_WSA_DEFAULT_PREFIX":"AXIS2_WSA_ACTION 

    /* ======================================================================= */

    /** WS-Addressing Param Service Group Context ID */
#define PARAM_SERVICE_GROUP_CONTEXT_ID "ServiceGroupContextIdFromAddressing"

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ADDR_H */
