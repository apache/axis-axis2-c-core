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
#include <oxs_error.h>
#include <oxs_key_mgr.h>

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_key_mgr_load_key(const axis2_env_t *env,
    oxs_asym_ctx_t *ctx)
{
    axis2_char_t *filename = NULL;
    axis2_char_t *password = "1234";
    axis2_status_t status = AXIS2_FAILURE;
    openssl_x509_format_t format;
    openssl_pkey_t *open_pkey = NULL;
    oxs_x509_cert_t *oxs_cert = NULL;

    X509 *cert = NULL;
    STACK_OF(X509) *ca = NULL;
    EVP_PKEY *pkey = NULL;

    /*Get file to be loaded. Can be either in PEM or PKCS12 format*/
    filename = oxs_asym_ctx_get_file_name(ctx, env);
    if(!filename){
        return AXIS2_FAILURE;
    }

    if(OXS_ASYM_CTX_FORMAT_PEM == oxs_asym_ctx_get_format(ctx, env)){
        format = OPENSSL_X509_FORMAT_PEM;
    }else if(OXS_ASYM_CTX_FORMAT_PKCS12 == oxs_asym_ctx_get_format(ctx, env)){
        format = OPENSSL_X509_FORMAT_PKCS12;
        status = openssl_x509_load_from_pkcs12(env, filename, password, &cert, &pkey, &ca);
 
    }
   
    /**
     *   Now we have X509 and/or Pkey. If the format is PKCS12, then we have a chance to get both certificate and pkey.
     *   If the format is PEM only, we might have only the pkey. 
     */ 
    
    /*Alright if the pkey is available, populate the openssl_pkey*/
    if(pkey){
        open_pkey = openssl_pkey_create(env);
        OPENSSL_PKEY_POPULATE(open_pkey, env, pkey, filename, OPENSSL_PKEY_TYPE_PRIVATE_KEY);
        
    }
    /*If the X509 certificate is available, populate oxs_x509_cert*/
    if(cert){
        axis2_char_t *serial = NULL;
        int serial_num = 0;

        oxs_cert = oxs_x509_cert_create(env);
        oxs_x509_cert_set_data(oxs_cert, env, openssl_x509_get_cert_data(env, cert));
        oxs_x509_cert_set_date(oxs_cert, env, openssl_x509_get_info(env, OPENSSL_X509_INFO_VALID_TO ,cert));
        oxs_x509_cert_set_issuer(oxs_cert, env, openssl_x509_get_info(env, OPENSSL_X509_INFO_ISSUER ,cert));
        oxs_x509_cert_set_subject(oxs_cert, env, openssl_x509_get_info(env, OPENSSL_X509_INFO_SUBJECT ,cert));
        oxs_x509_cert_set_fingerprint(oxs_cert, env, openssl_x509_get_info(env, OPENSSL_X509_INFO_FINGER,cert));
        serial_num = openssl_x509_get_serial(env, cert);
        sprintf(serial, "%d" ,serial_num);
        oxs_x509_cert_set_serial_number(oxs_cert, env, serial);
        /*TODO Subject hash*/ 


    }

    return AXIS2_SUCCESS;
}
