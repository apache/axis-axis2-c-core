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
    axis2_om_attribute_impl_t *attribute = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    if (!localname)
    {   /* localname is mandatory */
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error,
                    AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

    attribute = (axis2_om_attribute_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                                               sizeof (axis2_om_attribute_impl_t));
    if (!attribute)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error , AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }
    /**  initialize fields */
    attribute->localname =(axis2_char_t*) AXIS2_STRDUP(localname,env);
    if (!(attribute->localname))
    {
        AXIS2_FREE ((*env)->allocator, attribute);
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }
    attribute->value =(axis2_char_t*) AXIS2_STRDUP (value,env);
    if (!(attribute->value))
    {
        AXIS2_FREE ((*env)->allocator, attribute->localname);
        AXIS2_FREE ((*env)->allocator, attribute);
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }
    attribute->ns = ns;

    /** operations */

    attribute->om_attribute.ops = (axis2_om_attribute_ops_t*)AXIS2_MALLOC ((*env)->allocator,
                      sizeof (axis2_om_attribute_ops_t));
    if (!(attribute->om_attribute.ops))
    {
        AXIS2_FREE ((*env)->allocator, attribute->value);
        AXIS2_FREE ((*env)->allocator, attribute->localname);
        AXIS2_FREE ((*env)->allocator, attribute);
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }
    attribute->om_attribute.ops->free = axis2_om_attribute_free;
    attribute->om_attribute.ops->get_qname = axis2_om_attribute_get_qname;
    attribute->om_attribute.ops->serialize = axis2_om_attribute_serialize; 
    
    attribute->om_attribute.ops->get_localname = axis2_om_attribute_get_localname;
    attribute->om_attribute.ops->get_value = axis2_om_attribute_get_value;
    attribute->om_attribute.ops->get_namespace = axis2_om_attribute_get_namespace;
    
    attribute->om_attribute.ops->set_localname = axis2_om_attribute_set_localname;
    attribute->om_attribute.ops->set_namespace = axis2_om_attribute_set_namespace;
    attribute->om_attribute.ops->set_value = axis2_om_attribute_set_value;
    
    return &(attribute->om_attribute);
}


axis2_status_t AXIS2_CALL 
axis2_om_attribute_free (axis2_om_attribute_t *om_attribute,
                              axis2_env_t **env)
{   
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env, AXIS2_FAILURE);
    if (AXIS2_INTF_TO_IMPL(om_attribute))
    {
        if (AXIS2_INTF_TO_IMPL(om_attribute)->localname)
        {
            AXIS2_FREE ((*env)->allocator, AXIS2_INTF_TO_IMPL(om_attribute)->localname);
        }
        if (AXIS2_INTF_TO_IMPL(om_attribute)->value)
        {
            AXIS2_FREE  ((*env)->allocator, AXIS2_INTF_TO_IMPL(om_attribute)->value);
        }
        AXIS2_FREE  ((*env)->allocator, AXIS2_INTF_TO_IMPL(om_attribute));
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}



axis2_qname_t * AXIS2_CALL 
axis2_om_attribute_get_qname (axis2_om_attribute_t *om_attribute,
                                   axis2_env_t **env)
{
    axis2_qname_t *qname = NULL;
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env, NULL);

    if (AXIS2_INTF_TO_IMPL(om_attribute)->ns)
    {
        axis2_om_namespace_t *ns = NULL;
        ns = AXIS2_INTF_TO_IMPL(om_attribute)->ns;
        
        qname = axis2_qname_create (env,
                    AXIS2_INTF_TO_IMPL(om_attribute)->localname,
                    AXIS2_OM_NAMESPACE_GET_URI(ns, env),
                    AXIS2_OM_NAMESPACE_GET_PREFIX(ns, env));
    }                                
    else
        qname = axis2_qname_create (env,AXIS2_INTF_TO_IMPL(om_attribute)->localname,
                                    NULL,  NULL);

    return qname;
}


axis2_status_t AXIS2_CALL
axis2_om_attribute_serialize (axis2_om_attribute_t *om_attribute,
                              axis2_env_t **env,
                              axis2_om_output_t *om_output)
{
    int status = AXIS2_SUCCESS;
    axis2_om_attribute_impl_t *attribute = NULL;
    
    
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env,AXIS2_FAILURE);
    
    if (!om_output)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
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
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_attribute)->localname;

}                                 

axis2_char_t* AXIS2_CALL
axis2_om_attribute_get_value(axis2_om_attribute_t *om_attribute,
                             axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_attribute)->value;
}                             

axis2_om_namespace_t* AXIS2_CALL 
axis2_om_attribute_get_namespace(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_attribute)->ns;
}                                 

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_localname(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env,
                                 const axis2_char_t *localname)
{
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env, AXIS2_FAILURE);
    if(!localname)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    AXIS2_INTF_TO_IMPL(om_attribute)->localname = (axis2_char_t*)AXIS2_STRDUP(localname,env);
    return AXIS2_SUCCESS;
}                                 

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_value(axis2_om_attribute_t *om_attribute,
                             axis2_env_t **env,
                             const axis2_char_t *value)
{
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env, AXIS2_FAILURE);
    if(!value)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    AXIS2_INTF_TO_IMPL(om_attribute)->value = (axis2_char_t*)AXIS2_STRDUP(value,env);
    return AXIS2_SUCCESS;
}                             

axis2_status_t AXIS2_CALL
axis2_om_attribute_set_namespace(axis2_om_attribute_t *om_attribute,
                                 axis2_env_t **env,
                                 axis2_om_namespace_t *om_namespace)
{
    AXIS2_FUNC_PARAM_CHECK(om_attribute, env, AXIS2_FAILURE);
    if(!om_namespace)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    AXIS2_INTF_TO_IMPL(om_attribute)->ns = om_namespace;
    return AXIS2_SUCCESS;
}
