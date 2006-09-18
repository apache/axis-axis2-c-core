#include <w2c_cmdline_option_parser.h>
#include <axis2_utils.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <w2c_cmdline_option.h>
#include <w2c_cmdline_option_consts.h>
#include <w2c_messages.h>
#include <w2c_engine.h>

static void
w2c_print_message(const axis2_env_t* env, axis2_properties_t* messageh,
        axis2_char_t* key);

static void
w2c_print_usage(const axis2_env_t* env);

static axis2_status_t
w2c_validate_cmdline_options(w2c_cmdline_option_parser_t* cmd_opt_parser,
        const axis2_env_t* env);

int main(int argc, char** argv)
{
    w2c_cmdline_option_parser_t* cmd_opt_parser = NULL;
    axis2_env_t* env = NULL;
    axis2_allocator_t* allocator = NULL;
    w2c_engine_t* engine = NULL;
    axis2_status_t status;

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    cmd_opt_parser = w2c_cmdline_option_parser_create
            (env, argc - 1, argv + 1);/* ommit first arg */
    if (NULL == cmd_opt_parser)
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
    axis2_allocator_free(allocator);
    axis2_env_free(env);
    return 0;
}

static axis2_status_t
w2c_validate_cmdline_options(w2c_cmdline_option_parser_t* cmd_opt_parser,
        const axis2_env_t*  env)
{
    axis2_hash_t* h = NULL;
    axis2_array_list_t* invalid_arr = NULL;
    w2c_cmdline_option_t* option = NULL;
    int size = 0;
    axis2_char_t* val = NULL;

    invalid_arr = W2C_CMDLINE_OPTION_PARSER_GET_INVALID_OPTIONS(cmd_opt_parser, env);
    size  = AXIS2_ARRAY_LIST_SIZE(invalid_arr, env);
    AXIS2_ARRAY_LIST_FREE(invalid_arr, env);
    if (size > 0)
    {
        w2c_print_usage(env);
        return AXIS2_FAILURE;
    }
    h =  W2C_CMDLINE_OPTION_PARSER_GET_OPTIONS(cmd_opt_parser, env);
    option = (w2c_cmdline_option_t*)
            axis2_hash_get(h, W2C_CMDLINE_OPTION_CONSTS_WSDL_LOCATION_URI,
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
w2c_print_usage(const axis2_env_t* env)
{
    axis2_properties_t* props = NULL;

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

    AXIS2_PROPERTIES_FREE(props, env);
}

static void
w2c_print_message(const axis2_env_t* env, axis2_properties_t* messageh, axis2_char_t* key)
{
    axis2_char_t* message = NULL;
    message = (axis2_char_t*)
            AXIS2_PROPERTIES_GET_PROPERTY(messageh, env, key);
    printf("%s\n", message);
}
