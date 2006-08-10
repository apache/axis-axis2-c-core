/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
#include <axis2_defines.h>
#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_buffer.h>
#include <oxs_axiom.h>
#include <oxs_error.h>


static unsigned int  g_initial_size = OXS_BUFFER_INITIAL_SIZE;

AXIS2_EXTERN oxs_buffer_ptr AXIS2_CALL
oxs_create_buffer(const axis2_env_t *env, unsigned int size)
{
    oxs_buffer_ptr buf = NULL;
    buf = (oxs_buffer_ptr)AXIS2_MALLOC(env->allocator,sizeof(oxs_buffer));
   
    buf = oxs_buffer_initialize(env, buf, size); 
    
    return buf;
}

AXIS2_EXTERN oxs_buffer_ptr AXIS2_CALL
oxs_buffer_initialize(const axis2_env_t *env ,oxs_buffer_ptr buf,  unsigned int size)
{
    int ret;
    buf->data = NULL;
    buf->size = 0;
    buf->max_size = 0;
    buf->alloc_mode = oxs_alloc_mode_double;
    
    ret = oxs_buffer_set_max_size(env, buf, size);
    if(ret<0){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DEFAULT,
                     "oxs_buffer_set_max_size");
        return NULL;
    }
    return buf;
    
}

AXIS2_EXTERN int AXIS2_CALL
oxs_free_buffer(const axis2_env_t *env, oxs_buffer_ptr buf)
{
    if(!buf){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_free_buffer failed");
        return (-1);
    }   
 
    AXIS2_FREE(env->allocator, buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->max_size = 0;
    
    AXIS2_FREE(env->allocator, buf);
    buf = NULL;
    return (0);
}

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_remove_head(const axis2_env_t *env,
        oxs_buffer_ptr buf, unsigned int size)
{
    if(size < buf->size) {
        if(buf->data != NULL){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_buffer_remove_head failed"); 
            return  (-1);
        }
        buf->size -= size;
        memmove(buf->data, buf->data + size, buf->size);
    } else {
        buf->size = 0;
    }

    if(buf->size < buf->max_size) {
        if(buf->data != NULL){
             oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_buffer_remove_head failed");
             return  (-1);
        }
        memset(buf->data + buf->size, 0, buf->max_size - buf->size);
    }
    return(0);

}
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_buffer_remove_tail(const axis2_env_t *env, 
        oxs_buffer_ptr buf,unsigned int size)
{

    if(size < buf->size) {
        buf->size -= size;
    } else {
        buf->size = 0;
    }
    if(buf->size < buf->max_size) {
        if(buf->data != NULL){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     ""); 
            return  (-1);
        }
        memset(buf->data + buf->size, 0, buf->max_size - buf->size);
    }
    return(0);
}

AXIS2_EXTERN oxs_buffer_ptr AXIS2_CALL
oxs_string_to_buffer(const axis2_env_t *env, axis2_char_t* string)
{
    oxs_buffer_ptr buf = NULL;
    unsigned int size ;
    unsigned char* data = NULL;
 
    if(!string){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");     
        return NULL;
    }
    size =(unsigned int) AXIS2_STRLEN(string);
    data = (unsigned char*)AXIS2_STRDUP(string, env);
    
    buf = oxs_create_buffer(env, size);        
    buf->data = data;
    buf->size = size;
    /*buf->max_size = size;*/
    buf->alloc_mode = oxs_alloc_mode_double;
 
    return buf; 
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_buffer_to_string(const axis2_env_t *env, oxs_buffer_ptr buf)
{
    axis2_char_t* string;
    
    if(!buf){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");        
         return NULL;
    }
    
    string = (axis2_char_t*)buf->data;
    return string;

}

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_append(const axis2_env_t *env,
        oxs_buffer_ptr buf, unsigned char* data, unsigned int size)
{

    if(!buf){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
        return(-1);
    }

    if(size > 0) {
        oxs_buffer_set_max_size(env, buf, buf->size + size);
        if(!data){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
            return (-1);
        }

        memcpy(buf->data + buf->size, data, size);
        buf->size += size;
    }

    return(0);
}

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_prepend(const axis2_env_t *env,
        oxs_buffer_ptr buf, unsigned char* data, unsigned int size)
{

    if(!buf){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");        
         return (-1);
    }

    if(size > 0) {
        if(!data) {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
            return (-1);
        }

        buf->max_size = buf->size + size;

        memmove(buf->data + size, buf->data, buf->size);
        memcpy(buf->data, data, size);
        buf->size += size;
    }

    return(0);
}

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_set_size(const axis2_env_t *env, oxs_buffer_ptr buf, unsigned int size)
{
    int ret;

    if(buf == NULL){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "buffer is NULL");
         return (-1);
    }
    ret = oxs_buffer_set_max_size(env, buf, size);
    if(ret < 0) {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_buffer_set_max_size failed");
        return(-1);
    }
    buf->size = size;
    return(0);
}

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_set_max_size(const axis2_env_t *env, oxs_buffer_ptr buf, unsigned int size)
{
    unsigned char* new_data;
    unsigned int new_size = 0;

    if(buf == NULL) {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
        return (-1);
    }

    if(size <= buf->max_size) {
       return(0);
    }

    switch(buf->alloc_mode) {
    case oxs_alloc_mode_exact:
        new_size = size + 8;
        break;
    case oxs_alloc_mode_double:
        new_size = 2 * size + 32;
        break;
    }

    if(new_size < g_initial_size) {
       new_size = g_initial_size;
    }


    if(buf->data != NULL) {
        new_data = (unsigned char*)AXIS2_REALLOC(env->allocator, buf->data, new_size);
    } else {
        new_data = (unsigned char*)AXIS2_MALLOC(env->allocator, new_size);
    }

    if(new_data == NULL) {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
        return(-1);
    }

    buf->data = new_data;
    buf->max_size = new_size;

    if(buf->size < buf->max_size) {
        if(buf->data == NULL){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
             return (-1);
        }
        memset(buf->data + buf->size, 0, buf->max_size - buf->size);
    }

    return(0);

}

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_read_file(const axis2_env_t *env, oxs_buffer_ptr buf, const char* filename)
{
    unsigned char buffer[1024];
    FILE* f;
    int ret, len;

    f = fopen(filename, "rb");
    if(f == NULL) {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
        return (-1);
    }

    while(1)
    {
        len = fread(buffer, 1, sizeof(buffer), f);
        if(len == 0){
            break; /*Stop reading*/
        }else if(len < 0){
            fclose(f);
            return(-1);
        }
        ret = oxs_buffer_append(env, buf, buffer, len);
        if(ret < 0){
            fclose(f);
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
            return (-1);
        }
       
        /*Alright so far everything is fine. So let's close the output*/
        fclose(f);
        return (0); 
    }/*End of while*/
    return(0);
}

AXIS2_EXTERN int AXIS2_CALL
oxs_buffer_base64_node_content_read(const axis2_env_t *env, oxs_buffer_ptr buf, axiom_node_t *node)
{
    axis2_char_t *content = NULL;
    axis2_char_t *decoded_str = NULL;
    int ret;
    unsigned int length;
        
    content = oxs_axiom_get_node_content(env, node);    
    if(content == NULL) {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
        return (-1);
    }
   
    ret = oxs_buffer_set_max_size(env, buf, AXIS2_STRLEN(content));
    if(ret < 0)  {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "");
        return(-1);
    }

    /*OK. Now decode*/
    /*axis2_base64_decode(plain, encoded)*/
    length = axis2_base64_decode(decoded_str, content); 
    if(length < 0 ) return (-1);
    
    /*Store data in the buffer*/    
    buf->data = (unsigned char*)decoded_str;
    buf->size = length;

    return (0);  
}




