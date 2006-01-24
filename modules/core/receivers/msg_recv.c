/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_msg_recv.h>
#include <axis2_param.h>
#include <axis2_description.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <axis2_class_loader.h>

/** 
 * @brief Message Receiver struct impl
 * Axis2 Message Receiver impl  
 */ 
typedef struct axis2_msg_recv_impl
{
	axis2_msg_recv_t msg_recv;
    axis2_char_t *scope;
} axis2_msg_recv_impl_t;

#define AXIS2_INTF_TO_IMPL(msg_recv) ((axis2_msg_recv_impl_t *) msg_recv)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_msg_recv_free (axis2_msg_recv_t *msg_recv,
                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_msg_recv_receive (axis2_msg_recv_t *msg_recv,
                        axis2_env_t **env,
                        struct axis2_msg_ctx *in_msg_ctx);

axis2_svc_skeleton_t * AXIS2_CALL
axis2_msg_recv_make_new_svc_obj(axis2_msg_recv_t *msg_recv,
                                    axis2_env_t **env,
                                    struct axis2_msg_ctx *msg_ctx);

axis2_svc_skeleton_t * AXIS2_CALL
axis2_msg_recv_get_impl_obj(axis2_msg_recv_t *msg_recv,
                            axis2_env_t **env,
                            struct axis2_msg_ctx *msg_ctx); 

axis2_status_t AXIS2_CALL
axis2_msg_recv_set_scope(axis2_msg_recv_t *msg_recv,
                            axis2_env_t **env,
                            axis2_char_t *scope);
                                   
axis2_char_t * AXIS2_CALL
axis2_msg_recv_get_scope(axis2_msg_recv_t *msg_recv,
                            axis2_env_t **env);                            
		
/************************* End of function headers ****************************/	

axis2_msg_recv_t * AXIS2_CALL
axis2_msg_recv_create (axis2_env_t **env)
{
    axis2_msg_recv_impl_t *msg_recv_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
	msg_recv_impl = (axis2_msg_recv_impl_t *) malloc (sizeof (axis2_msg_recv_impl_t));
    
	if(NULL == msg_recv_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    /*msg_recv_impl->scope = AXIS2_STRDUP(AXIS2_APPLICATION_SCOPE, env);*/
    msg_recv_impl->scope = AXIS2_STRDUP("app*", env);;
    msg_recv_impl->msg_recv.ops = NULL;
    
    msg_recv_impl->msg_recv.ops = (axis2_msg_recv_ops_t *) malloc(sizeof(axis2_msg_recv_ops_t));
    
	if(NULL == msg_recv_impl->msg_recv.ops)
	{
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_msg_recv_free(&(msg_recv_impl->msg_recv), env);
        
        return NULL;
	}
    
	msg_recv_impl->msg_recv.ops->free = axis2_msg_recv_free;
    msg_recv_impl->msg_recv.ops->make_new_svc_obj = 
        axis2_msg_recv_make_new_svc_obj;
    msg_recv_impl->msg_recv.ops->get_impl_obj = axis2_msg_recv_get_impl_obj;
    msg_recv_impl->msg_recv.ops->set_scope = axis2_msg_recv_set_scope;
    msg_recv_impl->msg_recv.ops->get_scope = axis2_msg_recv_get_scope;
						
	return &(msg_recv_impl->msg_recv);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_msg_recv_free (axis2_msg_recv_t *msg_recv,
                        axis2_env_t **env)
{
    axis2_msg_recv_impl_t *recv_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(msg_recv, env, AXIS2_FAILURE);
    recv_impl = AXIS2_INTF_TO_IMPL(msg_recv);
    
    if(recv_impl->scope)
    {
        AXIS2_FREE((*env)->allocator, recv_impl->scope);
        recv_impl->scope = NULL;
    }

    if(NULL != msg_recv->ops)
    {
        AXIS2_FREE((*env)->allocator, msg_recv->ops);
        msg_recv->ops = NULL;
    }

    if(recv_impl)
    {
        AXIS2_FREE((*env)->allocator, recv_impl);
        recv_impl = NULL;
    }

	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_msg_recv_receive (axis2_msg_recv_t *msg_recv,
                        axis2_env_t **env,
                        struct axis2_msg_ctx *in_msg_ctx)
{
	return AXIS2_SUCCESS;
}


axis2_svc_skeleton_t * AXIS2_CALL
axis2_msg_recv_make_new_svc_obj(axis2_msg_recv_t *msg_recv,
                                    axis2_env_t **env,
                                    struct axis2_msg_ctx *msg_ctx)
{
    struct axis2_svc *svc = NULL;
    struct axis2_op_ctx *op_ctx = NULL;
    struct axis2_svc_ctx *svc_ctx = NULL;
    axis2_param_t *impl_info_param = NULL;
    void *impl_class = NULL;

    AXIS2_FUNC_PARAM_CHECK(msg_recv, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, NULL);

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
    svc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
    if(NULL == svc)
    {
        return NULL;
    }

    impl_info_param = AXIS2_SVC_GET_PARAM(svc, env, AXIS2_SERVICE_CLASS);
    if(!impl_info_param)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC,
            AXIS2_FAILURE);
        return NULL;
    }

    axis2_class_loader_init(env);

    impl_class = axis2_class_loader_create_dll(env, impl_info_param);

    return impl_class;
}


axis2_svc_skeleton_t * AXIS2_CALL
axis2_msg_recv_get_impl_obj(axis2_msg_recv_t *msg_recv,
                            axis2_env_t **env,
                            struct axis2_msg_ctx *msg_ctx)
{
    struct axis2_svc *svc = NULL;
    struct axis2_op_ctx *op_ctx = NULL;
    struct axis2_svc_ctx *svc_ctx = NULL;
    struct axis2_param *scope_param = NULL;
    axis2_qname_t *svc_qname = NULL;
    axis2_char_t *param_value = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_recv, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, NULL);
    
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    svc_ctx = AXIS2_OP_CTX_GET_PARENT(op_ctx, env);
    svc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
    if(NULL == svc)
    {
        return NULL;
    }
    scope_param = AXIS2_SVC_GET_PARAM(svc, env, AXIS2_SCOPE);
    svc_qname = AXIS2_SVC_GET_QNAME(svc, env);
    if (scope_param)
        param_value = AXIS2_PARAM_GET_VALUE(scope_param, env);
    /* TODO
     * This part is left until session_ctx is implemented or this will before
     * totally removed???
     */
    /*
    if(NULL != scope_param && (0 == AXIS2_STRCMP(AXIS2_SESSION_SCOPE, 
        param_value)))
    {
        SessionContext sessionContext = msgContext.getSessionContext();
        synchronized (sessionContext) {
            Object obj =
                sessionContext.getProperty(serviceName.getLocalPart());
            if (obj == null) {
                obj = makeNewServiceObject(msgContext);
                sessionContext.setProperty(serviceName.getLocalPart(), obj);
            }
            return obj;
        }
    }
    else if
    */ 
    if(NULL != scope_param && (0 == AXIS2_STRCMP(AXIS2_APPLICATION_SCOPE, 
        param_value)))
    {      
        struct axis2_conf_ctx *global_ctx = NULL;
        void *obj = NULL;
        axis2_char_t *local_part = NULL;
        struct axis2_ctx *ctx = NULL;
        
        global_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
        local_part = AXIS2_QNAME_GET_LOCALPART(svc_qname, env);
        ctx = AXIS2_CONF_CTX_GET_BASE(global_ctx, env);
        obj = AXIS2_CTX_GET_PROPERTY(ctx, env, local_part, AXIS2_FALSE);
        if(NULL == obj)
        {
            obj = axis2_msg_recv_make_new_svc_obj(msg_recv, env, msg_ctx);
            AXIS2_CTX_SET_PROPERTY(ctx, env, local_part, obj, AXIS2_FALSE);
        }
        return obj;
    } 
    else
    {
        return axis2_msg_recv_make_new_svc_obj(msg_recv, env, msg_ctx);
    }

    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_msg_recv_set_scope(axis2_msg_recv_t *msg_recv,
                            axis2_env_t **env,
                            axis2_char_t *scope)
{
    axis2_msg_recv_impl_t *msg_recv_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_recv, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, scope, AXIS2_FAILURE);
    msg_recv_impl = AXIS2_INTF_TO_IMPL(msg_recv);
    
    if(msg_recv_impl->scope)
    {
        AXIS2_FREE((*env)->allocator, msg_recv_impl->scope);
        msg_recv_impl->scope = NULL;
    }
    msg_recv_impl->scope = AXIS2_STRDUP(scope, env);
    if(!msg_recv_impl->scope)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_msg_recv_get_scope(axis2_msg_recv_t *msg_recv,
                            axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(msg_recv)->scope;
}
