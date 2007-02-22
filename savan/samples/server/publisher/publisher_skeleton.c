/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
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
#include <axis2_svc_skeleton.h>
#include <axis2_array_list.h>
#include <axiom_element.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_header.h>
#include <axiom_soap_body.h>
#include <axiom_soap_fault.h>
#include <axis2_engine.h>
#include <axis2_conf_ctx.h>
#include <axis2_conf_init.h>
#include <axis2_thread_pool.h>
#include <axis2_util.h>
#include <axis2_svc_client.h>
#include <axis2_options.h>
#include <axis2_op.h>
#include <axis2_qname.h>

#include <savan.h>

#include "publisher.h"

typedef struct publisher_data
{
    axis2_env_t *env;
    axis2_svc_t *svc;
    axis2_conf_ctx_t *conf_ctx;
}publisher_data_t;

int AXIS2_CALL
publisher_free(axis2_svc_skeleton_t *svc_skeleton,
            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
publisher_free_void_arg(void *svc_skeleton,
                    const axis2_env_t *env);

/*
 * This method invokes the right service method 
 */
axiom_node_t* AXIS2_CALL 
publisher_invoke(axis2_svc_skeleton_t *svc_skeleton,
            const axis2_env_t *env,
            axiom_node_t *node,
            axis2_msg_ctx_t *msg_ctx);
            

int AXIS2_CALL 
publisher_init(axis2_svc_skeleton_t *svc_skeleton,
          const axis2_env_t *env);

axiom_node_t* AXIS2_CALL
publisher_on_fault(axis2_svc_skeleton_t *svc_skeli, 
              const axis2_env_t *env, axiom_node_t *node);

static void * AXIS2_THREAD_FUNC
publisher_worker_func(
    axis2_thread_t *thrd,
    void* data);
    
/*Create function */
axis2_svc_skeleton_t *
axis2_publisher_create(const axis2_env_t *env)
{

	axis2_svc_skeleton_t *svc_skeleton = NULL;

    printf("publisher create called.\n");

    
    
    /* Allocate memory for the structs */
    svc_skeleton = AXIS2_MALLOC(env->allocator, 
        sizeof(axis2_svc_skeleton_t));

    svc_skeleton->ops = AXIS2_MALLOC(
        env->allocator, sizeof(axis2_svc_skeleton_ops_t));

    svc_skeleton->func_array = NULL;

    /* Assign function pointers */
    svc_skeleton->ops->free = publisher_free;
    svc_skeleton->ops->init = publisher_init;
    svc_skeleton->ops->invoke = publisher_invoke;
    svc_skeleton->ops->on_fault = publisher_on_fault;

    return svc_skeleton;
}

/* Initialize the service */
int AXIS2_CALL
publisher_init(axis2_svc_skeleton_t *svc_skeleton,
                        const axis2_env_t *env)
{
    svc_skeleton->func_array = axis2_array_list_create(env, 0);

    /* Add the implemented operation names of the service to  
     * the array list of functions 
     */

    AXIS2_ARRAY_LIST_ADD(svc_skeleton->func_array, env, "start");

    /* Any initialization stuff of service should go here */

    return AXIS2_SUCCESS;
}

/*
 * This method invokes the right service method 
 */
axiom_node_t* AXIS2_CALL
publisher_invoke(axis2_svc_skeleton_t *svc_skeleton,
            const axis2_env_t *env,
            axiom_node_t *node,
            axis2_msg_ctx_t *msg_ctx)
{

	axis2_thread_t *worker_thread = NULL;
	publisher_data_t *data = NULL;

    printf("publisher invoke called.\n");

    /* Invoke the business logic.
     * Depending on the function name invoke the correct impl method.
     */

    data = AXIS2_MALLOC(env->allocator, sizeof(publisher_data_t));
    data->env = (axis2_env_t*)env;
    data->svc = AXIS2_MSG_CTX_GET_SVC(msg_ctx, env);
    data->conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
    
    worker_thread = AXIS2_THREAD_POOL_GET_THREAD(env->thread_pool,
        publisher_worker_func, (void*)data);
    if(NULL == worker_thread)
    {
        printf("failed to create thread");
        return AXIS2_FAILURE;
    }
    AXIS2_THREAD_POOL_THREAD_DETACH(env->thread_pool, worker_thread);
    
    return axis2_publisher_start(env, node);
}

/* On fault, handle the fault */
axiom_node_t* AXIS2_CALL
publisher_on_fault(axis2_svc_skeleton_t *svc_skeli, 
              const axis2_env_t *env, axiom_node_t *node)
{
   /* Here we are just setting a simple error message inside an element 
    * called 'EchoServiceError' 
    */
    axiom_node_t *error_node = NULL;
    axiom_node_t* text_node = NULL;
    axiom_element_t *error_ele = NULL;
    error_ele = axiom_element_create(env, node, "TestServiceError", NULL, 
        &error_node);
    AXIOM_ELEMENT_SET_TEXT(error_ele, env, "Test service failed ", 
        text_node);
    return error_node;
}

/* Free the resources used */
int AXIS2_CALL
publisher_free(axis2_svc_skeleton_t *svc_skeleton,
            const axis2_env_t *env)
{
    printf("free called.\n");

    /* Free the function array */
    if(svc_skeleton->func_array)
    {
        AXIS2_ARRAY_LIST_FREE(svc_skeleton->func_array, env);
        svc_skeleton->func_array = NULL;
    }
    
    /* Free the function array */
    if(svc_skeleton->ops)
    {
        AXIS2_FREE(env->allocator, svc_skeleton->ops);
        svc_skeleton->ops = NULL;
    }
    
    /* Free the service skeleton */
    if(svc_skeleton)
    {
        AXIS2_FREE(env->allocator, svc_skeleton);
        svc_skeleton = NULL;
    }

    return AXIS2_SUCCESS; 
}


static void * AXIS2_THREAD_FUNC
publisher_worker_func(
    axis2_thread_t *thrd,
    void* data)
{
    axis2_env_t *main_env = NULL;
    axis2_env_t *env = NULL;
    axiom_namespace_t *test_ns = NULL;
    axiom_node_t *test_node = NULL;
    axiom_element_t* test_elem = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_svc_t *svc = NULL;
    savan_publishing_client_t *pub_client = NULL;
    
    publisher_data_t *mydata = (publisher_data_t*)data;
    main_env = mydata->env;
    conf_ctx = mydata->conf_ctx;
    svc = mydata->svc;
    
    env = axis2_init_thread_env(main_env);

    printf("inside thread worker func...\n");
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "inside thread worker func...");

    /* Build a payload and pass it to the savan publishing client */ 
    test_ns = axiom_namespace_create (env, "http://www.wso2.com/savan/c/publisher", "test");
    test_elem = axiom_element_create(env, NULL, "notify", test_ns, &test_node);
    
    AXIOM_ELEMENT_SET_TEXT(test_elem, env, "test data", test_node);

    pub_client = savan_publishing_client_create(env, conf_ctx, svc);
    
    while(1)
    {
        savan_publishing_client_publish(pub_client, env, test_node);

        AXIS2_SLEEP(10);
        
        printf("Returned from sleep\n");
    }
}

/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int 
axis2_get_instance(axis2_svc_skeleton_t **inst,
                   const axis2_env_t *env)
{
    printf("get instance called.\n");
   *inst = axis2_publisher_create(env);
    if(!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int 
axis2_remove_instance(axis2_svc_skeleton_t *inst,
                      const axis2_env_t *env)
{
	axis2_status_t status = AXIS2_FAILURE;

    printf("remove instance called.\n");
    
   if (inst)
   {
        status = AXIS2_SVC_SKELETON_FREE(inst, env);
    }
    return status;
}
