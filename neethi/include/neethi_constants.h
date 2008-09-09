
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

#ifndef NEETHI_CONSTANTS_H
#define NEETHI_CONSTANTS_H

#define NEETHI_EXACTLYONE "ExactlyOne"
#define NEETHI_ALL "All"
#define NEETHI_POLICY "Policy"
#define NEETHI_REFERENCE "PolicyReference"
#define NEETHI_URI "URI"
#define NEETHI_NAMESPACE "http://schemas.xmlsoap.org/ws/2004/09/policy"
#define NEETHI_POLICY_15_NAMESPACE "http://www.w3.org/ns/ws-policy"
#define NEETHI_PREFIX "wsp"
#define NEETHI_WSU_NS "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd"
#define NEETHI_ID "Id"
#define NEETHI_WSU_NS_PREFIX "wsu"
#define NEETHI_NAME "Name"
#define AXIS2_OPTIMIZED_MIME_SERIALIZATION "OptimizedMimeSerialization"
#define AXIS2_MTOM_POLICY_NS "http://schemas.xmlsoap.org/ws/2004/09/policy/optimizedmimeserialization"
#define AXIS2_RM_POLICY_10_NS "http://schemas.xmlsoap.org/ws/2005/02/rm/policy"
#define AXIS2_RM_POLICY_11_NS "http://docs.oasis-open.org/ws-rx/wsrmp/200702"
#define AXIS2_SANDESHA2_NS "http://ws.apache.org/sandesha2/c/policy"

/* Reliable messaging related constatnts */

/* RMPolicy 1.0 */

#define AXIS2_RM_RMASSERTION "RMAssertion"
#define AXIS2_RM_INACTIVITY_TIMEOUT "InactivityTimeout"
#define AXIS2_RM_BASE_RETRANSMISSION_INTERVAL "BaseRetransmissionInterval"
#define AXIS2_RM_EXPONENTIAL_BACK_OFF "ExponentialBackoff"
#define AXIS2_RM_ACKNOWLEDGEMENT_INTERVAL "AcknowledgementInterval"

/* RM policy 1.1 */

#define AXIS2_RM_SEQUENCE_STR "SequenceSTR"
#define AXIS2_RM_SEQUENCE_TRANSPORT_SECURITY "SequenceTransportSecurity"
#define AXIS2_RM_DELIVERY_ASSURANCE "DeliveryAssurance"
#define AXIS2_RM_EXACTLY_ONCE "ExactlyOnce"
#define AXIS2_RM_AT_LEAST_ONCE "AtLeastOnce"
#define AXIS2_RM_AT_MOST_ONCE "AtMostOnce"
#define AXIS2_RM_IN_ORDER "InOrder"

/* Sandesha2/C specific */

#define AXIS2_RM_SANDESHA2_DB "sandesha2_db"
#define AXIS2_RM_STORAGE_MANAGER "StorageManager"
#define AXIS2_RM_MESSAGE_TYPES_TO_DROP "MessageTypesToDrop"
#define AXIS2_RM_MAX_RETRANS_COUNT "MaxRetransCount"
#define AXIS2_RM_SENDER_SLEEP_TIME "SenderSleepTime"
#define AXIS2_RM_INVOKER_SLEEP_TIME "InvokerSleepTime"
#define AXIS2_RM_POLLING_WAIT_TIME "PollingWaitTime"
#define AXIS2_RM_TERMINATE_DELAY "TerminateDelay"


/**
* @file neethi_constants.h
* @brief includes all the string constants
*/
#ifdef __cplusplus
extern "C"
{
#endif

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /*NEETHI_INCLUDES_H */
