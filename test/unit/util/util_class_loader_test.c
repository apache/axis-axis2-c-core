#include "util_class_loader_test.h"
#include <axis2_dll_desc.h>
#include <axis2_msg_recv.h>
#include <axis2_param.h>
#include <axis2_class_loader.h>
/*
axis2_status_t AXIS2_CALL
axis2_test_class_loader_load_lib (axis2_env_t **env,
                                axis2_dll_desc_t *dll_desc)
{
    axis2_char_t *dll_name = NULL;
    AXIS2_DLHANDLER dl_handler = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    dll_name = "/home/damitha/programs/axis2c/lib/libaxis2_receivers.so";
    printf("dll_name:%s\n", dll_name);
    dl_handler = AXIS2_PLATFORM_LOADLIB(dll_name);
    if(NULL == dl_handler)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_LOADING_FAILED, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status = AXIS2_DLL_DESC_SET_DL_HANDLER(dll_desc, env, dl_handler);

    if(AXIS2_FAILURE == status)
    {
        AXIS2_FREE((*env)->allocator, dl_handler);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_LOADING_FAILED, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}
*/
/*
void *
axis2_test_class_loader_create_dll (axis2_env_t **env,
                                axis2_param_t *impl_info_param)
{
    axis2_msg_recv_t *msg_recv = NULL; 
    void *handler = NULL;
    void *module = NULL;
    CREATE_FUNCT create_funct = NULL;
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_dll_type_t dll_type = AXIS2_SVC_DLL;
    
    dll_desc = AXIS2_PARAM_GET_VALUE(impl_info_param, env);
    if(!dll_desc)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
            AXIS2_FAILURE);
        return NULL;
    }
    status = axis2_class_loader_load_lib(env, dll_desc);
    printf("load lib status:%d\n", status);
    if (AXIS2_SUCCESS == status)
    {
        CREATE_FUNCT create_funct = NULL;
        DELETE_FUNCT delete_funct = NULL;
        AXIS2_DLHANDLER dl_handler = NULL;
        
        dl_handler = AXIS2_DLL_DESC_GET_DL_HANDLER(dll_desc, env);
        create_funct = (CREATE_FUNCT) AXIS2_PLATFORM_GETPROCADDR(dl_handler,
            AXIS2_CREATE_FUNCTION);
        status = AXIS2_DLL_DESC_SET_CREATE_FUNCT(dll_desc, env, create_funct); 
        if(AXIS2_FAILURE == status)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
                AXIS2_FAILURE);
            return NULL;
        }
            
        delete_funct = (DELETE_FUNCT) AXIS2_PLATFORM_GETPROCADDR(dl_handler,
            AXIS2_DELETE_FUNCTION);

        status = AXIS2_DLL_DESC_SET_DELETE_FUNCT(dll_desc, env, delete_funct);
        printf("status of setting delete:%d\n", status);        
        if(AXIS2_FAILURE == status)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
                AXIS2_FAILURE);
            return NULL;
        }
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
                AXIS2_FAILURE);
        return NULL;
    }
    
    create_funct = AXIS2_DLL_DESC_GET_CREATE_FUNCT(dll_desc, env);
    
    dll_type = AXIS2_DLL_DESC_GET_TYPE(dll_desc, env);
    if(AXIS2_SVC_DLL == dll_type)
    {
        create_funct(&svc_skeli, env);
        if(NULL == svc_skeli)
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_SVC_SKELETON_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        return svc_skeli;
    }
    if(AXIS2_HANDLER_DLL == dll_type)
    {
        create_funct(&handler, env);
        if(NULL == handler)
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_HANDLER_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        return handler;
    }
    if(AXIS2_MODULE_DLL == dll_type)
    {
        create_funct(&module, env);
        if(NULL == module)
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        return module;
    }
    if(AXIS2_MSG_RECV_DLL == dll_type)
    {
        axis2_char_t *result = NULL;
        printf("came6**\n");
        create_funct(&msg_recv, env);
        printf("came7\n");
        if(NULL == msg_recv)
        {
            printf("msg_recv is null\n");
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MSG_RECV_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        result = AXIS2_MSG_RECV_GET_SCOPE(msg_recv, env);
        printf("result:%s\n", result);
        return msg_recv;
    }

    return NULL;    
}
*/
/*
axis2_status_t AXIS2_CALL
axis2_test_class_loader_init(axis2_env_t **env)
{
    AXIS2_PLATFORM_LOADLIBINIT();
    return AXIS2_SUCCESS;
}
*/
void Testaxis2_class_loader_create_dll(CuTest *tc)
{
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_char_t *dll_name = NULL;
    axis2_msg_recv_t *msg_recv = NULL;
    axis2_param_t *impl_info_param = NULL;
    axis2_char_t *scope = NULL;
    axis2_char_t *expected = NULL;
    axis2_char_t *axis2c_home = NULL;

    expected = "application";
    printf("testing axis2_class_loader_create\n"); 
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);
    
    dll_desc = axis2_dll_desc_create(&env);
    
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    dll_name = AXIS2_STRACAT (axis2c_home, "/lib/libaxis2_receivers.so", &env);

    AXIS2_DLL_DESC_SET_NAME(dll_desc, &env, dll_name);
    AXIS2_DLL_DESC_SET_TYPE(dll_desc, &env, AXIS2_MSG_RECV_DLL);
    axis2_class_loader_init(&env);
    impl_info_param = axis2_param_create(&env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, &env, dll_desc);
    msg_recv = (axis2_msg_recv_t *) axis2_class_loader_create_dll(&env, 
        impl_info_param);
    scope = AXIS2_MSG_RECV_GET_SCOPE(msg_recv, &env);
    printf("scope:%s\n", scope);
    AXIS2_FREE(env->allocator, dll_name);
    CuAssertStrEquals(tc, expected, scope);
}
