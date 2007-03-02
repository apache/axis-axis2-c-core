/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <axis2_env.h>
#include <oxs_buffer.h>
#include <oxs_error.h>
#include <oxs_key_mgr.h>
#include <openssl_sign.h>
#include <openssl_digest.h>
#include <openssl_constants.h>
#include <openssl_pkey.h>
#include <oxs_axis2_utils.h>
#include <oxs_x509_cert.h>


int main()
{
    axis2_env_t *env = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    oxs_buffer_t *inbuf = NULL;
    oxs_buffer_t *outbuf = NULL;
    openssl_pkey_t *prvkey = NULL;
    openssl_pkey_t *pubkey = NULL;
    oxs_x509_cert_t *cert = NULL;
    char *data = "Winners in good times and survivors in hard times";
    int len = -1;

    env = axis2_env_create_all("./openssl.log", AXIS2_LOG_LEVEL_TRACE);
    
    /*Load private key*/
    prvkey = oxs_key_mgr_load_private_key_from_file(env, "rsakey.pem", "");
    if(!prvkey){
            printf("Cannot load private key");
    }

    /*Load certificate*/
    cert = oxs_key_mgr_load_x509_cert_from_pem_file(env, "rsacert.pem");
    if(!cert){
         printf("Cannot load certificate");
    }

    inbuf = oxs_buffer_create(env);
    OXS_BUFFER_POPULATE(inbuf, env, (unsigned char *)data, strlen(data));

    outbuf = oxs_buffer_create(env);
    
    /*Sign*/
    printf("Signing\n");
    len = openssl_sig_sign(env, prvkey, inbuf, outbuf);
    if(len < 0 ){
        printf("Signing failed\n");
        return 0;
    }else{
        printf("Signing OK. Sig len = %d\n", len);
    }


    /*Verify*/
    printf("Verifying\n");
    pubkey = oxs_x509_cert_get_public_key(cert, env);
    
    status = openssl_sig_verify(env, pubkey, inbuf, outbuf);
    if(AXIS2_SUCCESS != status){
        printf("Sign verification failed\n");
    }else{
        printf("Sign verification success\n");

    }

    
    return 0;
}
