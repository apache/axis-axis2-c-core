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
  
#ifndef W2C_CMDLINE_OPTION_VALIDATOR_H
#define W2C_CMDLINE_OPTION_VALIDATOR_H

/**
 * @file w2c_cmdline_option_validator.h
 * @brief handles Axis2/C w2c command line option validation
 */

#include <axis2_utils.h>
#include <w2c_cmdline_option.h>

#ifdef __cplusplus

extern "C"
{
#endif

/** 
 * @defgroup w2c_cmdline_option_validator commnadline option validator IF
 * @ingroup w2c_wsdl_utils
 * @{
 */    


/**
 * validate the command line option.
 * @param env double pointer to environment struct. MUST NOT be NULL
 * @param option option to validate
 * @return validation sucess:AXIS2_FALSE otherwise:AXIS2_TRUE
 */

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
w2c_cmdline_option_validator_isinvalid( 
    const axis2_env_t *env,
    w2c_cmdline_option_t *option);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_CMDLINE_OPTION_VALIDATOR_H */
