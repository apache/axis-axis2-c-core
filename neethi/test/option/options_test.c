#include "../../../util/test/util/create_env.h"
#include <neethi_options.h>

axutil_env_t *env = NULL;

/** @brief test options 
 * build a om from a client options
 */

axis2_status_t test_options(axutil_env_t *env)
{  
    neethi_options_t * options = NULL;
    axis2_bool_t include_timestamp;
    axis2_bool_t is_username_token;
    axis2_bool_t token_protection;
    axis2_bool_t signature_protection;
    axis2_bool_t encrypt_before_sign;
    axis2_bool_t encrypt_body;
    axis2_bool_t sign_body;
    axis2_bool_t server_side;
    axis2_char_t * algorithmsuite;
    axis2_char_t * keyidentifier ;
    axiom_node_t * root_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    options = neethi_options_create(env);   
    neethi_options_set_include_timestamp(options,env,include_timestamp);
    include_timestamp = neethi_options_get_include_timestamp(options,env);
    is_username_token = neethi_options_get_is_username_token(options,env);
    neethi_options_set_is_username_token(options,env,is_username_token);
    token_protection = neethi_options_get_token_protection(options,env);
    neethi_options_set_token_protection(options,env,token_protection);
    signature_protection = neethi_options_get_signature_protection(options,env);
    neethi_options_set_signature_protection(options,env,signature_protection);
    encrypt_before_sign = neethi_options_get_encrypt_before_sign(options,env);
    neethi_options_set_encrypt_before_sign(options,env,encrypt_before_sign);
    sign_body = neethi_options_get_sign_body(options,env);
    neethi_options_set_sign_body(options,env,sign_body);
    encrypt_body = neethi_options_get_encrypt_body(options,env);
    neethi_options_set_encrypt_body(options,env,encrypt_body);
    server_side = neethi_options_get_server_side(options,env);
    neethi_options_set_server_side(options,env,server_side);
    algorithmsuite = neethi_options_get_algorithmsuite(options,env);
    printf("The algorithmsuite is %s\n",algorithmsuite);
    neethi_options_set_algorithmsuite(options,env,algorithmsuite);
    keyidentifier = neethi_options_get_keyidentifier(options,env);
    neethi_options_set_keyidentifier(options,env,keyidentifier);
    root_node = neethi_options_get_root_node(options,env);
    printf ("%s\n", axiom_node_to_string (root_node, env));
    status = neethi_options_create_asym_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("neethi options create with axiom node is success\n");
     neethi_options_create_policy_node(env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("neethi options create with policy node is success\n");
    status = create_initiator_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("create initiator  is success\n");
    status = create_recipient_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("create recipient node is success\n");
    status = create_algo_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("create algo node  is success\n");
    status = create_layout_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("create layout node is success\n");
    status = create_username_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("create username  is success\n");
    status = create_wss10_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("create wss10 node is success\n");
    status = neethi_options_create_signed_parts_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("neethi option create with signed parts is success\n");
    status = neethi_options_create_encrypted_parts_node(options,env,root_node);
    if (status == AXIS2_SUCCESS)
        printf("neethi option create with encrypted parts is success\n");
    neethi_options_free(options,env);
    return AXIS2_SUCCESS;
}
int main()
{
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_options(env);
    if(status == AXIS2_FAILURE)
    {
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}



