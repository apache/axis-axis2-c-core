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

#include <axis2_om_output_format.h>
#include <axis2_defines.h>
#include <axis2_soap.h>
#include <axis2_uuid_gen.h>
/** 
 * @brief Flow struct impl
 *	Axis2 Flow impl  
 */
typedef struct axis2_om_output_format_impl
{
	axis2_om_output_format_t om_output_format;
    /**
     * Field list
     */
   axis2_bool_t is_optimized;
   
   /*@TODO needs accessor methods for the following variables*/ 
   axis2_char_t *mime_boundary;
   axis2_char_t *root_content_id;
   int next_id;
   axis2_bool_t is_soap11;

   axis2_char_t *char_set_encoding;
   axis2_char_t *xml_version;
   axis2_bool_t ignore_xml_declaration;    
} axis2_om_output_format_impl_t;



#define AXIS2_INTF_TO_IMPL(om_output_format) ((axis2_om_output_format_impl_t *)(om_output_format))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_om_output_format_free (axis2_om_output_format_t *om_output_format, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_output_format_set_do_optimized(axis2_om_output_format_t *om_output_format,
                        axis2_env_t **env,
                        const axis2_bool_t is_optimized);

axis2_status_t AXIS2_CALL
axis2_om_output_format_is_optimized (axis2_om_output_format_t *om_output_format,
                        axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_om_output_format_get_content_type (axis2_om_output_format_t *om_output_format, axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_om_output_format_get_mime_boundary(axis2_om_output_format_t *om_output_format, axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_om_output_format_get_root_content_id(axis2_om_output_format_t *om_output_format, axis2_env_t **env);

axis2_status_t  AXIS2_CALL
axis2_om_output_format_set_char_set_encoding(axis2_om_output_format_t *om_output_format, axis2_env_t **env, axis2_char_t *char_set_encoding);

axis2_char_t*  AXIS2_CALL
axis2_om_output_format_get_char_set_encoding(axis2_om_output_format_t *om_output_format, axis2_env_t **env);

axis2_char_t*  AXIS2_CALL
axis2_om_output_format_get_next_content_id(axis2_om_output_format_t *om_output_format, axis2_env_t **env);


axis2_char_t* AXIS2_CALL
axis2_om_output_format_get_xml_version(axis2_om_output_format_t *om_output_format, axis2_env_t **env);

axis2_status_t  AXIS2_CALL
axis2_om_output_format_set_xml_version(axis2_om_output_format_t *om_output_format, axis2_env_t **env, axis2_char_t *xml_version);

axis2_bool_t  AXIS2_CALL
axis2_om_output_format_is_soap11(axis2_om_output_format_t *om_output_format, axis2_env_t **env); 

axis2_status_t  AXIS2_CALL
axis2_om_output_format_set_is_soap11(axis2_om_output_format_t *om_output_format, axis2_env_t **env, const axis2_bool_t is_soap11);

axis2_bool_t  AXIS2_CALL
axis2_om_output_format_is_ignore_xml_declaration(axis2_om_output_format_t *om_output_format, axis2_env_t **env); 

axis2_status_t  AXIS2_CALL
axis2_om_output_format_set_ignore_xml_declaration(axis2_om_output_format_t *om_output_format, axis2_env_t **env, const axis2_bool_t ignore_xml_declaration);


/************************** End of Function headers ************************/
AXIS2_DECLARE(axis2_om_output_format_t *)
axis2_om_output_format_create (axis2_env_t **env)
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	om_output_format_impl = (axis2_om_output_format_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_om_output_format_impl_t));
		
	if(NULL == om_output_format_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	om_output_format_impl->is_optimized = AXIS2_FALSE;
    om_output_format_impl->mime_boundary = NULL;
    om_output_format_impl->root_content_id = NULL;
    om_output_format_impl->next_id = 0;
    om_output_format_impl->is_soap11 = AXIS2_FALSE;
    om_output_format_impl->char_set_encoding = NULL;
    om_output_format_impl->xml_version = NULL;
    om_output_format_impl->ignore_xml_declaration = AXIS2_FALSE;    

    
    om_output_format_impl->om_output_format.ops = NULL;
    
	om_output_format_impl->om_output_format.ops = AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_om_output_format_ops_t));
	if(NULL == om_output_format_impl->om_output_format.ops)
    {
        axis2_om_output_format_free(&(om_output_format_impl->om_output_format), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

	om_output_format_impl->om_output_format.ops->free = axis2_om_output_format_free;

    om_output_format_impl->om_output_format.ops->is_optimized = axis2_om_output_format_is_optimized;
    om_output_format_impl->om_output_format.ops->set_do_optimized = axis2_om_output_format_set_do_optimized;
    om_output_format_impl->om_output_format.ops->get_content_type = axis2_om_output_format_get_content_type;
    om_output_format_impl->om_output_format.ops->get_mime_boundary = axis2_om_output_format_get_mime_boundary;
    om_output_format_impl->om_output_format.ops->get_root_content_id = axis2_om_output_format_get_root_content_id;
    om_output_format_impl->om_output_format.ops->set_char_set_encoding = axis2_om_output_format_set_char_set_encoding;
    om_output_format_impl->om_output_format.ops->get_char_set_encoding = axis2_om_output_format_get_char_set_encoding;
    om_output_format_impl->om_output_format.ops->get_next_content_id = axis2_om_output_format_get_next_content_id;
    om_output_format_impl->om_output_format.ops->get_xml_version = axis2_om_output_format_get_xml_version;
    om_output_format_impl->om_output_format.ops->set_xml_version = axis2_om_output_format_set_xml_version;
    om_output_format_impl->om_output_format.ops->is_soap11 = axis2_om_output_format_is_soap11;
    om_output_format_impl->om_output_format.ops->set_is_soap11 = axis2_om_output_format_set_is_soap11;
    om_output_format_impl->om_output_format.ops->is_ignore_xml_declaration = axis2_om_output_format_is_ignore_xml_declaration;
    om_output_format_impl->om_output_format.ops->set_ignore_xml_declaration = axis2_om_output_format_set_ignore_xml_declaration;
    
/**/    
	return &(om_output_format_impl->om_output_format);
}


/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axis2_om_output_format_free (axis2_om_output_format_t *om_output_format, axis2_env_t **env)
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);
    
    if(NULL != om_output_format->ops)
    {
        AXIS2_FREE((*env)->allocator, om_output_format->ops);
        om_output_format->ops = NULL;
    }
    
    if(om_output_format_impl)
    {
        AXIS2_FREE((*env)->allocator, om_output_format_impl);
        om_output_format_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_om_output_format_set_do_optimized(axis2_om_output_format_t *om_output_format,
                        axis2_env_t **env,
                        const axis2_bool_t is_optimized)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error , om_output_format, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_output_format)->is_optimized = is_optimized;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_om_output_format_is_optimized (axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_output_format)->is_optimized;
}

axis2_char_t* AXIS2_CALL
axis2_om_output_format_get_content_type (axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    axis2_char_t *soap_content_type = NULL;
    axis2_char_t *content_type_for_mime = NULL;
    axis2_mime_output_t *mime_output = NULL;
    axis2_char_t *mime_boundary = NULL;  
    axis2_char_t *root_content_id = NULL;
    axis2_char_t *char_set_encoding = NULL;
    
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);

    if (axis2_om_output_format_is_optimized (om_output_format, env) ) 
    {
        if ( om_output_format_impl->is_soap11) 
        {
            soap_content_type = AXIS2_SOAP_11_CONTENT_TYPE;
        }
        else 
        {
            soap_content_type = AXIS2_SOAP_12_CONTENT_TYPE;
        }
        mime_output = axis2_mime_output_create(env);
        mime_boundary = axis2_om_output_format_get_mime_boundary(om_output_format, env);  
        root_content_id = axis2_om_output_format_get_root_content_id(om_output_format, env);
        char_set_encoding = axis2_om_output_format_get_char_set_encoding(om_output_format, env);
        
        content_type_for_mime =  AXIS2_MIME_OUTPUT_GET_CONTENT_TYPE_FOR_MIME(mime_output, env, mime_boundary, root_content_id, char_set_encoding, soap_content_type);
        
        return content_type_for_mime;
        /*@TODO please check the above statements*/
        /*        return MIMEOutputUtils.getContentTypeForMime(
                getMimeBoundary(),
                getRootContentId(),
                this.getCharSetEncoding(), SOAPContentType);*/
    }
    else 
    {
        if (!om_output_format_impl->is_soap11) 
        {
            return AXIS2_SOAP_11_CONTENT_TYPE;
        }
        else
        {
            return AXIS2_SOAP_11_CONTENT_TYPE;
        }
    }
}

axis2_char_t*  AXIS2_CALL
axis2_om_output_format_get_mime_boundary(axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    axis2_char_t *uuid = NULL;

    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);

    if (om_output_format_impl->mime_boundary == NULL) 
    {
        om_output_format_impl->mime_boundary = AXIS2_STRDUP("MIMEBoundary", env);
        uuid = axis2_uuid_gen(env);
        AXIS2_STRACAT(om_output_format_impl->mime_boundary, uuid, env);
    }
    return om_output_format_impl->mime_boundary;
 }

axis2_char_t*  AXIS2_CALL
axis2_om_output_format_get_root_content_id(axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    axis2_char_t *uuid = NULL;

    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);

    if (om_output_format_impl->root_content_id == NULL) 
    {
        om_output_format_impl->root_content_id = AXIS2_STRDUP("0.", env);
        uuid = axis2_uuid_gen(env);
        
        /*@TODO iimplement the following line */
        AXIS2_STRACAT(om_output_format_impl->root_content_id, uuid, env);
        AXIS2_STRACAT(om_output_format_impl->root_content_id, "@apache.org", env);
        
    }
    return om_output_format_impl->root_content_id;
 }

axis2_status_t AXIS2_CALL
axis2_om_output_format_set_char_set_encoding(axis2_om_output_format_t *om_output_format, axis2_env_t **env, axis2_char_t *char_set_encoding) 
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);
    
    om_output_format_impl->char_set_encoding = char_set_encoding;
    return AXIS2_SUCCESS;
}
 

axis2_char_t*  AXIS2_CALL
axis2_om_output_format_get_char_set_encoding(axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);
    return om_output_format_impl->char_set_encoding;
 }

 
 
axis2_char_t*  AXIS2_CALL
axis2_om_output_format_get_next_content_id(axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    axis2_char_t *next_id_value = NULL;
    axis2_char_t id_str[256];
    axis2_char_t *uuid = NULL;
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);

    om_output_format_impl->next_id++;
    sprintf(id_str, "%d", om_output_format_impl->next_id);
    AXIS2_STRACAT(next_id_value, id_str, env);    
    AXIS2_STRACAT(next_id_value,".",env);
    uuid = axis2_uuid_gen(env);
    AXIS2_STRACAT(next_id_value, uuid, env);
    AXIS2_STRACAT(next_id_value,"@apache.org",env);    
    return next_id_value;
}
 
axis2_char_t*  AXIS2_CALL
axis2_om_output_format_get_xml_version(axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);

    return om_output_format_impl->xml_version;
}

axis2_status_t  AXIS2_CALL
axis2_om_output_format_set_xml_version(axis2_om_output_format_t *om_output_format, axis2_env_t **env, axis2_char_t *xml_version) 
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);
    
    om_output_format_impl->xml_version = xml_version;
    return AXIS2_SUCCESS;
}


axis2_bool_t  AXIS2_CALL
axis2_om_output_format_is_soap11(axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);

    return om_output_format_impl->is_soap11;
}

axis2_status_t  AXIS2_CALL
axis2_om_output_format_set_is_soap11(axis2_om_output_format_t *om_output_format, axis2_env_t **env, const axis2_bool_t is_soap11) 
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);
    
    om_output_format_impl->is_soap11 = is_soap11;
    return AXIS2_SUCCESS;
}

 
 
axis2_bool_t  AXIS2_CALL
axis2_om_output_format_is_ignore_xml_declaration(axis2_om_output_format_t *om_output_format, axis2_env_t **env) 
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);

    return om_output_format_impl->ignore_xml_declaration;
}

axis2_status_t  AXIS2_CALL
axis2_om_output_format_set_ignore_xml_declaration(axis2_om_output_format_t *om_output_format, axis2_env_t **env, const axis2_bool_t ignore_xml_declaration) 
{
    axis2_om_output_format_impl_t *om_output_format_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_format_impl = AXIS2_INTF_TO_IMPL(om_output_format);
    
    om_output_format_impl->ignore_xml_declaration = ignore_xml_declaration;
    return AXIS2_SUCCESS;
}
