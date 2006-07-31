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

#ifndef OXS_CTX_H
#define OXS_CTX_H


/**
  * @file oxs_ctx.h
  * @brief 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <oxs_buffer.h>
#include <oxs_key.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup oxs_ctx Message Context 
 * @ingroup oxs_context
 * @{
 */

typedef struct _enc_ctx enc_ctx, *enc_ctx_ptr;

typedef enum  {
    oxs_operation_none =0,
    oxs_operation_sign,
    oxs_operation_verify,
    oxs_operation_encrypt,
    oxs_operation_decrypt
} oxs_operation_t;

typedef enum {
    enc_ctx_mode_encrypted_data = 0,
    enc_ctx_mode_encrypted_key
} enc_ctx_mode_t;



struct _enc_ctx{
    /*Encryption mode*/
    enc_ctx_mode_t mode;
    
    /*transformation type */
    oxs_operation_t operation;    

    /*key*/
    oxs_key_ptr key;

    /* attributes from EncryptedData or EncryptedKey */
    axis2_char_t*           id;
    axis2_char_t*           type;
    axis2_char_t*           mime_type;
    axis2_char_t*           encoding;
    axis2_char_t*           recipient;
    axis2_char_t*           carriedKeyName;

    /*attributes from EncryptionMethod*/ 
    axis2_char_t*           encmtd_algorithm;
    
    /*Key name*/
    axis2_char_t*           keyname_val;

    axiom_node_t*           enc_data_node;
    axiom_node_t*           enc_method_node;
    axiom_node_t*           key_info_node;
    axiom_node_t*           cipher_value_node;
    /*Used  in decryption process to keep the data to be decrypted*/
    axis2_char_t*           inputdata;   

}; 



AXIS2_EXTERN enc_ctx_ptr AXIS2_CALL
oxs_ctx_create_ctx(const axis2_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_free_ctx(enc_ctx_ptr ctx);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_reset_ctx(enc_ctx_ptr ctx);
/**
  <complexType name='EncryptedType' abstract='true'>
    <sequence>
      <element name='EncryptionMethod' type='xenc:EncryptionMethodType' 
               minOccurs='0'/>
      <element ref='ds:KeyInfo' minOccurs='0'/>
      <element ref='xenc:CipherData'/>
      <element ref='xenc:EncryptionProperties' minOccurs='0'/>
    </sequence>
    <attribute name='Id' type='ID' use='optional'/>
    <attribute name='Type' type='anyURI' use='optional'/>
    <attribute name='MimeType' type='string' use='optional'/>
    <attribute name='Encoding' type='anyURI' use='optional'/> 
   </complexType>

  <complexType name='EncryptionMethodType' mixed='true'>
    <sequence>
      <element name='KeySize' minOccurs='0' type='xenc:KeySizeType'/>
      <element name='OAEPparams' minOccurs='0' type='base64Binary'/>
      <any namespace='##other' minOccurs='0' maxOccurs='unbounded'/>
    </sequence>
    <attribute name='Algorithm' type='anyURI' use='required'/>
  </complexType>


*/



/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_CTX_H */
