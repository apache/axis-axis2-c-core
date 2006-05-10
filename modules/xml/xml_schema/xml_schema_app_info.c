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

#include <xml_schema/axis2_xml_schema_app_info.h>

typedef struct axis2_xml_schema_app_info_impl axis2_xml_schema_app_info_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_app_info_impl
{
    axis2_xml_schema_app_info_t app_info;
    
    axis2_xml_schema_obj_t *schema_obj;
    axis2_xml_schema_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    /**
     * Provides the source of the application information.
     */
    axis2_char_t *source;
    
    void *markup; /* TODO Replace (void *) with node list */
};

#define AXIS2_INTF_TO_IMPL(app_info) \
        ((axis2_xml_schema_app_info_impl_t *) app_info)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_app_info_free(
        void *app_info,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_app_info_super_objs(
        void *app_info,
        axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_app_info_type(
        void *app_info,
        axis2_env_t **env);

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_app_info_get_base_impl(void *app_info,
                                axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_xml_schema_app_info_get_source(void *app_info,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_app_info_set_source(void *app_info,
                                        axis2_env_t **env,
                                        axis2_char_t *source);

/* TODO replace (void *) mark up with node list */
void *AXIS2_CALL
axis2_xml_schema_app_info_get_markup(void *app_info,
                                        axis2_env_t **env);

/* TODO replace (void *) mark up with node list */
axis2_status_t AXIS2_CALL
axis2_xml_schema_app_info_set_markup(void *app_info,
                                        axis2_env_t **env,
                                        void *markup);
/****************** end macros ***********************************************/

AXIS2_DECLARE(axis2_xml_schema_app_info_t *)
axis2_xml_schema_app_info_create(axis2_env_t **env)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    app_info_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_app_info_impl_t));
    if(!app_info_impl)
    {
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    app_info_impl->schema_obj = NULL;
    app_info_impl->app_info.base.ops = NULL;
    app_info_impl->app_info.ops = NULL;
    app_info_impl->obj_type = AXIS2_XML_SCHEMA_APP_INFO;
    app_info_impl->super = NULL;
    app_info_impl->methods = NULL;
    app_info_impl->source = NULL;
    app_info_impl->markup = NULL;
    
    app_info_impl->app_info.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_app_info_ops_t));
    
    if(!app_info_impl->app_info.ops)
    {
        axis2_xml_schema_app_info_free(&(app_info_impl->app_info), env);
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    app_info_impl->app_info.ops->free = 
        axis2_xml_schema_app_info_free;
    app_info_impl->app_info.ops->super_objs = 
        axis2_xml_schema_app_info_super_objs;
    app_info_impl->app_info.ops->type = 
        axis2_xml_schema_app_info_type;
    app_info_impl->app_info.ops->get_base_impl = 
        axis2_xml_schema_app_info_get_base_impl;
    app_info_impl->app_info.ops->get_source = 
        axis2_xml_schema_app_info_get_source;
    app_info_impl->app_info.ops->set_source = 
        axis2_xml_schema_app_info_set_source;
    app_info_impl->app_info.ops->get_markup = 
        axis2_xml_schema_app_info_get_markup;
    app_info_impl->app_info.ops->set_markup = 
        axis2_xml_schema_app_info_set_markup;
   
    app_info_impl->methods = axis2_hash_make(env);
    if(!app_info_impl->methods) 
    {
        axis2_xml_schema_app_info_free(&(app_info_impl->app_info), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(app_info_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_app_info_free);
    axis2_hash_set(app_info_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_app_info_super_objs);
    axis2_hash_set(app_info_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_app_info_type);
    axis2_hash_set(app_info_impl->methods, "get_source", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_app_info_get_source);
    axis2_hash_set(app_info_impl->methods, "set_source", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_app_info_set_source);
    axis2_hash_set(app_info_impl->methods, "get_markup", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_app_info_get_markup);
    axis2_hash_set(app_info_impl->methods, "set_markup", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_app_info_set_markup);

    app_info_impl->schema_obj = axis2_xml_schema_obj_create(env);
    if(!app_info_impl->schema_obj) 
    {
        axis2_xml_schema_app_info_free(&(app_info_impl->app_info), env);
        return NULL;
    }

    app_info_impl->super = axis2_hash_make(env);
    if(!app_info_impl->super) 
    {
        axis2_xml_schema_app_info_free(&(app_info_impl->app_info), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(app_info_impl->super, "AXIS2_XML_SCHEMA_APP_INFO", 
            AXIS2_HASH_KEY_STRING, &(app_info_impl->app_info));
    axis2_hash_set(app_info_impl->super, "AXIS2_XML_SCHEMA_OBJ", 
            AXIS2_HASH_KEY_STRING, app_info_impl->schema_obj);

    status = 
        axis2_xml_schema_obj_resolve_methods(&(app_info_impl->app_info.base), 
            env, app_info_impl->schema_obj, app_info_impl->methods);
    return &(app_info_impl->app_info);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_app_info_free(void *app_info,
                                    axis2_env_t **env)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    app_info_impl = AXIS2_INTF_TO_IMPL(app_info);

    if(NULL != app_info_impl->source)
    {
        AXIS2_FREE((*env)->allocator, app_info_impl->source);
        app_info_impl->source = NULL;
    }

    /* TODO Free markup */
    
    if(NULL != app_info_impl->super)
    {
        axis2_hash_free(app_info_impl->super, env);
        app_info_impl->super = NULL;
    }
    
    if(NULL != app_info_impl->methods)
    {
        axis2_hash_free(app_info_impl->methods, env);
        app_info_impl->methods = NULL;
    }

    if(NULL != app_info_impl->schema_obj)
    {
        AXIS2_XML_SCHEMA_OBJ_FREE(app_info_impl->schema_obj, env);
        app_info_impl->schema_obj = NULL;
    }
    
    if(NULL != app_info_impl->app_info.ops)
    {
        AXIS2_FREE((*env)->allocator, app_info_impl->app_info.ops);
        app_info_impl->app_info.ops = NULL;
    }
    if(NULL != app_info_impl->app_info.base.ops)
    {
        AXIS2_FREE((*env)->allocator, app_info_impl->app_info.base.ops);
        app_info_impl->app_info.base.ops = NULL;    
    }
    if(NULL != app_info_impl)
    {
        AXIS2_FREE((*env)->allocator, app_info_impl);
        app_info_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_app_info_super_objs(
        void *app_info,
        axis2_env_t **env)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    app_info_impl = AXIS2_INTF_TO_IMPL(app_info);

    return app_info_impl->super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_app_info_type(
        void *app_info,
        axis2_env_t **env)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    app_info_impl = AXIS2_INTF_TO_IMPL(app_info);

    return app_info_impl->obj_type;
}

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_app_info_get_base_impl(
        void *app_info,
        axis2_env_t **env)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    app_info_impl = AXIS2_INTF_TO_IMPL(app_info);

    return app_info_impl->schema_obj;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_app_info_resolve_methods(
         axis2_xml_schema_app_info_t *app_info,
         axis2_env_t **env,
         axis2_xml_schema_app_info_t *app_info_impl,
         axis2_hash_t *methods)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, app_info_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    app_info_impl_l = (axis2_xml_schema_app_info_impl_t *) app_info_impl;
    
    app_info->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_app_info_ops_t));
            
     if(!app_info_impl)
    {
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }        
    app_info->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    app_info->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    app_info->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    app_info->ops->get_source = axis2_hash_get(methods, 
            "get_source", AXIS2_HASH_KEY_STRING);
    
    if(!app_info->ops->get_source)
            app_info->ops->get_source = 
            app_info_impl_l->app_info.ops->get_source;
    
    app_info->ops->set_source = axis2_hash_get(methods, 
            "set_source", AXIS2_HASH_KEY_STRING);
    if(!app_info->ops->set_source)
            app_info->ops->set_source = 
            app_info_impl_l->app_info.ops->set_source;
    
    app_info->ops->get_markup = axis2_hash_get(methods, 
            "get_markup", AXIS2_HASH_KEY_STRING);
    if(!app_info->ops->get_markup)
            app_info->ops->get_markup = 
            app_info_impl_l->app_info.ops->get_markup;
    
    app_info->ops->set_markup = axis2_hash_get(methods, 
            "set_markup", AXIS2_HASH_KEY_STRING);
    if(!app_info->ops->set_markup)
            app_info->ops->set_markup = 
            app_info_impl_l->app_info.ops->set_markup;
    
    return axis2_xml_schema_obj_resolve_methods(&(app_info->base), 
            env, app_info_impl_l->schema_obj, methods);
}

axis2_char_t *AXIS2_CALL
axis2_xml_schema_app_info_get_source(void *app_info,
                                        axis2_env_t **env)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(app_info, env);
    app_info_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_USE",
                AXIS2_HASH_KEY_STRING));
    
    return app_info_impl->source;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_app_info_set_source(void *app_info,
                                        axis2_env_t **env,
                                        axis2_char_t *source)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, source, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(app_info, env);
    app_info_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_USE",
                AXIS2_HASH_KEY_STRING));
    
    
    if(NULL != app_info_impl->source)
    {
        AXIS2_FREE((*env)->allocator, app_info_impl->source);
        app_info_impl->source = NULL;
    }
    app_info_impl->source = AXIS2_STRDUP(source, env);
    if(!app_info_impl->source)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/* TODO replace (void *) mark up with node list */
void *AXIS2_CALL
axis2_xml_schema_app_info_get_markup(void *app_info,
                                        axis2_env_t **env)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(app_info, env);
    app_info_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_USE",
                AXIS2_HASH_KEY_STRING));
    
    return app_info_impl->markup;
}

/* TODO replace (void *) mark up with node list */
axis2_status_t AXIS2_CALL
axis2_xml_schema_app_info_set_markup(void *app_info,
                                        axis2_env_t **env,
                                        void *markup)
{
    axis2_xml_schema_app_info_impl_t *app_info_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, markup, AXIS2_FAILURE);
    super = AXIS2_XML_SCHEMA_USE_SUPER_OBJS(app_info, env);
    app_info_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_USE",
                AXIS2_HASH_KEY_STRING));
    
    
    app_info_impl->markup = markup;
    return AXIS2_SUCCESS;
}
