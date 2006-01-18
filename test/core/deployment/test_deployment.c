#include <stdio.h>
#include <axis2_op.h>
#include <axis2_module_desc.h>
#include <axis2_phases_info.h>
#include <stdio.h>
#include <axis2_op.h>
#include <axis2_dep_engine.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_class_loader.h>

int axis2_test_dep_engine_load()
{
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_conf_t *conf = NULL;
    axis2_hash_t *svc_map = NULL;
    axis2_array_list_t *in_phases = NULL;
    axis2_char_t *axis2c_home = NULL;

	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);

    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dep_engine = axis2_dep_engine_create_with_repos_name(&env, 
        axis2c_home);
    if(!dep_engine)
    {
        printf("dep engine is not created \n");
        return -1;
    }
    conf = AXIS2_DEP_ENGINE_LOAD(dep_engine, &env);
    if (!conf)
    {
        printf("conf is NULL\n)");
        return -1;
    }
    
    svc_map = AXIS2_CONF_GET_SVCS(conf, &env);
    if (svc_map)
        printf("svc_map count = %d\n", axis2_hash_count(svc_map));
    else
        printf("svc_map count = zero\n");
    
    in_phases = 
        AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(
            conf, &env);
    if(!in_phases)
    {
        printf("in phases up to and including post dispatch is NULL\n");
    }
    else
    {
        printf("dep engine load is successfull\n");
    }

    return 0;
}

int axis2_test_dep_engine_do_deploy()
{
    return 0;
}

int axis2_test_engine_conf_builder_populate_conf()
{
	axis2_status_t status = AXIS2_FAILURE;
    axis2_conf_t *conf = NULL;
    axis2_conf_builder_t *builder = NULL;
    axis2_char_t *conf_name = NULL;
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_char_t *axis2c_home = NULL;

    printf("testing conf_builder_populate_conf method \n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	conf = axis2_conf_create(&env);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");

    dep_engine = axis2_dep_engine_create_with_repos_name(&env, 
        axis2c_home);

    conf_name = AXIS2_STRACAT (axis2c_home, "/axis2.xml", &env);
    builder = axis2_conf_builder_create_with_file_and_dep_engine_and_conf(&env,
            conf_name, dep_engine, conf);
    if(!builder)
    {
        printf("conf builder is not created \n");
        return -1;
    }
    status = AXIS2_CONF_BUILDER_POPULATE_CONF(builder, &env);   
    if(AXIS2_SUCCESS != status)
    {
        printf("Populate conf is not successfule. Test failed \n");
    }
    else
    {
        printf("conf is populated. Test is a success \n");
    }
    AXIS2_FREE(env->allocator, conf_name);
    return 0; 
}

axis2_test_transport_receiver_load()
{
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_char_t *dll_name = NULL;
    axis2_transport_receiver_t *transport_recv = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_bool_t *is_running = AXIS2_FALSE;
    axis2_char_t *expected = NULL;
    axis2_char_t *axis2c_home = NULL;

    expected = "application";
    printf("testing axis2_transport_recv load\n"); 
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);
    
    dll_desc = axis2_dll_desc_create(&env);
    
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name = AXIS2_STRACAT (axis2c_home, "/lib/libaxis2_http_receiver.so", &env);
    printf("transport receiver name:%s\n", dll_name);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, &env, dll_name);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, &env, AXIS2_TRANSPORT_RECV_DLL);
    axis2_class_loader_init(&env);
    impl_info_param = axis2_param_create(&env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, &env, dll_desc);
    transport_recv = (axis2_transport_receiver_t *) axis2_class_loader_create_dll(&env, 
        impl_info_param);
    is_running = AXIS2_TRANSPORT_RECEIVER_IS_RUNNING(transport_recv, &env);
    printf("is_running:%d\n", is_running);
    AXIS2_FREE(env->allocator, dll_name);
    printf("transport receiver load test successful\n");
    return 0;
}

axis2_test_transport_sender_load()
{
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_char_t *dll_name = NULL;
    axis2_transport_sender_t *transport_sender = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *axis2c_home = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;

    printf("testing axis2_transport_sender load\n"); 
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);
    
    msg_ctx = (axis2_msg_ctx_t *) AXIS2_MALLOC(env->allocator, 5);
    dll_desc = axis2_dll_desc_create(&env);
    
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name = AXIS2_STRACAT (axis2c_home, "/lib/libaxis2_http_sender.so", &env);
    printf("transport sender name:%s\n", dll_name);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, &env, dll_name);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, &env, AXIS2_TRANSPORT_SENDER_DLL);
    axis2_class_loader_init(&env);
    impl_info_param = axis2_param_create(&env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, &env, dll_desc);
    transport_sender = (axis2_transport_sender_t *) axis2_class_loader_create_dll(&env, 
        impl_info_param);
    
    status = AXIS2_TRANSPORT_SENDER_CLEANUP(transport_sender, &env, msg_ctx);
    printf("clean status:%d\n", status);
    AXIS2_FREE(env->allocator, dll_name);
    printf("transport sender load test successful\n");
    return 0;
}

int main()
{
    /*axis2_test_dep_engine_do_deploy();
    axis2_test_engine_conf_builder_populate_conf();*/
    axis2_test_dep_engine_load();
    /*axis2_test_transport_receiver_load();*/
    /*axis2_test_transport_sender_load();*/
    
	return 0;
}
