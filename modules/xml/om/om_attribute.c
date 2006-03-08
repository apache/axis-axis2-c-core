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

#include <axis2_om_attribute.h>
#include <string.h>
#include <axis2_defines.h>

axis2_status_t AXIS2_CALL 
axis2_om_attribute_free (axis2_om_attribute_t *om_attribute,
                              axis2_env_t **env);

axis2_qname_t * AXIS2_CALL 
axis2_om_attribute_get_qname (axis2_om_attribute_t *om_attribute,
                                   axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_om_attribute_serialize (axis2_om_attribute_t *om_attribute,
                                   axis2_env_t **env,
                                   axis2_om_output_t *om_output);

axis2_char_t* AXIS2_CALL
axis2_om_attribute_get_localname(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_om_attribute_get_value(axis2_om_attribute_t *om_attribute,
                             axis2_env_t **env);

axis2_om_namespace_t* AXIS2_CALL 
axis2_om_attribute_get_namespace(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_localname(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env,
                                 const axis2_char_t *localname);

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_value(axis2_om_attribute_t *om_attribute,
                             axis2_env_t **env,
                             const axis2_char_t *value);

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_namespace(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env,
                                 axis2_om_namespace_t *om_namespace);
                                                                                                                                                                                             
/*********************************** axis2_om_attribute_impl_t ************************/

typedef struct axis2_om_attribute_impl
{
    axis2_om_attribute_t om_attribute;
    /** localname of this attribute  */
    axis2_char_t *localname;
    /** value of this attribute */
    axis2_char_t *value;
    /** attribute namespace */
    axis2_om_namespace_t *ns;
    /** store qname here */
    axis2_qname_t *qname;

}axis2_om_attribute_impl_t;

/***************************************** macro *******************************/

#define AXIS2_INTF_TO_IMPL(om_attr) ((axis2_om_attribute_impl_t*)om_attr)


/*************************************** ***************************************/

AXIS2_DECLARE(axis2_om_attribute_t*)
axis2_om_attribute_create (axis2_env_t **env,
                           const axis2_char_t * localname,
                           const axis2_char_t * value,
                           axis2_om_namespace_t * ns)
{
    axis2_om_attribute_impl_t *attribute_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    /* localname is mandatory */
    AXIS2_FUNC_PARAM_CHECK(localname, env, NULL);
    
    attribute_impl = (axis2_om_attribute_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                                               sizeof (axis2_om_attribute_impl_t));
    if (!attribute_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    /**  initialize fields */
    attribute_impl->localname = NULL;
    attribute_impl->value     = NULL;
    attribute_impl->ns        = NULL;
    attribute_impl->om_attribute.ops = NULL;
    attribute_impl->qname = NULL;
    
    attribute_impl->localname =(axis2_char_t*) AXIS2_STRDUP(localname,env);
    if (!(attribute_impl->localname))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE ((*env)->allocator, attribute_impl);
        return NULL;
    }
    if(NULL != value)
    {
        attribute_impl->value =(axis2_char_t*) AXIS2_STRDUP (value,env);
        if (!(attribute_impl->value))
        {
            AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_FREE ((*env)->allocator, attribute_impl->localname);
            AXIS2_FREE ((*env)->allocator, attribute_impl);
            return NULL;
        }
    }
    attribute_impl->ns = ns;

    attribute_impl->om_attribute.ops = (axis2_om_attribute_ops_t*)AXIS2_MALLOC(
                                    (*env)->allocator,
                                    sizeof (axis2_om_attribute_ops_t));
                                    
    if (!(attribute_impl->om_attribute.ops))
    {
        AXIS2_FREE ((*env)->allocator, attribute_impl->value);
        AXIS2_FREE ((*env)->allocator, attribute_impl->localname);
        AXIS2_FREE ((*env)->allocator, attribute_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    attribute_impl->om_attribute.ops->free = axis2_om_attribute_free;
    attribute_impl->om_attribute.ops->get_qname = axis2_om_attribute_get_qname;
    attribute_impl->om_attribute.ops->serialize = axis2_om_attribute_serialize; 
    
    attribute_impl->om_attribute.ops->get_localname = axis2_om_attribute_get_localname;
    attribute_impl->om_attribute.ops->get_value = axis2_om_attribute_get_value;
    attribute_impl->om_attribute.ops->get_namespace = axis2_om_attribute_get_namespace;
    
    attribute_impl->om_attribute.ops->set_localname = axis2_om_attribute_set_localname;
    attribute_impl->om_attribute.ops->set_namespace = axis2_om_attribute_set_namespace;
    attribute_impl->om_attribute.ops->set_value = axis2_om_attribute_set_value;
    
    return &(attribute_impl->om_attribute);
}


axis2_status_t AXIS2_CALL 
axis2_om_attribute_free (axis2_om_attribute_t *om_attribute,
                         axis2_env_t **env)
{   
    axis2_om_attribute_impl_t *attribute_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    attribute_impl = AXIS2_INTF_TO_IMPL(om_attribute);

    if (attribute_impl->localname)
    {
        AXIS2_FREE ((*env)->allocator, attribute_impl->localname);
        attribute_impl->localname = NULL;
    }
    if (attribute_impl->value)
    {
        AXIS2_FREE  ((*env)->allocator, attribute_impl->value);
        attribute_impl->value = NULL;
    }
    if(attribute_impl->qname)
    {
        AXIS2_QNAME_FREE(attribute_impl->qname, env);
        attribute_impl->qname = NULL;
    }
    AXIS2_FREE((*env)->allocator, om_attribute->ops);
    AXIS2_FREE  ((*env)->allocator,attribute_impl);
    return AXIS2_SUCCESS;
}

axis2_qname_t * AXIS2_CALL 
axis2_om_attribute_get_qname (axis2_om_attribute_t *om_attribute,
                                   axis2_env_t **env)
{
    axis2_qname_t *qname = NULL;
    axis2_om_attribute_impl_t *attribute_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    attribute_impl = AXIS2_INTF_TO_IMPL(om_attribute);
    if(!(attribute_impl->qname))
    {
        if (attribute_impl->ns)
        {
            qname = axis2_qname_create (env,
                    attribute_impl->localname,
                    AXIS2_OM_NAMESPACE_GET_URI(attribute_impl->ns, env),
                    AXIS2_OM_NAMESPACE_GET_PREFIX(attribute_impl->ns, env));
        }                                
        else
        {    qname = axis2_qname_create (env,attribute_impl->localname,
                                    NULL,  NULL);
        }
        attribute_impl->qname = qname;
        return qname;                                        
    }
    return attribute_impl->qname;
}


axis2_status_t AXIS2_CALL
axis2_om_attribute_serialize (axis2_om_attribute_t *om_attribute,
                              axis2_env_t **env,
                              axis2_om_output_t *om_output)
{
    int status = AXIS2_SUCCESS;
    axis2_om_attribute_impl_t *attribute = NULL;
    
    
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    AXIS2_FUNC_PARAM_CHECK(om_output, env, AXIS2_FAILURE);
    attribute = AXIS2_INTF_TO_IMPL(om_attribute);
        
    if (attribute->ns && AXIS2_OM_NAMESPACE_GET_URI(attribute->ns,env) && 
        AXIS2_OM_NAMESPACE_GET_PREFIX(attribute->ns,env))
    {    
        status = axis2_om_output_write (om_output, env, AXIS2_OM_ATTRIBUTE, 4,
                                        attribute->localname,
                                        attribute->value,
                                        AXIS2_OM_NAMESPACE_GET_URI(attribute->ns, env),
                                        AXIS2_OM_NAMESPACE_GET_PREFIX(attribute->ns, env));
    }                                   
    else if (attribute->ns && AXIS2_OM_NAMESPACE_GET_URI(attribute->ns,env))
    {
        status = axis2_om_output_write (om_output, env, AXIS2_OM_ATTRIBUTE, 3, 
                                        attribute->localname,
                                        attribute->value,
                                        AXIS2_OM_NAMESPACE_GET_URI(attribute->ns,env));
    }                                   
    else
    {
        status = axis2_om_output_write ( om_output, env, AXIS2_OM_ATTRIBUTE, 2,
                                         attribute->localname,
                                         attribute->value);
    }                                   
    return status;
}

axis2_char_t* AXIS2_CALL
axis2_om_attribute_get_localname(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_attribute)->localname;

}                                 

axis2_char_t* AXIS2_CALL
axis2_om_attribute_get_value(axis2_om_attribute_t *om_attribute,
                             axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_attribute)->value;
}                             

axis2_om_namespace_t* AXIS2_CALL 
axis2_om_attribute_get_namespace(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_attribute)->ns;
}                                 

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_localname(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env,
                                 const axis2_char_t *localname)
{
    axis2_om_attribute_impl_t *attr_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_FUNC_PARAM_CHECK(localname, env, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(om_attribute);
    
    if(NULL != (attr_impl->localname))
    {
        AXIS2_FREE((*env)->allocator, attr_impl->localname);
        attr_impl->localname = NULL;
    }
    
    attr_impl->localname = (axis2_char_t*)AXIS2_STRDUP(localname,env);
    
    if(!(attr_impl->localname))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}                                 

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_value(axis2_om_attribute_t *om_attribute,
                             axis2_env_t **env,
                             const axis2_char_t *value)
{
    axis2_om_attribute_impl_t *attr_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_FUNC_PARAM_CHECK(value, env, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(om_attribute);
    if(NULL != attr_impl->value)
    {
        AXIS2_FREE((*env)->allocator, attr_impl->value);
        attr_impl->value = NULL;
    }
    
    attr_impl->value = (axis2_char_t*)AXIS2_STRDUP(value, env);
    if(!(attr_impl->value))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}                             

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_namespace(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env,
                                 axis2_om_namespace_t *om_namespace)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_FUNC_PARAM_CHECK(om_namespace, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_attribute)->ns = om_namespace;
    return AXIS2_SUCCESS;
}
