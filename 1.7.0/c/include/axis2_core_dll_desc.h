
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

#ifndef AXIS2_CORE_DLL_DESC_H
#define AXIS2_CORE_DLL_DESC_H

/**
 * @file axis2_core_dll_desc.h
 * @brief Axis2 Core dll_desc interface
 */

#include <axutil_dll_desc.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axis2_core_dll_desc Core DLL description
     * @ingroup axis2_core_utils
     * @{
     */

    /* 
     * For DLL Types, starting index is 10000, and ending
     * index is 10999, this is for unique indexing purposes.
     * Indexes 10000-10099 are reserved for Axis2 Core.
     */

    /* service dll */
    #define AXIS2_SVC_DLL 10000 

    /* handler dll */
    #define AXIS2_HANDLER_DLL 10001

    /* message receiver dll */
    #define AXIS2_MSG_RECV_DLL 10002

    /* module dll */
    #define AXIS2_MODULE_DLL 10003

    /* transport receiver dll */
    #define AXIS2_TRANSPORT_RECV_DLL 10004

    /* transport sender dll */
    #define AXIS2_TRANSPORT_SENDER_DLL 10005

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CORE_DLL_DESC_H */
