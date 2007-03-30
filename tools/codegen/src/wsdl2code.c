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

#include <w2c_cmdline_option_parser.h>
#include <axutil_utils.h>
#include <axutil_array_list.h>
#include <axutil_hash.h>
#include <w2c_cmdline_option.h>
#include <w2c_cmdline_option_consts.h>
#include <w2c_messages.h>
#include <w2c_engine.h>

static void
w2c_print_message(const axutil_env_t* env, axutil_properties_t* messageh,
        axis2_char_t* key);

static void
w2c_print_usage(const axutil_env_t* env);

static axis2_status_t
w2c_validate_cmdline_options(w2c_cmdline_option_parser_t* cmd_opt_parser,
        const axutil_env_t* env);

int main(int argc, char** argv)
{
    w2c_cmdline_option_parser_t* cmd_opt_parser = NULL;
    axutil_env_t* env = NULL;
    w2c_engine_t* engine = NULL;
    axis2_status_t status;

	env = axutil_env_create_all("codegen.log", AXIS2_LOG_LEVEL_DEBUG);

    cmd_opt_parser = w2c_cmdline_option_parser_create
            (env, argc - 1, argv + 1);/* ommit first arg */
    if (! cmd_opt_parser)
    {
        w2c_print_usage(env);
    }
    else
    {
        status = w2c_validate_cmdline_options(cmd_opt_parser, env);

        if (AXIS2_SUCCESS == status)
        {
            engine = w2c_engine_create_with_parser(env, cmd_opt_parser);
            W2C_ENGINE_GENERATE(engine, env);

            W2C_ENGINE_FREE(engine, env);
        }
    }
    W2C_CMDLINE_OPTION_PARSER_FREE(cmd_opt_parser, env);
    axutil_env_free(env);
    return 0;
}

static axis2_status_t
w2c_validate_cmdline_options(w2c_cmdline_option_parser_t* cmd_opt_parser,
        const axutil_env_t*  env)
{
    axutil_hash_t* h = NULL;
    axutil_array_list_t* invalid_arr = NULL;
    w2c_cmdline_option_t* option = NULL;
    int size = 0;
    axis2_char_t* val = NULL;

    invalid_arr = W2C_CMDLINE_OPTION_PARSER_GET_INVALID_OPTIONS(cmd_opt_parser, env);
    size  = axutil_array_list_size(invalid_arr, env);
    axutil_array_list_free(invalid_arr, env);
    if (size > 0)
    {
        w2c_print_usage(env);
        return AXIS2_FAILURE;
    }
    h =  W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS(cmd_opt_parser, env);
    option = (w2c_cmdline_option_t*)
            axutil_hash_get(h, W2C_CMDLINE_OPTION_CONSTS_WSDL_LOCATION_URI,
                    AXIS2_HASH_KEY_STRING);
    if (option == NULL)
    {
        w2c_print_usage(env);
        return AXIS2_FAILURE;
    }
    else
    {
        val = W2C_CMDLINE_OPTION_GET_VALUE(option, env);
        if (val == NULL || *val == '\0')
        {
            w2c_print_usage(env);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

static void
w2c_print_usage(const axutil_env_t* env)
{
    axutil_properties_t* props = NULL;

    props = w2c_messages_get_message_properties(env);
    w2c_print_message(env, props, "wsdl2code.arg1");
    w2c_print_message(env, props, "wsdl2code.arg2");
    w2c_print_message(env, props, "wsdl2code.arg3");
    w2c_print_message(env, props, "wsdl2code.arg4");
    w2c_print_message(env, props, "wsdl2code.arg5");
    w2c_print_message(env, props, "wsdl2code.arg6");
    w2c_print_message(env, props, "wsdl2code.arg7");
    w2c_print_message(env, props, "wsdl2code.arg8");
    w2c_print_message(env, props, "wsdl2code.arg9");
    w2c_print_message(env, props, "wsdl2code.arg10");

    axutil_properties_free(props, env);
}

static void
w2c_print_message(const axutil_env_t* env, axutil_properties_t* messageh, axis2_char_t* key)
{
    axis2_char_t* message = NULL;
    message = (axis2_char_t*)
            axutil_properties_get_property(messageh, env, key);
    printf("%s\n", message);
}
