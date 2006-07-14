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

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_buffer.h>
#include <openssl_cipher_ctx.h>
#include <openssl_crypt.h>
#include <openssl/rand.h>


AXIS2_EXTERN int AXIS2_CALL  openssl_block_cipher_crypt(axis2_env_t *env, 
    openssl_evp_block_cipher_ctx_ptr bc_ctx, 
    oxs_buffer_ptr in_buf, 
    oxs_buffer_ptr out_buf, 
    int do_encrypt)
{
    EVP_CIPHER_CTX evp_ctx ;
    evp_ctx = bc_ctx->cipher_ctx;

    unsigned char *tempbuf;
    int insize, outsize, block_len;
    int outlen = 0;     

    if(!bc_ctx->ctxInitialized){
        return (-1); /* Ctx should be initialized by now*/
    }
    
    block_len = EVP_CIPHER_block_size(bc_ctx->cipher);

     /* loop until we dont have any data left in the input buffer */
    for(;;)
    {
        insize = in_buf->size;

        if(insize <= 0) break;/*No More Data!!! Quit loop*/


        outsize = out_buf->size;
        oxs_buffer_set_max_size(env, out_buf, outsize + insize + block_len);
    
        tempbuf = out_buf->data + outsize;
 
        if(!EVP_CipherUpdate(&(bc_ctx->cipher_ctx), tempbuf, &outlen, in_buf->data, insize))
        {
            /* Error!!! Do the cleanup */
            EVP_CIPHER_CTX_cleanup(&(bc_ctx->cipher_ctx));
            return -1;
        }
    
        /*set the correct size of the output buffer*/
        oxs_buffer_set_size(env, out_buf, outsize + outlen );
        printf("\noxs_buffer_set_size= %d", outsize + outlen );

        /*remove the processed data from the input*/
        oxs_buffer_remove_head(env, in_buf, insize);

     }/*End of For loop*/

     if(!EVP_CipherFinal_ex(&(bc_ctx->cipher_ctx), tempbuf, &outsize))
     {
        /* Error */
        EVP_CIPHER_CTX_cleanup(&(bc_ctx->cipher_ctx));
        return -1;
     }
     /*Now set out_buf data*/
     out_buf->data = tempbuf;
     oxs_buffer_set_size(env, out_buf, outsize + outlen );

     EVP_CIPHER_CTX_cleanup(&(bc_ctx->cipher_ctx));
     
     return 1;

}

