
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

#ifndef AXIS2_OUT_TRANSPORT_INFO_H
#define AXIS2_OUT_TRANSPORT_INFO_H

/**
 * @defgroup axis2_out_transport_info out transport info
 * @ingroup axis2_core_trans
 * Description
 * @{
 */

/**
 * @file axis2_out_transport_info.h
 * @brief axis2 Out Transport Info
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_out_transport_info */
    typedef struct axis2_out_transport_info axis2_out_transport_info_t;

    typedef struct axis2_out_transport_info_ops axis2_out_transport_info_ops_t;

    struct axis2_out_transport_info_ops
    {
        axis2_status_t(
            AXIS2_CALL
            * set_content_type)(
                axis2_out_transport_info_t * info,
                const axutil_env_t * env,
                const axis2_char_t * content_type);

        axis2_status_t(
            AXIS2_CALL
            * set_char_encoding)(
                axis2_out_transport_info_t * info,
                const axutil_env_t * env,
                const axis2_char_t * encoding);

        void(
            AXIS2_CALL
            * free)(
                axis2_out_transport_info_t * info,
                const axutil_env_t * env);
    };

    struct axis2_out_transport_info
    {
        const axis2_out_transport_info_ops_t *ops;
    };

    /** Set content type. */
#define AXIS2_OUT_TRANSPORT_INFO_SET_CONTENT_TYPE(out_transport_info, \
               env, content_type) ((out_transport_info->ops)->set_content_type(out_transport_info, env, content_type))

    /** Set cahr encoding. */
#define AXIS2_OUT_TRANSPORT_INFO_SET_CHAR_ENCODING(out_transport_info, \
               env, encoding) ((out_transport_info->ops)->set_char_encoding(out_transport_info, env, encoding))

    /** Free. */
#define AXIS2_OUT_TRANSPORT_INFO_FREE(out_transport_info, env)\
                    ((out_transport_info->ops)->free(out_transport_info, env))

    /** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_OUT_TRANSPORT_INFO_H */
