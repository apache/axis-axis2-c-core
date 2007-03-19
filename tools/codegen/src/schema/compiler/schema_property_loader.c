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
 
#include <w2c_schema_property_loader.h>
#include <axis2_properties.h>
#include <axis2_array_list.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <axis2_hash.h>
#include <stdlib.h>
#include <w2c_messages.h>
#include <w2c_class_loader.h>

#define W2C_SCHEMA_PROPERTY_LOADER_WRITER_CLASS "schema.writer.class"
#define W2C_SCHEMA_PROPERTY_LOADER_WRITER_TEMPLATE "schema.writer.template"
#define W2C_SCHEMA_PROPERTY_LOADER_TYPEMAP "schema.typemap"

#define W2C_SCHEMA_PROPERTY_LOADER_CLASS "default.class"
#define W2C_SCHEMA_PROPERTY_LOADER_ARRAY "default.array"

#define W2C_SCHEMA_PROPERTY_LOADER_DEFAULT_PROPERTIES \
            "/conf/schema-config.properties"

/** 
 * @brief
 */
typedef struct w2c_schema_property_loader_impl
{
    w2c_schema_property_loader_t schema_property_loader;
    
    axis2_char_t *template_name;
    w2c_schema_writer_t *writer_instance;
    axis2_char_t *typemapper_name;

    axis2_properties_t *prop_set;
    axis2_char_t *prop_filename;
    axis2_char_t *language;
    axis2_char_t *default_class;
    axis2_char_t *default_array;
       
} w2c_schema_property_loader_impl_t;

#define W2C_INTF_TO_IMPL(schema_property_loader) \
    ((w2c_schema_property_loader_impl_t *) schema_property_loader)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
w2c_schema_property_loader_free(
    w2c_schema_property_loader_t *schema_property_loader, 
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_property_loader_set_filename(
          w2c_schema_property_loader_t *schema_property_loader,
          const axis2_env_t *env,
          axis2_char_t *filename);

axis2_status_t AXIS2_CALL
w2c_schema_property_loader_set_language(
          w2c_schema_property_loader_t *schema_property_loader,
          const axis2_env_t *env,
          axis2_char_t *language);

axis2_status_t AXIS2_CALL
w2c_schema_property_loader_reload(
    w2c_schema_property_loader_t *schema_property_loader, 
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_schema_property_loader_get_template_name(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env);

w2c_schema_writer_t* AXIS2_CALL
w2c_schema_property_loader_get_writer_instance(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL 
w2c_schema_property_loader_get_typemapper_name(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL 
w2c_schema_property_loader_get_default_array(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL 
w2c_schema_property_loader_get_default_class(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env);



/* private methods */

static void
w2c_schema_property_loader_load_values(
                      w2c_schema_property_loader_impl_t *obj_impl,
                      const axis2_env_t *env);

/************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_schema_property_loader_t * AXIS2_CALL 
w2c_schema_property_loader_create (const axis2_env_t *env,
                                 axis2_char_t *language)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
    axis2_char_t* axis2c_home;
    int path_len;
    /*
    FILE *f = NULL;
   */
    AXIS2_ENV_CHECK(env, NULL);

    

    schema_property_loader_impl = (w2c_schema_property_loader_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_schema_property_loader_impl_t));

    if(! schema_property_loader_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    schema_property_loader_impl->schema_property_loader.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_schema_property_loader_ops_t));
    if(! schema_property_loader_impl->schema_property_loader.ops)
    {
        w2c_schema_property_loader_free(&(schema_property_loader_impl->schema_property_loader), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
 
    schema_property_loader_impl->default_array = NULL;
    schema_property_loader_impl->default_class = NULL;
    schema_property_loader_impl->template_name = NULL;
    schema_property_loader_impl->writer_instance = NULL;
    schema_property_loader_impl->typemapper_name = NULL;
    schema_property_loader_impl->prop_filename = NULL;
    schema_property_loader_impl->language = axis2_strdup(language, env);
     
    axis2c_home = getenv ( "AXIS2C_HOME" );
    path_len = axis2_strlen (axis2c_home ) + 
             axis2_strlen (W2C_SCHEMA_PROPERTY_LOADER_DEFAULT_PROPERTIES) +
             2;
    schema_property_loader_impl-> prop_filename = AXIS2_MALLOC ( env->allocator,
                                        sizeof(axis2_char_t)*path_len );
    sprintf(schema_property_loader_impl-> prop_filename, "%s%s", axis2c_home,
                           W2C_SCHEMA_PROPERTY_LOADER_DEFAULT_PROPERTIES);
    
    /*
	f = fopen ( schema_property_loader_impl-> prop_filename, "r+");
    if ( f == NULL )
    {
        w2c_schema_property_loader_free(&(schema_property_loader_impl->schema_property_loader), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_COULD_NOT_OPEN_FILE, AXIS2_FAILURE);
        return NULL;
    }
	*/
    schema_property_loader_impl-> prop_set =  axis2_properties_create( env);
	axis2_properties_load( schema_property_loader_impl-> prop_set, env, schema_property_loader_impl->prop_filename);

    if ( schema_property_loader_impl-> prop_set != NULL )
    {
        w2c_schema_property_loader_load_values( schema_property_loader_impl, env );
    }
    schema_property_loader_impl->schema_property_loader.ops->free = 
                      w2c_schema_property_loader_free;
    schema_property_loader_impl->schema_property_loader.ops->set_filename= 
                      w2c_schema_property_loader_set_filename;
    schema_property_loader_impl->schema_property_loader.ops->set_language= 
                      w2c_schema_property_loader_set_language;
    schema_property_loader_impl->schema_property_loader.ops->reload = 
                      w2c_schema_property_loader_reload;
    schema_property_loader_impl->schema_property_loader.ops->get_template_name=
                      w2c_schema_property_loader_get_template_name ;
    schema_property_loader_impl->schema_property_loader.ops->get_writer_instance=
                      w2c_schema_property_loader_get_writer_instance;
    schema_property_loader_impl->schema_property_loader.ops->get_typemapper_name= 
                      w2c_schema_property_loader_get_typemapper_name;
    schema_property_loader_impl->schema_property_loader.ops->get_default_array= 
                      w2c_schema_property_loader_get_default_array;
    schema_property_loader_impl->schema_property_loader.ops->get_default_class= 
                      w2c_schema_property_loader_get_default_class;

    return &(schema_property_loader_impl->schema_property_loader);
}


/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_schema_property_loader_free (w2c_schema_property_loader_t *schema_property_loader, 
                            const axis2_env_t *env)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    
    if (schema_property_loader_impl-> template_name)
    {
        AXIS2_FREE (env-> allocator,
                 schema_property_loader_impl-> template_name);
    }
    
    if (schema_property_loader_impl-> writer_instance)
    {
        W2C_SCHEMA_WRITER_FREE( 
                schema_property_loader_impl-> writer_instance, env);
    }
    if (schema_property_loader_impl-> typemapper_name)
    {
        AXIS2_FREE (env-> allocator,
                 schema_property_loader_impl-> typemapper_name);
    }

    if (schema_property_loader_impl-> prop_set)
    {
        axis2_properties_free( schema_property_loader_impl-> prop_set, env);
    }

    if (schema_property_loader_impl-> prop_filename)
    {
        AXIS2_FREE (env-> allocator, schema_property_loader_impl-> prop_filename);
    }

    if (schema_property_loader_impl-> language)
    {
        AXIS2_FREE (env-> allocator, schema_property_loader_impl-> language);
    }

    if (schema_property_loader_impl-> default_class)
    {
        AXIS2_FREE (env-> allocator, schema_property_loader_impl-> default_class);
    }

    if (schema_property_loader_impl-> default_array)
    {
        AXIS2_FREE (env-> allocator, schema_property_loader_impl-> default_array);
    }

    if(schema_property_loader->ops)
    {
        AXIS2_FREE(env->allocator, schema_property_loader->ops);
        schema_property_loader->ops = NULL;
    }
    if(schema_property_loader_impl)
    {
        AXIS2_FREE(env->allocator, schema_property_loader_impl);
        schema_property_loader_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
w2c_schema_property_loader_set_filename (
          w2c_schema_property_loader_t *schema_property_loader,
          const axis2_env_t *env,
          axis2_char_t *filename)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
  
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    schema_property_loader_impl -> prop_filename = filename;   
    w2c_schema_property_loader_reload ( schema_property_loader, env );
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_schema_property_loader_set_language(
          w2c_schema_property_loader_t *schema_property_loader,
          const axis2_env_t *env,
          axis2_char_t *language)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
  
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    schema_property_loader_impl -> language = language;   
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
w2c_schema_property_loader_reload (
    w2c_schema_property_loader_t *schema_property_loader, 
    const axis2_env_t *env)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
    /*
    FILE *f = NULL;
    */
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    
    if (schema_property_loader_impl-> prop_set)
    {
         axis2_properties_free ( schema_property_loader_impl->prop_set, env );
    }
	/*
    f = fopen ( schema_property_loader_impl-> prop_filename, "r+");
    if ( f == NULL )
    {
        w2c_schema_property_loader_free(&(schema_property_loader_impl->schema_property_loader), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_COULD_NOT_OPEN_FILE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
	*/
    schema_property_loader_impl-> prop_set =  axis2_properties_create( env);
	axis2_properties_load( schema_property_loader_impl-> prop_set, env, schema_property_loader_impl->prop_filename);

    if ( schema_property_loader_impl-> prop_set != NULL )
    {
        w2c_schema_property_loader_load_values( schema_property_loader_impl, env );
    }
    else
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
w2c_schema_property_loader_get_template_name(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    
    return schema_property_loader_impl-> template_name;
}

axis2_char_t* AXIS2_CALL
w2c_schema_property_loader_get_default_class(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    
    return schema_property_loader_impl-> default_class;
}

axis2_char_t* AXIS2_CALL
w2c_schema_property_loader_get_default_array(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    
    return schema_property_loader_impl-> default_array;
}

w2c_schema_writer_t* AXIS2_CALL
w2c_schema_property_loader_get_writer_instance(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    
    return schema_property_loader_impl-> writer_instance;
}


axis2_char_t* AXIS2_CALL 
w2c_schema_property_loader_get_typemapper_name(
    w2c_schema_property_loader_t *schema_property_loader,
    const axis2_env_t *env)
{
    w2c_schema_property_loader_impl_t *schema_property_loader_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    schema_property_loader_impl = W2C_INTF_TO_IMPL(schema_property_loader);
    
    return schema_property_loader_impl-> typemapper_name;
}



static void
w2c_schema_property_loader_load_values(
                      w2c_schema_property_loader_impl_t *obj_impl,
                      const axis2_env_t *env)
{
     axis2_hash_t *prop_hash = NULL;
     axis2_hash_index_t *hi = NULL;
     axis2_char_t *key= NULL;
     axis2_char_t *val= NULL;
     int index = 0;
     axis2_char_t *language = NULL;
     w2c_schema_writer_t *writer = NULL;
     axis2_dll_desc_t *dll_desc = NULL;

     prop_hash = axis2_properties_get_all( obj_impl-> prop_set, env);
     language = obj_impl-> language;

     /**
      * run through the language specific properties and populate the
      * language specific property map
      */
     if ( language)
     {
         for (hi = axis2_hash_first(prop_hash, env); hi;
              hi = axis2_hash_next(env, hi))
         {
            axis2_hash_this(hi, (void*)&key, NULL, (void*)&val);
            if ( w2c_string_indexof_cs ( key, language) == 0 && 
                         (index = w2c_string_indexof ( key, '.')) == axis2_strlen(language))
            {
                if (w2c_string_indexof_cs( key,
                     W2C_SCHEMA_PROPERTY_LOADER_WRITER_CLASS) == index + 1)
                {
                    writer = w2c_class_loader_get_object_from_class_name
                          ( env, val, W2C_CLASS_LOADER_WRITER_PATH, &dll_desc );
                    if ( ! writer )
                    {
                        w2c_messages_print_n_log_error( env,
                                       "schema.writerclassMissing");
                        return;
                    }
                    obj_impl-> writer_instance = writer;
                }
                if (w2c_string_indexof_cs( key,
                     W2C_SCHEMA_PROPERTY_LOADER_WRITER_TEMPLATE) == index + 1)
                {
                    obj_impl-> template_name = axis2_strdup( val, env);
                }
                if (w2c_string_indexof_cs( key, 
                     W2C_SCHEMA_PROPERTY_LOADER_TYPEMAP) == index + 1)
                {
                    obj_impl-> typemapper_name = axis2_strdup( val, env);
                }
                if (w2c_string_indexof_cs( key, 
                     W2C_SCHEMA_PROPERTY_LOADER_CLASS) == index + 1)
                {
                    obj_impl-> default_class = axis2_strdup( val, env);
                }
                if (w2c_string_indexof_cs( key, 
                     W2C_SCHEMA_PROPERTY_LOADER_ARRAY) == index + 1)
                {
                    obj_impl-> default_array = axis2_strdup( val, env);
                }
            }
         }
     }   
}

