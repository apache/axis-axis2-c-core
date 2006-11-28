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
    openssl_pkey_t *open_prvkey = NULL;
    openssl_pkey_t *open_pubkey = NULL;
    oxs_x509_cert_t *oxs_cert = NULL;

    X509 *cert = NULL;
    STACK_OF(X509) *ca = NULL;
    EVP_PKEY *prvkey = NULL;
    EVP_PKEY *pubkey = NULL;

    /*Get file to be loaded. Can be either in PEM or PKCS12 format*/
    filename = oxs_asym_ctx_get_file_name(ctx, env);
    if(!filename){
        return AXIS2_FAILURE;
    }

    if(OXS_ASYM_CTX_FORMAT_PEM == oxs_asym_ctx_get_format(ctx, env)){
        format = OPENSSL_X509_FORMAT_PEM;

    }else if(OXS_ASYM_CTX_FORMAT_PKCS12 == oxs_asym_ctx_get_format(ctx, env)){
        format = OPENSSL_X509_FORMAT_PKCS12;
        status = openssl_x509_load_from_pkcs12(env, filename, password, &cert, &prvkey, &ca);
 
    }
   
    
    /*Alright if the prvkey is available, populate the openssl_pkey*/
    if(prvkey){
        open_prvkey = openssl_pkey_create(env);
        OPENSSL_PKEY_POPULATE(open_prvkey, env, prvkey, filename, OPENSSL_PKEY_TYPE_PRIVATE_KEY);
        oxs_asym_ctx_set_private_key(ctx, env, open_prvkey);
    }
    /*If the X509 certificate is available, populate oxs_x509_cert*/
    if(cert){

        /*Create certificate*/
        oxs_cert = oxs_x509_cert_create(env);
        /*And populate it*/
        oxs_x509_cert_set_data(oxs_cert, env, openssl_x509_get_cert_data(env, cert));
        oxs_x509_cert_set_date(oxs_cert, env, openssl_x509_get_info(env, OPENSSL_X509_INFO_VALID_TO ,cert));
        oxs_x509_cert_set_issuer(oxs_cert, env, openssl_x509_get_info(env, OPENSSL_X509_INFO_ISSUER ,cert));
        oxs_x509_cert_set_subject(oxs_cert, env, openssl_x509_get_info(env, OPENSSL_X509_INFO_SUBJECT ,cert));
        oxs_x509_cert_set_fingerprint(oxs_cert, env, openssl_x509_get_info(env, OPENSSL_X509_INFO_FINGER,cert));
        oxs_x509_cert_set_serial_number(oxs_cert, env, openssl_x509_get_serial(env, cert));
        /*TODO Subject hash*/ 

        /*Additionally we need to set the public key*/
        openssl_x509_get_pubkey(env, cert, &pubkey);
        open_pubkey = openssl_pkey_create(env);
        OPENSSL_PKEY_POPULATE(open_pubkey, env, pubkey, openssl_x509_get_info(env, OPENSSL_X509_INFO_FINGER,cert), OPENSSL_PKEY_TYPE_PUBLIC_KEY); 
        /*Set the public key to the x509 certificate*/
        oxs_x509_cert_set_public_key(oxs_cert, env, open_pubkey);
        /*Set the x509 certificate to the asym ctx*/
        oxs_asym_ctx_set_certificate(ctx, env, oxs_cert);
    }

    return AXIS2_SUCCESS;
}
