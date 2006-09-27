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
 
#include <w2c_cmdline_option_validator.h>
#include <w2c_cmdline_option_consts.h>
#include <w2c_cmdline_option.h>


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
w2c_cmdline_option_validator_isinvalid(
    const axis2_env_t *env,
    w2c_cmdline_option_t *option)
{
    axis2_bool_t isinvalid = AXIS2_FALSE;
    axis2_char_t *type = NULL;
    int index = -1;
    
    type = W2C_CMDLINE_OPTION_GET_TYPE( option, env );
    index = w2c_string_indexof_cs(type,
            W2C_CMDLINE_OPTION_CONSTS_EXTRA_OPTIONTYPE_PREFIX);
    if (index != 0 )
    {
        isinvalid =  
            strcmp( W2C_CMDLINE_OPTION_CONSTS_WSDL_LOCATION_URI, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_OUTPUT_LOCATION, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_SERVER_SIDE_CODE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_SERVICE_DESCRIPTION, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_ASYNC_ONLY, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_SYNC_ONLY, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_PACKAGE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_STUB_LANGUAGE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_GENERATE_TEST_CASE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_DATA_BINDING_TYPE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_PACK_CLASSES, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_GENERATE_ALL, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_PORT_NAME, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_SERVICE_NAME, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_REPOSITORY_PATH, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_NAME_SPACE_TO_PACKAGE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_SERVER_SIDE_INTERFACE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_EXTERNAL_MAPPING, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_WSDL_VERSION, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_FLATTEN_FILES, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_CONVERT_NAMES, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_INDENT_CODE , type ) &&

            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_OUTPUT_LOCATION, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_SERVER_SIDE_CODE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_SERVICE_DESCRIPTION, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_ASYNC_ONLY, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_SYNC_ONLY, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_PACKAGE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_STUB_LANGUAGE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_GENERATE_TEST_CASE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_DATA_BINDING_TYPE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_PACK_CLASSES, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_GENERATE_ALL, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_PORT_NAME, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_SERVICE_NAME, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_REPOSITORY_PATH, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_NAME_SPACE_TO_PACKAGE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_SERVER_SIDE_INTERFACE, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_EXTERNAL_MAPPING, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_WSDL_VERSION, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_FLATTEN_FILES, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_CONVERT_NAMES, type ) &&
            strcmp( W2C_CMDLINE_OPTION_CONSTS_LONG_INDENT_CODE, type );
    }
    return isinvalid;
}
