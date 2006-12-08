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
#include <oxs_utility.h>
#include <oxs_error.h>
#include <oxs_asym_ctx.h>

/* Generates an id for an element.
 * Specially used in xml encryption and signature references.
 * */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_util_generate_id(const axis2_env_t *env,
        axis2_char_t *prefix)
{
    axis2_char_t *id = NULL;
    char _id[50];
    axis2_char_t *random ;

    random =  AXIS2_STRNDUP(axis2_uuid_gen(env),18, env);
    sprintf(_id, "%s-%s",prefix,random);
    id = (axis2_char_t*)AXIS2_STRDUP(_id, env);
    return id;
    
}

AXIS2_EXTERN oxs_asym_ctx_format_t AXIS2_CALL
oxs_util_get_format_by_file_extension(const axis2_env_t *env,
        axis2_char_t *file_name)
{
    axis2_char_t *extension = NULL;
    if(!file_name){
        return OXS_ASYM_CTX_FORMAT_UNKNOWN;
    }
    extension = AXIS2_RINDEX(file_name, '.');
    if(!extension){
        /*No extension*/
        /*Its safe to assume that PEM can be without extension*/
        return OXS_ASYM_CTX_FORMAT_PEM;
    }
    printf("ext %s", extension);
    if((strcmp(extension, ".pfx") == 0) ){
        return OXS_ASYM_CTX_FORMAT_PKCS12;
    }else{
        /*Its safe to assume that PEM can be in any extensions. e.g. .cert, .cer, .pem*/
        return OXS_ASYM_CTX_FORMAT_PEM;
    }

}
