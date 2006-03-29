/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *mime_body_part
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_mime_body_part.h>

/** 
 * @brief Flow struct impl
 *	Axis2 Flow impl  
 */
typedef struct axis2_mime_body_part_impl
{
	axis2_mime_body_part_t mime_body_part;
    /**
     * Field list
     */
	/** Hash map to hold header name, value pairs */
	axis2_hash_t *header_map;
} axis2_mime_body_part_impl_t;



#define AXIS2_INTF_TO_IMPL(mime_body_part) ((axis2_mime_body_part_impl_t *)(mime_body_part))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_mime_body_part_free (axis2_mime_body_part_t *mime_body_part, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_mime_body_part_add_header (axis2_mime_body_part_t *mime_body_part, axis2_env_t **env,
    axis2_char_t *name, axis2_char_t *value);

axis2_status_t AXIS2_CALL
axis2_mime_body_part_set_data_handler (axis2_mime_body_part_t *mime_body_part, axis2_env_t **env, axis2_data_handler_t *data_handler); 

axis2_status_t AXIS2_CALL
axis2_mime_body_part_write_to (axis2_mime_body_part_t *mime_body_part, axis2_env_t **env, 
                                axis2_byte_t **output_stream, int *output_stream_size, 
                                axis2_char_t *ignore_list);
/************************** End of Function headers ************************/

AXIS2_DECLARE(axis2_mime_body_part_t *)
axis2_mime_body_part_create (axis2_env_t **env)
{
    axis2_mime_body_part_impl_t *mime_body_part_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	mime_body_part_impl = (axis2_mime_body_part_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_mime_body_part_impl_t));
		
	if(NULL == mime_body_part_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	
    mime_body_part_impl->mime_body_part.ops = NULL;
    
	mime_body_part_impl->mime_body_part.ops = AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_mime_body_part_ops_t));
	if(NULL == mime_body_part_impl->mime_body_part.ops)
    {
        axis2_mime_body_part_free(&(mime_body_part_impl->mime_body_part), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

	mime_body_part_impl->mime_body_part.ops->free =  axis2_mime_body_part_free;
    mime_body_part_impl->mime_body_part.ops->add_header = axis2_mime_body_part_add_header;
    mime_body_part_impl->mime_body_part.ops->set_data_handler = axis2_mime_body_part_set_data_handler; 
    mime_body_part_impl->mime_body_part.ops->write_to = axis2_mime_body_part_write_to;
    
    return &(mime_body_part_impl->mime_body_part);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axis2_mime_body_part_free (axis2_mime_body_part_t *mime_body_part, axis2_env_t **env)
{
    axis2_mime_body_part_impl_t *mime_body_part_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mime_body_part_impl = AXIS2_INTF_TO_IMPL(mime_body_part);
    
    if(NULL != mime_body_part->ops)
    {
        AXIS2_FREE((*env)->allocator, mime_body_part->ops);
        mime_body_part->ops = NULL;
    }
    
    if(mime_body_part_impl)
    {
        AXIS2_FREE((*env)->allocator, mime_body_part_impl);
        mime_body_part_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_mime_body_part_add_header (axis2_mime_body_part_t *mime_body_part, axis2_env_t **env,
    axis2_char_t *name, axis2_char_t *value) 
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	/* headers.addHeader(name, value);     */
    return AXIS2_SUCCESS;
}

/**
 * This method provides the mechanism to set this body part's content.
 * The given DataHandler object should wrap the actual content.
 * 
 * @param   dh      The DataHandler for the content
 */                 

axis2_status_t AXIS2_CALL
axis2_mime_body_part_set_data_handler (axis2_mime_body_part_t *mime_body_part, axis2_env_t **env, axis2_data_handler_t *data_handler) 
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_SUCCESS;

	/*** 
	setDataHandler(DataHandler dh) 
	this.dh = dh;
	MimeBodyPart.invalidateContentHeaders(this);
	*/
}

axis2_status_t AXIS2_CALL
axis2_mime_body_part_write_to (axis2_mime_body_part_t *mime_body_part, axis2_env_t **env, 
                                axis2_byte_t **output_stream, int *output_stream_size, 
                                axis2_char_t *ignore_list) 
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_SUCCESS;
}


/**
    static void writeTo(MimePart part, OutputStream os, String[] ignoreList)
			throws IOException, MessagingException {

	// see if we already have a LOS
	LineOutputStream los = null;
	if (os instanceof LineOutputStream) {
	    los = (LineOutputStream) os;
	} else {
	    los = new LineOutputStream(os);
	}

	// First, write out the header
	Enumeration hdrLines = part.getNonMatchingHeaderLines(ignoreList);
	while (hdrLines.hasMoreElements())
	    los.writeln((String)hdrLines.nextElement());

	// The CRLF separator between header and content
	los.writeln();

	// Finally, the content. Encode if required.
	// XXX: May need to account for ESMTP ?
	os = MimeUtility.encode(os, part.getEncoding());
	part.getDataHandler().writeTo(os);
	os.flush(); // Needed to complete encoding
    }
*/
