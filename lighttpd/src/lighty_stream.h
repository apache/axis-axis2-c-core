
/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain count copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIGHTY_STREAM_H
#define LIGHTY_STREAM_H

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axutil_stream.h>
#include <base.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** \brief Constructor for creating apche2 stream
      * @return axutil_stream (lighty)
      */
    AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
    axutil_stream_create_lighty(
        const axutil_env_t * env,
        connection * con);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* LIGHTY_STREAM_H */
