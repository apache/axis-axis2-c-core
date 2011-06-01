/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <axutil_base64.h>
#include <axutil_string.h>
#include <axis2_util.h>

#include <stdio.h>
#include <axis2_ntlm.h>


int main()
{
    axis2_status_t status = AXIS2_FAILURE;
    const axutil_env_t *env = NULL;
    env = axutil_env_create_all("ntlm_test_client.log", AXIS2_LOG_LEVEL_TRACE);
    /*int flags = NTLM_NEG_UNICODE | NTLM_NEG_NTLM | NTLM_TARGET_DOMAIN;*/

    {
        axis2_char_t *encoded = NULL;
        int elen = 0;
        int flags = 0;
        axis2_ntlm_t *ntlm = NULL;
        ntlm = axis2_ntlm_create(env);
        status = axis2_ntlm_auth_create_type1_message(ntlm, env, &encoded, &elen, "foo", 
                "digestpassword", flags, "mydomain");
        if(AXIS2_SUCCESS != status)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "axis2_ntlm_auth_create_type1_message call failed");
            printf("axis2_ntlm_auth_create_type1_message call failed\n");
            return 1;
        }
        printf("encoded:***%s***\n", encoded);
        printf("elen:***%d***\n", elen);
    }
    {
        int elen = 0;
        axis2_char_t *encoded = NULL;
        axis2_char_t *header_value = 
            "TlRMTVNTUAACAAAAHgAeADgAAAAFAoICZAg+4lbcT0AAAAAAAAAAAJgAmABWAAAABgGxHQAAAA9XAEkATgAtADcAMQBNAEUAUABIADQANgBOAFEAMwACAB4AVwBJAE4ALQA3ADEATQBFAFAASAA0ADYATgBRADMAAQAeAFcASQBOAC0ANwAxAE0ARQBQAEgANAA2AE4AUQAzAAQAHgBXAEkATgAtADcAMQBNAEUAUABIADQANgBOAFEAMwADAB4AVwBJAE4ALQA3ADEATQBFAFAASAA0ADYATgBRADMABwAIAHIX52u6HcwBAAAAAA==";
        axis2_ntlm_t *ntlm = NULL;
        ntlm = axis2_ntlm_create(env);
        status = axis2_ntlm_auth_create_type3_message(ntlm, env, header_value, &encoded, 
                &elen, "nandika", "nandika", "mydomain", "workstation");
        if(AXIS2_SUCCESS != status)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "axis2_ntlm_auth_create_type3_message call failed");
            printf("axis2_ntlm_auth_create_type3_message call failed\n");
            return 1;
        }
        printf("encoded:***%s***\n", encoded);
        printf("elen:***%d***\n", elen);
    }
    
    return 0;
}

