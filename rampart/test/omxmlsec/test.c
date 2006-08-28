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
#include <oxs_constants.h>
#include <axiom.h>
#include <axiom_xml_reader.h>
#include <axis2_env.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_enc_engine.h>


AXIS2_EXTERN axiom_node_t* AXIS2_CALL
load_sample_xml(const axis2_env_t *env,
                    axiom_node_t* tmpl,
                    axis2_char_t* filename
                    )
{
    
    axiom_document_t *doc = NULL;
    axiom_stax_builder_t *builder = NULL;
    axiom_xml_reader_t *reader = NULL;
    /*axiom_xml_writer_t *writer = NULL;*/

    reader = axiom_xml_reader_create_for_file(env, filename, NULL);
    if(!reader) printf("\n Reader is NULL");
    builder = axiom_stax_builder_create(env, reader);
    if(!builder) printf("\n builder is NULL");
    doc = axiom_document_create(env, NULL, builder);
    if(!doc) printf("\n doc is NULL");
    tmpl = AXIOM_DOCUMENT_BUILD_ALL(doc, env);

/*    tmpl = AXIOM_DOCUMENT_GET_ROOT_ELEMENT(doc, env);*/
    if(!tmpl) printf("\n tmpl is NULL");
    return tmpl;
}

axis2_env_t *test_init()
{
   axis2_allocator_t *allocator = axis2_allocator_init (NULL);
   axis2_error_t *error = (axis2_error_t*)axis2_error_create (allocator);
   axis2_env_t *env = axis2_env_create_with_error(allocator, error);
   return env;
}


int main(int argc, char *argv[])
{
   oxs_error(ERROR_LOCATION,OXS_ERROR_DEFAULT, "Api danne neee %d", 13);
   axis2_env_t *env = NULL;
   axis2_char_t *filename = "a.xml";
   axis2_char_t *doc_file = "b.xml";
   axis2_char_t *data = "OM the universal sound";
   enc_ctx_ptr ctx = NULL;
   axis2_status_t temp_status= AXIS2_FAILURE; 
   axiom_node_t *tmpl = NULL, *enc_doc = NULL;
   axis2_char_t *encrypted_result = NULL, *decrypted_result = NULL;
   FILE *outf;

   env = test_init();
   printf("--Testing started--------------------------------------------\n");
   
   if (argc > 1)
        filename = argv[1];
   tmpl = load_sample_xml(env , tmpl, filename);

   if( tmpl)
   {
        printf("load_sample_xml SUCCESS");
   }else{
        printf("load_sample_xml FAILED");
       return -1;
   }
  
   enc_doc =  load_sample_xml(env , enc_doc, doc_file); 
   if( enc_doc)
   {
        printf("load_sample_xml enc_doc SUCCESS");
   /*     data = AXIOM_NODE_TO_STRING(enc_doc, env); *//*Enable this to encrypt an xml*/
   }else{
        printf("load_sample_xml enc_doc FAILED");
       return -1;
   }

    

    /*Encrypt***************************************************/     
    ctx = oxs_ctx_create_ctx(env);    
    ctx->key = oxs_key_read_from_file(env, "deskey.bin");
    
    /*TODO Set the key name inside KeyName node*/
      
    if(!ctx){printf("\nCannot create context"); return -1;}
    temp_status = oxs_enc_encrypt_template(env, tmpl, data, ctx);
    
    if(temp_status){
        printf("oxs_enc_encrypt_template SUCCESS\n");
    }else{
        printf("oxs_enc_encrypt_template FAILURE\n");
    }
   
   encrypted_result = AXIOM_NODE_TO_STRING(tmpl, env) ;
   printf("Final template is\n %s  ",encrypted_result);
 
   outf = fopen("result.xml", "wb");
   fwrite(encrypted_result, 1, AXIS2_STRLEN(encrypted_result), outf);

   /*Decrypt **************************************************/
    #if 1
    printf("\nDecryption start\n**************************************************\n");
    ctx = oxs_ctx_create_ctx(env);    
    ctx->key = oxs_key_read_from_file(env, "deskey.bin");

    if(!ctx){printf("\nCannot create context"); return -1;}

    temp_status = oxs_enc_decrypt_template(env, tmpl, &decrypted_result, ctx);

    if(temp_status){
        printf("oxs_enc_decrypt_template SUCCESS\n");
    }else{
        printf("oxs_enc_decrypt_template FAILURE\n");
    }

    printf("Decrypted result is\n%s", decrypted_result)    ;

#endif
    return 0;
    
    
}
