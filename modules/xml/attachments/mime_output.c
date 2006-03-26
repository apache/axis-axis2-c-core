/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *mime_output
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_mime_output.h>
#include <axis2_data_handler.h>
#include <axis2_mime_body_part.h>
#include <axis2_string.h>
/** 
 * @brief Flow struct impl
 *	Axis2 Flow impl  
 */


axis2_char_t AXIS2_CRLF[] =  { 13, 10 };
int AXIS2_CRLF_LENGTH = 2;

typedef struct axis2_mime_output_impl
{
	axis2_mime_output_t mime_output;
    /**
     * Field list
     *private XMLStreamWriter xmlWriter;
     *private OutputStream outStream;
     *private LinkedList binaryNodeList = new LinkedList();
     *private StringWriter bufferedSOAPBody;
     *private OMOutputFormat format = new OMOutputFormat();
     */
    
    
} axis2_mime_output_impl_t;

#define AXIS2_INTF_TO_IMPL(mime_output) ((axis2_mime_output_impl_t *)(mime_output))
#define AXIS2_MIME_BOUNDARY_BYTE 45
#define AXIS2_MIME_CRLF_BYTE 13
/* private static byte[] CRLF =  { 13, 10 }; */

axis2_status_t axis2_char_2_byte(axis2_env_t **env, axis2_char_t *char_buffer, axis2_byte_t **byte_buffer, int *byte_buffer_size);

axis2_status_t axis2_char_2_byte(axis2_env_t **env, axis2_char_t *char_buffer, axis2_byte_t **byte_buffer, int *byte_buffer_size)
{
    int length;
    int i = 0;
    axis2_byte_t *bytes;
    /*env check*/
    length = AXIS2_STRLEN(char_buffer);
    bytes = AXIS2_MALLOC((*env)->allocator, length * sizeof(axis2_byte_t));
    /*@TODO do the char to byte conversion*/
    
    for(i = 0; i < length; i++)
    {
        bytes[i] = (axis2_byte_t) char_buffer[i];
    }
    byte_buffer = &bytes;
    *byte_buffer_size = length;
}

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_mime_output_free (axis2_mime_output_t *mime_output, axis2_env_t **env);

axis2_byte_t * AXIS2_CALL
axis2_mime_output_complete(axis2_mime_output_t *mime_output,
                            axis2_env_t **env, axis2_byte_t **output_stream, 
                            int *output_stream_size, 
                            axis2_char_t *string_to_write, 
                            axis2_array_list_t *binary_node_list,
                            axis2_char_t *boundary, axis2_char_t *content_id,
                            axis2_char_t *char_set_encoding,axis2_char_t *soap_content_type);
axis2_status_t * AXIS2_CALL
axis2_start_writing_mime (axis2_mime_output_t *mime_output, 
                            axis2_env_t **env, axis2_byte_t **output_stream, 
                            int *output_stream_size, axis2_char_t *boundary);

axis2_mime_body_part_t * AXIS2_CALL
axis2_create_mime_body_part (axis2_mime_output_t *mime_output, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_write_body_part (axis2_mime_output_t *mime_output, axis2_env_t **env, 
                        axis2_byte_t **output_stream, int *output_stream_size, 
                        axis2_mime_body_part_t *part, axis2_char_t *boundary);

axis2_status_t AXIS2_CALL
axis2_write_mime_boundary (axis2_mime_output_t *mime_output, axis2_env_t **env, 
                            axis2_byte_t **output_stream, int *output_stream_size, 
                            axis2_char_t *boundary);

axis2_status_t AXIS2_CALL
axis2_write_finish_writing_mime (axis2_mime_output_t *mime_output, axis2_env_t **env, 
                        axis2_byte_t **output_stream, int *output_stream_size, axis2_char_t *boundary);

axis2_char_t * AXIS2_CALL
axis2_mime_output_get_content_type_for_mime (axis2_mime_output_t *mime_output, axis2_env_t **env, 
                        axis2_char_t *boundary, axis2_char_t *content_id, axis2_char_t *char_set_encoding, axis2_char_t *soap_content_type);

/************************** End of Function headers ************************/

AXIS2_DECLARE(axis2_mime_output_t *)
axis2_mime_output_create (axis2_env_t **env)
{
    axis2_mime_output_impl_t *mime_output_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	mime_output_impl = (axis2_mime_output_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_mime_output_impl_t));
		
	if(NULL == mime_output_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	
    mime_output_impl->mime_output.ops = NULL;
    
	mime_output_impl->mime_output.ops = AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_mime_output_ops_t));
	if(NULL == mime_output_impl->mime_output.ops)
    {
        axis2_mime_output_free(&(mime_output_impl->mime_output), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	mime_output_impl->mime_output.ops->free =  axis2_mime_output_free;
    mime_output_impl->mime_output.ops->complete = axis2_mime_output_complete;
    mime_output_impl->mime_output.ops->get_content_type_for_mime = axis2_mime_output_get_content_type_for_mime; 
	return &(mime_output_impl->mime_output);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL    

axis2_mime_output_free (axis2_mime_output_t *mime_output, axis2_env_t **env)
{
    axis2_mime_output_impl_t *mime_output_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mime_output_impl = AXIS2_INTF_TO_IMPL(mime_output);
    
    if(NULL != mime_output->ops)
    {
        AXIS2_FREE((*env)->allocator, mime_output->ops);
        mime_output->ops = NULL;
    }
    
    if(mime_output_impl)
    {
        AXIS2_FREE((*env)->allocator, mime_output_impl);
        mime_output_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_byte_t * AXIS2_CALL
axis2_mime_output_complete(axis2_mime_output_t *mime_output,
                            axis2_env_t **env, axis2_byte_t **output_stream, 
                            int *output_stream_size, 
                            axis2_char_t *string_to_write, 
                            axis2_array_list_t *binary_node_list,
                            axis2_char_t *boundary, axis2_char_t *content_id,
                            axis2_char_t *char_set_encoding,axis2_char_t *soap_content_type)
{
            /*axis2_byte_t *mime_stream = NULL;
            AXIS2_CRLF_LENGTH
            axis2_byte_t *root_mime_body_part = NULL;
            Define this
            int root_mime_body_part_length = 0;
            int mime_stream_length = (AXIS2_CRLF_LENGTH * 2) + (root_mime_body_part_length);
            mime_stream = AXIS2_MALLOC((*env)->allocator, (mime_stream_length * sizeof(mime_stream_length)));
            */
    axis2_char_t *encoding_string = NULL;
    axis2_char_t *header_value = NULL;
    axis2_char_t *content_id_string = NULL;    

    AXIS2_ENV_CHECK(env, NULL); 
         
    axis2_start_writing_mime (mime_output, env, output_stream, output_stream_size, boundary);

    encoding_string = AXIS2_STRDUP("text/xml; charset=", env);
    AXIS2_STRACAT(encoding_string, char_set_encoding, env);    
       
    axis2_data_handler_t *data_handler = axis2_data_handler_create(env, (void*)string_to_write, encoding_string);
    
    axis2_mime_body_part_t *root_mime_body_part = axis2_mime_body_part_create(env);
    /*@TODO please check the correctness of the parameters passed*/
    axis2_mime_body_part_set_data_handler(root_mime_body_part, env ,data_handler);

    header_value = AXIS2_STRDUP("application/xop+xml; charset=", env);
    AXIS2_STRACAT(header_value, char_set_encoding, env);
    AXIS2_STRACAT(header_value, "; typeAXIS2_CRLF=\"", env);
    AXIS2_STRACAT(header_value, soap_content_type, env);
    AXIS2_STRACAT(header_value, "\";", env);
    /*@TODO please check the correctness of the parameters passed*/
    axis2_mime_body_part_add_header(root_mime_body_part, env, "content-transfer-encoding","binary");

    AXIS2_STRACAT(content_id_string, "<", env);
    AXIS2_STRACAT(content_id_string, content_id, env);
    AXIS2_STRACAT(content_id_string, ">", env);
    /*@TODO please check the correctness of the parameters passed*/
    axis2_mime_body_part_add_header(root_mime_body_part, env, "content-id",content_id_string);
    /***/
    /*@TODO please check the correctness of the parameters passed*/
    axis2_write_body_part (mime_output, env, output_stream, output_stream_size, root_mime_body_part, boundary);
            
            /*javax.activation.DataHandler dh = new javax.activation.DataHandler(writer.toString(),
                    "text/xml; charset=" + char_set_encoding);
            MimeBodyPart rootMimeBodyPart = new MimeBodyPart();
            rootMimeBodyPart.setDataHandler(dh);
            
            rootMimeBodyPart.addHeader("content-type",
                    "application/xop+xml; charset=" + char_set_encoding + 
					"; typeAXIS2_CRLF=\""+soap_content_type+"\";");
            rootMimeBodyPart.addHeader("content-transfer-encoding", "binary");
            rootMimeBodyPart.addHeader("content-id","<"+content_id+">");
            */
    
            /*writeBodyPart(outStream, rootMimeBodyPart, boundary);*/
    
    
            
            /*
            Iterator binaryNodeIterator = binary_node_list.iterator();
            while (binaryNodeIterator.hasNext()) {
                OMText binaryNode = (OMText) binaryNodeIterator.next();
                writeBodyPart(outStream, createMimeBodyPart(binaryNode),
                        boundary);

            finishWritingMime(outStream);
            */
    }


/**
  * This will write the boundary with CRLF data_handler 
  * The method is a private method and is called from axis2_output_complete method
  */

axis2_status_t * AXIS2_CALL
axis2_start_writing_mime (axis2_mime_output_t *mime_output,
                            axis2_env_t **env, axis2_byte_t **output_stream,
                            int *output_stream_size, axis2_char_t *boundary)
{
    AXIS2_ENV_CHECK(env, NULL);     
    axis2_write_mime_boundary (mime_output, env, output_stream, output_stream_size, boundary);
}


 
/*
    public static void writeMimeBoundary(OutputStream outStream,
                                         axis2_char_t *boundary) throws IOException {
        outStream.write(new byte[]{45, 45});
        outStream.write(boundary.getBytes());
    }
*/
/*@TODO please check the coretness of the following method*/
axis2_status_t AXIS2_CALL
axis2_write_mime_boundary (axis2_mime_output_t *mime_output, axis2_env_t **env,
                            axis2_byte_t **output_stream, int *output_stream_size, 
                            axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer;
    axis2_byte_t *byte_stream;
    int size;
        
    AXIS2_ENV_CHECK(env, NULL);
    
    axis2_char_2_byte(env, boundary, &byte_buffer, &size);
    byte_stream = AXIS2_MALLOC((*env)->allocator, (size + 1) * sizeof(axis2_byte_t));
    byte_stream[size + 1] = AXIS2_MIME_BOUNDARY_BYTE;
    
    
    AXIS2_FREE((*env)->allocator, byte_buffer);
    
    output_stream = &byte_stream;
    output_stream_size = size + 1;
        
    return AXIS2_SUCCESS;
}


axis2_mime_body_part_t * AXIS2_CALL
axis2_create_mime_body_part (axis2_mime_output_t *mime_output, axis2_env_t **env)
/*public static MimeBodyPart createMimeBodyPart(OMText node)*/
{
    /**    
      *MimeBodyPart mimeBodyPart = new MimeBodyPart();
      *mimeBodyPart.setDataHandler((javax.activation.DataHandler)node.getDataHandler());
      *mimeBodyPart.addHeader("content-id", "<"+node.getContentID()+">");
      *mimeBodyPart.addHeader("content-type", "application/octet-stream");
      *mimeBodyPart.addHeader("content-transfer-encoding", "binary");
      *return mimeBodyPart;
     */
    AXIS2_ENV_CHECK(env, NULL);     
}



/**
     * Writes a CRLF for the earlier boundary then the BodyPart data
     * with headers followed by boundary. Writes only the boundary. No more, int *output_stream_size, 
     * CRLF's are written after that.
     *
     */
/*  
  public static void writeBodyPart(OutputStream outStream,
                                     MimeBodyPart part, 
                                     axis2_char_t *boundary) throws IOException,
            MessagingException {
        outStream.write(CRLF);
        part.writeTo(outStream);
        outStream.write(CRLF);
        writeMimeBoundary(outStream, boundary);
    }
*/


axis2_status_t AXIS2_CALL
axis2_write_body_part (axis2_mime_output_t *mime_output, axis2_env_t **env, 
                        axis2_byte_t **output_stream, int *output_stream_size, 
                        axis2_mime_body_part_t *part, axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer;
    axis2_byte_t *byte_stream;
    int size;
        
    AXIS2_ENV_CHECK(env, NULL);
    
    axis2_char_2_byte(env, boundary, &byte_buffer, &size);
    byte_stream = AXIS2_MALLOC((*env)->allocator, (size + 2) * sizeof(axis2_byte_t));
    /*@TODO pls implement this part*/
    /*
    byte_stream[0] = AXIS2_MIME_CRLF_BYTE;
    memcpy(&byte_stream[2], byte_buffer, size);
    byte_stream[3] = AXIS2_MIME_CRLF_BYTE;
    */
    AXIS2_FREE((*env)->allocator, byte_buffer);
    
    output_stream = &byte_stream;
    output_stream_size = size + 2;
        
    return AXIS2_SUCCESS;    
    
}


   /* ========================== ================================*/


/**
     * This will write "--" to the end of last boundary
     */

axis2_status_t AXIS2_CALL
axis2_write_finish_writing_mime (axis2_mime_output_t *mime_output, axis2_env_t **env, 
                        axis2_byte_t **output_stream, int *output_stream_size, axis2_char_t *boundary)
{
    axis2_byte_t *byte_buffer;
    axis2_byte_t *byte_stream;
    int size;
        
    AXIS2_ENV_CHECK(env, NULL);
    
    axis2_char_2_byte(env, boundary, &byte_buffer, &size);
    byte_stream = AXIS2_MALLOC((*env)->allocator, (size + 2) * sizeof(axis2_byte_t));
    
    AXIS2_FREE((*env)->allocator, byte_buffer);
    
    output_stream = &byte_stream;
    output_stream_size = size + 2;
        
    return AXIS2_SUCCESS;    

}
/*  public static void finishWritingMime(OutputStream outStream)
            throws IOException {
        outStream.write(new byte[]{45, 45});
    }
*/


axis2_char_t * AXIS2_CALL
axis2_mime_output_get_content_type_for_mime (axis2_mime_output_t *mime_output, axis2_env_t **env, 
                        axis2_char_t *boundary, axis2_char_t *content_id, axis2_char_t *char_set_encoding, axis2_char_t *soap_content_type)
{
    axis2_char_t *content_type_string = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    content_type_string = AXIS2_STRDUP("multipart/related", env);
    AXIS2_STRACAT(content_type_string, "; ", env);       
    AXIS2_STRACAT(content_type_string, "boundary=", env);
    AXIS2_STRACAT(content_type_string, boundary, env);
    AXIS2_STRACAT(content_type_string, "; ", env);
    AXIS2_STRACAT(content_type_string, "type=\"application/xop+xml\"", env);
    AXIS2_STRACAT(content_type_string, "; ", env);
    AXIS2_STRACAT(content_type_string, "start=\"<", env);
    AXIS2_STRACAT(content_type_string, content_id , env);
    AXIS2_STRACAT(content_type_string, ">\"", env);
    AXIS2_STRACAT(content_type_string, "; ", env);
    AXIS2_STRACAT(content_type_string, "start-info=\"", env);
    AXIS2_STRACAT(content_type_string, soap_content_type, env);
    AXIS2_STRACAT(content_type_string, "\"", env);    
    
    /*@TODO check this line*/
    return content_type_string;
}

/*
    public static axis2_char_t *getContentTypeForMime(axis2_char_t *boundary, axis2_char_t *content_id, axis2_char_t *char_set_encoding, axis2_char_t *soap_content_type) {
        StringBuffer sb = new StringBuffer();
        sb.append("multipart/related");
        sb.append("; ");
        sb.append("boundary=");
        sb.append(boundary);
        sb.append("; ");
        sb.append("type=\"application/xop+xml\"");
        sb.append("; ");
        sb.append("start=\"<" + content_id + ">\"");
        sb.append("; ");
        sb.append("start-info=\""+soap_content_type+"\"");
        return sb.toString();
    }
}
*/
