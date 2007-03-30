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

#include <w2c_schema_compiler_options.h>
#include <axutil_string.h>

/**
 * @brief
 */
typedef struct w2c_schema_compiler_options_impl
{
    w2c_schema_compiler_options_t compiler_options;

    axis2_char_t *output_location;
    axis2_char_t *package_name;
    axis2_char_t *mapper_class_package;
    axis2_bool_t helper_mode;
    axis2_bool_t write_out;
    axis2_bool_t wrap_classes;
    axutil_hash_t *ns2package_map;
    axis2_char_t *language;

    W2C_ENGINE_CONFIGURATION_NAMEMAKER maker_func;
    w2c_qname2name_maker_t *qname2name_maker;

} w2c_schema_compiler_options_impl_t;

#define W2C_INTF_TO_IMPL(schema_compiler_options) \
        ((w2c_schema_compiler_options_impl_t *) schema_compiler_options)

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_free (w2c_schema_compiler_options_t *schema_compiler_options,
                                    const axutil_env_t *env);

axis2_char_t*AXIS2_CALL
w2c_schema_compiler_options_get_output_location( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_output_location( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_char_t *output_location);

axis2_char_t*AXIS2_CALL
w2c_schema_compiler_options_get_mapper_class_package( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_mapper_class_package( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_char_t *mapper_class_package);


axis2_char_t* AXIS2_CALL
w2c_schema_compiler_options_get_package_name( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_package_name( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_char_t *package_name);

axis2_bool_t AXIS2_CALL
w2c_schema_compiler_options_get_helper_mode( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_helper_mode( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_bool_t helper_mode);

axis2_bool_t AXIS2_CALL
w2c_schema_compiler_options_get_write_out( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_write_out( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_bool_t write_out);

axis2_bool_t AXIS2_CALL
w2c_schema_compiler_options_get_wrap_classes( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_wrap_classes( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_bool_t wrap_classes);

axutil_hash_t* AXIS2_CALL
w2c_schema_compiler_options_get_ns2package_map( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_ns2package_map( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axutil_hash_t *ns2package_map);

axis2_char_t* AXIS2_CALL
w2c_schema_compiler_options_get_language( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_language( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_char_t *language);

w2c_qname2name_maker_t* AXIS2_CALL
w2c_schema_compiler_options_get_qname2name_maker( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_qname2name_maker( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        w2c_qname2name_maker_t *maker);

W2C_ENGINE_CONFIGURATION_NAMEMAKER AXIS2_CALL
w2c_schema_compiler_options_get_name_maker_func( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_name_maker_func( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        W2C_ENGINE_CONFIGURATION_NAMEMAKER maker);

/** function for the default qname 2 name */
axis2_char_t* 
w2c_schema_compiler_options_default_name_maker( axis2_char_t *name,
                                     const axutil_env_t *env);

AXIS2_EXTERN w2c_schema_compiler_options_t * AXIS2_CALL
w2c_schema_compiler_options_create (const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    compiler_options_impl = (w2c_schema_compiler_options_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_schema_compiler_options_impl_t));

    if(! compiler_options_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    compiler_options_impl->output_location = NULL;
    compiler_options_impl->package_name = NULL;
    compiler_options_impl->helper_mode = AXIS2_FALSE;
    compiler_options_impl->write_out = AXIS2_FALSE;
    compiler_options_impl->wrap_classes = AXIS2_FALSE;
    compiler_options_impl->ns2package_map = NULL;
    compiler_options_impl->language = NULL;
    compiler_options_impl->qname2name_maker = NULL;
    compiler_options_impl->maker_func = 
        w2c_schema_compiler_options_default_name_maker;


    compiler_options_impl->compiler_options.ops =
    AXIS2_MALLOC (env->allocator, sizeof(w2c_schema_compiler_options_ops_t));
    if(! compiler_options_impl->compiler_options.ops)
    {
        w2c_schema_compiler_options_free(&(compiler_options_impl->compiler_options), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    compiler_options_impl->compiler_options.ops->free = w2c_schema_compiler_options_free;
    
    compiler_options_impl->compiler_options.ops->get_output_location =
                            w2c_schema_compiler_options_get_output_location;
    compiler_options_impl->compiler_options.ops->set_output_location =
                            w2c_schema_compiler_options_set_output_location;
    compiler_options_impl->compiler_options.ops->get_package_name =
                            w2c_schema_compiler_options_get_package_name;
    compiler_options_impl->compiler_options.ops->set_package_name =
                            w2c_schema_compiler_options_set_package_name;
    compiler_options_impl->compiler_options.ops->get_helper_mode =
                            w2c_schema_compiler_options_get_helper_mode;
    compiler_options_impl->compiler_options.ops->set_helper_mode =
                            w2c_schema_compiler_options_set_helper_mode;
    compiler_options_impl->compiler_options.ops->get_write_out =
                            w2c_schema_compiler_options_get_write_out;
    compiler_options_impl->compiler_options.ops->set_write_out =
                            w2c_schema_compiler_options_set_write_out;
    compiler_options_impl->compiler_options.ops->get_wrap_classes =
                            w2c_schema_compiler_options_get_wrap_classes;
    compiler_options_impl->compiler_options.ops->set_wrap_classes =
                            w2c_schema_compiler_options_set_wrap_classes;
    compiler_options_impl->compiler_options.ops->get_ns2package_map =
                            w2c_schema_compiler_options_get_ns2package_map;
    compiler_options_impl->compiler_options.ops->set_ns2package_map =
                            w2c_schema_compiler_options_set_ns2package_map;
    compiler_options_impl->compiler_options.ops->get_mapper_class_package=
                            w2c_schema_compiler_options_get_mapper_class_package;
    compiler_options_impl->compiler_options.ops->set_mapper_class_package=
                            w2c_schema_compiler_options_set_mapper_class_package;
    compiler_options_impl->compiler_options.ops->get_language=
                            w2c_schema_compiler_options_get_language;
    compiler_options_impl->compiler_options.ops->set_language=
                            w2c_schema_compiler_options_set_language;
    compiler_options_impl->compiler_options.ops->get_name_maker_func=
                            w2c_schema_compiler_options_get_name_maker_func;
    compiler_options_impl->compiler_options.ops->set_name_maker_func=
                            w2c_schema_compiler_options_set_name_maker_func;
    compiler_options_impl->compiler_options.ops->get_qname2name_maker=
                            w2c_schema_compiler_options_get_qname2name_maker;
    compiler_options_impl->compiler_options.ops->set_qname2name_maker=
                            w2c_schema_compiler_options_set_qname2name_maker;


    return &(compiler_options_impl->compiler_options);
}

axis2_status_t AXIS2_CALL 
w2c_schema_compiler_options_free (w2c_schema_compiler_options_t *schema_compiler_options, 
                            const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *schema_compiler_options_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    schema_compiler_options_impl = W2C_INTF_TO_IMPL(schema_compiler_options);
    
    if (schema_compiler_options_impl-> output_location)
    {
        AXIS2_FREE(env-> allocator, schema_compiler_options_impl-> output_location);
    }
    if (schema_compiler_options_impl-> mapper_class_package)
    {
        AXIS2_FREE(env-> allocator, schema_compiler_options_impl-> mapper_class_package);
    }
    if (schema_compiler_options_impl-> package_name)
    {
        AXIS2_FREE(env-> allocator, schema_compiler_options_impl-> package_name);
    }
    if (schema_compiler_options_impl-> ns2package_map)
    {
        axutil_hash_free(schema_compiler_options_impl-> ns2package_map, env);
    }
    if (schema_compiler_options_impl-> language)
    {
        AXIS2_FREE(env-> allocator, schema_compiler_options_impl-> language);
    }
    if(schema_compiler_options->ops)
    {
        AXIS2_FREE(env->allocator, schema_compiler_options->ops);
        schema_compiler_options->ops = NULL;
    }
    if(schema_compiler_options_impl)
    {
        AXIS2_FREE(env->allocator, schema_compiler_options_impl);
        schema_compiler_options_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}




axis2_char_t*AXIS2_CALL
w2c_schema_compiler_options_get_output_location( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

	AXIS2_ENV_CHECK(env, NULL);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> output_location;

}

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_output_location( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_char_t *output_location)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> output_location = axis2_strdup(env, output_location);

    return AXIS2_SUCCESS;
}

axis2_char_t*AXIS2_CALL
w2c_schema_compiler_options_get_mapper_class_package( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

	AXIS2_ENV_CHECK(env, NULL);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> mapper_class_package;

}

axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_mapper_class_package( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_char_t *mapper_class_package)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> mapper_class_package = axis2_strdup(env, mapper_class_package);

    return AXIS2_SUCCESS;
}



axis2_char_t* AXIS2_CALL
w2c_schema_compiler_options_get_package_name( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

	AXIS2_ENV_CHECK(env, NULL);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> package_name;
}


axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_package_name( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_char_t *package_name)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> package_name = axis2_strdup(env, package_name);

    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
w2c_schema_compiler_options_get_helper_mode( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> helper_mode;
}


axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_helper_mode( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_bool_t helper_mode)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> helper_mode = helper_mode;

    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
w2c_schema_compiler_options_get_write_out( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> write_out;
}


axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_write_out( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_bool_t write_out)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> write_out = write_out;

    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
w2c_schema_compiler_options_get_wrap_classes( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> wrap_classes;
}


axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_wrap_classes( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_bool_t wrap_classes)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> wrap_classes = wrap_classes;

    return AXIS2_SUCCESS;
}

axutil_hash_t* AXIS2_CALL
w2c_schema_compiler_options_get_ns2package_map( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> ns2package_map;
}


axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_ns2package_map( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axutil_hash_t *ns2package_map)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> ns2package_map = ns2package_map;

    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
w2c_schema_compiler_options_get_language( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> language;
}


axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_language( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        axis2_char_t *language)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> language = axis2_strdup(env, language);

    return AXIS2_SUCCESS;
}

w2c_qname2name_maker_t* AXIS2_CALL
w2c_schema_compiler_options_get_qname2name_maker( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    return compiler_options_impl-> qname2name_maker;
}


axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_qname2name_maker( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        w2c_qname2name_maker_t *maker)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);

    compiler_options_impl-> qname2name_maker = maker;

    return AXIS2_SUCCESS;
}

W2C_ENGINE_CONFIGURATION_NAMEMAKER AXIS2_CALL
w2c_schema_compiler_options_get_name_maker_func( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);
    
    return compiler_options_impl-> maker_func;
}


axis2_status_t AXIS2_CALL
w2c_schema_compiler_options_set_name_maker_func( w2c_schema_compiler_options_t *compiler_options,
        const axutil_env_t *env,
        W2C_ENGINE_CONFIGURATION_NAMEMAKER maker)
{
    w2c_schema_compiler_options_impl_t *compiler_options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    compiler_options_impl = W2C_INTF_TO_IMPL(compiler_options);
    
    if ( maker != NULL)
    {
        compiler_options_impl-> maker_func = maker;
    }

    return AXIS2_SUCCESS;
}

axis2_char_t* 
w2c_schema_compiler_options_default_name_maker( axis2_char_t *name,
                                     const axutil_env_t *env)
{
        return axis2_strdup(env, name);
}

