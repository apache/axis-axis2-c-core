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

#ifndef AXIS2_PHASE_META_H
#define AXIS2_PHASE_META_H

/**
 * @file axis2_conf.h
 * @brief Axis2 configuration interface
 */

#include <axis2_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_phase_meta phase meta data
 * @ingroup axis2_engine
 * @{
 */
     #define  AXIS2_INFLOW  1
     #define  AXIS2_OUTFLOW 2
     #define  AXIS2_FAULT_INFLOW 3
     #define  AXIS2_FAULT_OUTFLOW 4

    /* INFLOW */
     #define AXIS2_PHASE_TRANSPORTIN "TransportIn"
     #define AXIS2_PHASE_PRE_DISPATCH "PreDispatch"
     #define AXIS2_PHASE_DISPATCH "Dispatch"
     #define AXIS2_PHASE_POST_DISPATCH "PostDispatch"
     #define AXIS2_PHASE_POLICY_DETERMINATION "PolicyDetermination"
     #define AXIS2_PHASE_MESSAGE_PROCESSING "MessageProcessing"

    /* OUTFLOW */
     #define AXIS2_PHASE_MESSAGE_OUT "MessageOut"
     #define AXIS2_PHASE_TRANSPORT_OUT "MessageOut"
     
    /**
     * All the handlers inside transport_sender and tranport_recievre in axis2.xml gose
     * to this phase
     */
     #define AXIS2_TRANSPORT_PHASE "TRANSPORT"


/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_PHASE_META_H */
