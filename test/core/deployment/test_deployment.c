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
#include <axis2_error_default.h>
#include <axis2_log_default.h>
#include <axis2_transport_sender.h>
#include <axis2_transport_receiver.h>

int axis2_test_dep_engine_load()
{
    axis2_dep_engine_t *dep_engine = NULL;
    axis2_conf_t *conf = NULL;
    axis2_hash_t *svc_map = NULL;
    axis2_array_list_t *in_phases = NULL;
    axis2_char_t *axis2c_home = NULL;
    axis2_env_t *env = NULL;

    printf("******************************************\n");
    printf("testing dep_engine_load method \n");
    printf("******************************************\n");
   
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_error_t *error = axis2_error_create(allocator);
    axis2_log_t *log = axis2_log_create(allocator, NULL, "test_deployment.log");
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_INFO;
    
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

    if (svc_map)
    {
        axis2_hash_index_t *hi = NULL;
        void *service = NULL;
        for (hi = axis2_hash_first (svc_map, &env);
             NULL != hi; hi = axis2_hash_next (&env, hi))
        {
             axis2_hash_t *ops= NULL;
             axis2_svc_t *svc = NULL;
             axis2_param_t *impl_info_param = NULL;

             axis2_hash_this(hi, NULL, NULL, &service);
             svc = (axis2_svc_t *) service;
             impl_info_param = AXIS2_SVC_GET_PARAM(svc, &env, AXIS2_SERVICE_CLASS);
             if(!impl_info_param)
             {
                printf("imple_info_param is null\n");
             }
             ops = AXIS2_SVC_GET_OPS(svc, &env);
             if(ops)
             {
                printf("ops count = %d\n", axis2_hash_count(ops));

                axis2_hash_index_t *hi2 = NULL;
                void *op = NULL;
                axis2_char_t *oname = NULL;
                int count = 0;

                for(hi2 = axis2_hash_first(ops, &env); hi2; hi2 = axis2_hash_next(&env, hi2))
                {
                    if (!hi2)
                        break;
                    axis2_hash_this(hi2, NULL, NULL, &op);
                    if (op)
                    {
                        axis2_qname_t *qname = NULL;
                        qname = AXIS2_OP_GET_QNAME((axis2_op_t *)op, &env);
					    oname = AXIS2_QNAME_GET_LOCALPART(qname, &env);
                        printf("op name = %s\n", oname);
                    }
                }
             }
             else
                printf("ops count = zero\n");
        }
    }
   
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
    AXIS2_CONF_FREE(conf, &env);

    return 0;
}

axis2_test_transport_receiver_load()
{
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_char_t *dll_name = NULL;
    axis2_transport_receiver_t *transport_recv = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_bool_t *is_running = AXIS2_FALSE;
    axis2_char_t *axis2c_home = NULL;

    printf("******************************************\n");
    printf("testing axis2_transport_recv load\n"); 
    printf("******************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);
    
    dll_desc = axis2_dll_desc_create(&env);
    
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name = AXIS2_STRACAT (axis2c_home, "/lib/libaxis2_http_receiver.so", &env);
    printf("transport receiver name:%s\n", dll_name);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, &env, dll_name);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, &env, AXIS2_TRANSPORT_RECV_DLL);
    impl_info_param = axis2_param_create(&env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, &env, dll_desc);
    axis2_class_loader_init(&env);
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

    printf("******************************************\n");
    printf("testing axis2_transport_sender load\n"); 
    printf("******************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);
    msg_ctx = (axis2_msg_ctx_t *) AXIS2_MALLOC(env->allocator, 5);
    dll_desc = axis2_dll_desc_create(&env);
    
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name = AXIS2_STRACAT (axis2c_home, "/lib/libaxis2_http_sender.so", &env);
    printf("transport sender name:%s\n", dll_name);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, &env, dll_name);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, &env, AXIS2_TRANSPORT_SENDER_DLL);
    impl_info_param = axis2_param_create(&env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, &env, dll_desc);
    axis2_class_loader_init(&env);
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
    /*axis2_test_transport_receiver_load();
    axis2_test_transport_sender_load();*/
    axis2_test_dep_engine_load();
	return 0;
}
