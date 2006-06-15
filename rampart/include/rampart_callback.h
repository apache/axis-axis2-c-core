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

#ifndef RAMPART_CALLBACK_H
#define RAMPART_CALLBACK_H


#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Struct to get password using callbacks
     * @defgroup rampart_callback rampart callback
     * @ingroup rampart_util
     * @{
     */

    /** 
     * Type name for struct rampart_callback_ops 
     */
    typedef struct rampart_callback_ops rampart_callback_ops_t;

    /**
     * Type name for struct rampart_callback
     */

    typedef struct rampart_callback rampart_callback_t;
    
    /**
     * Only operation get_password is to get the password
     * User should prvide a function pointer to this
     */
    AXIS2_DECLARE_DATA struct rampart_callback_ops
    {            
            axis2_char_t *(AXIS2_CALL*
            get_password)(rampart_callback_t *rcb,
            const axis2_env_t *env, const axis2_char_t *username);
            
            axis2_status_t (AXIS2_CALL*
            free)(rampart_callback_t *rcb,
                  const axis2_env_t* env);

    };

    struct rampart_callback
    {
        rampart_callback_ops_t *ops;
    };

    /** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* RAMPART_CALLBACK_H */

