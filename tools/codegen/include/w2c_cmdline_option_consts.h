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
  
#ifndef W2C_CMDLINE_OPTION_CONSTS_H
#define W2C_CMDLINE_OPTION_CONSTS_H

/**
 * @file w2c_cmdline_option_consts.h
 * @brief command line option constants
 */

#include <stdio.h>
#include <axutil_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_cmdline_option_consts keep command line optons consts 
 * @ingroup w2c_wsdl_utils
 * @{
 */    

/* short option constants*/
#define W2C_CMDLINE_OPTION_CONSTS_WSDL_LOCATION_URI "uri"
#define W2C_CMDLINE_OPTION_CONSTS_OUTPUT_LOCATION "o"
#define W2C_CMDLINE_OPTION_CONSTS_SERVER_SIDE_CODE "ss"
#define W2C_CMDLINE_OPTION_CONSTS_SERVICE_DESCRIPTION "sd"
#define W2C_CMDLINE_OPTION_CONSTS_ASYNC_ONLY "a"
#define W2C_CMDLINE_OPTION_CONSTS_SYNC_ONLY "s"
#define W2C_CMDLINE_OPTION_CONSTS_PACKAGE "p"
#define W2C_CMDLINE_OPTION_CONSTS_STUB_LANGUAGE "l"
#define W2C_CMDLINE_OPTION_CONSTS_GENERATE_TEST_CASE "t"
#define W2C_CMDLINE_OPTION_CONSTS_DATA_BINDING_TYPE "d"
#define W2C_CMDLINE_OPTION_CONSTS_PACK_CLASSES "pk"
#define W2C_CMDLINE_OPTION_CONSTS_GENERATE_ALL "g"
#define W2C_CMDLINE_OPTION_CONSTS_PORT_NAME "pn"
#define W2C_CMDLINE_OPTION_CONSTS_SERVICE_NAME "sn"
#define W2C_CMDLINE_OPTION_CONSTS_REPOSITORY_PATH "r"
#define W2C_CMDLINE_OPTION_CONSTS_NAME_SPACE_TO_PACKAGE "ns2p"
#define W2C_CMDLINE_OPTION_CONSTS_SERVER_SIDE_INTERFACE "ssi"
#define W2C_CMDLINE_OPTION_CONSTS_EXTERNAL_MAPPING "em"
#define W2C_CMDLINE_OPTION_CONSTS_WSDL_VERSION "wv"
#define W2C_CMDLINE_OPTION_CONSTS_FLATTEN_FILES "f"
#define W2C_CMDLINE_OPTION_CONSTS_CONVERT_NAMES "cn"
#define W2C_CMDLINE_OPTION_CONSTS_INDENT_CODE "id"

/* long option constants */
#define W2C_CMDLINE_OPTION_CONSTS_LONG_OUTPUT_LOCATION "output"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_SERVER_SIDE_CODE "server-side"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_SERVICE_DESCRIPTION "service-description"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_ASYNC_ONLY "async"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_SYNC_ONLY "sync"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_PACKAGE "package"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_STUB_LANGUAGE "language"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_GENERATE_TEST_CASE "test-case"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_DATA_BINDING_TYPE "databinding-method"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_PACK_CLASSES "pack-classes"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_GENERATE_ALL "generate-all"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_PORT_NAME "port-name"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_SERVICE_NAME "service-name"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_REPOSITORY_PATH "repository-path"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_NAME_SPACE_TO_PACKAGE "namespace2package"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_SERVER_SIDE_INTERFACE "serverside-interface"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_EXTERNAL_MAPPING "external-mapping"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_WSDL_VERSION "wsdl-version"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_FLATTEN_FILES "flatten-files"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_CONVERT_NAMES "convert-names"
#define W2C_CMDLINE_OPTION_CONSTS_LONG_INDENT_CODE "indent-code"

#define W2C_CMDLINE_OPTION_CONSTS_INVALID "INVALID_OPTION"
#define W2C_CMDLINE_OPTION_CONSTS_EXTRA_OPTIONTYPE_PREFIX  "E"

#define W2C_CMDLINE_OPTION_CONSTS_WSDL_VERSION_2  "2.0"
#define W2C_CMDLINE_OPTION_CONSTS_WSDL_VERSION_2_OPTIONAL  "2"
#define W2C_CMDLINE_OPTION_CONSTS_WSDL_VERSION_1  "1.1"

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_CMDLINE_OPTION_CONSTS_H */
