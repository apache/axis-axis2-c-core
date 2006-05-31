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

#ifndef AXIS2_HANDLER_H
#define AXIS2_HANDLER_H

/**
 * @file axis2_handler.h
 * @brief Axis2 handler interface
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_param.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_handler;
    struct axis2_handler_ops;
    struct axis2_handler_desc;
    struct axis2_msg_ctx;


/**
 * @defgroup axis2_handler Handlers
 * @ingroup axis2_core_handlers
 * @{
 */

/**
 *   \brief Handler ops struct
 */
 AXIS2_DECLARE_DATA   typedef struct axis2_handler_ops
    {
      /**
        * Free an axis2_handler struct
        * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param axis2_handler pointer to axis2_handler struct to be freed
        * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
		free) (struct axis2_handler * handler, 
               const axis2_env_t *env);
         

      /**
        * Method init
        *
        * @param handlerdesc
        */
        axis2_status_t (AXIS2_CALL *
		init) (struct axis2_handler * handler, 
               const axis2_env_t *env, 
               struct axis2_handler_desc *handler_desc);

      /**
        * Invoke is called to do the actual work of the Handler object.
        * If there is a fault during the processing of this method it is
        * invoke's job to report the error and undo any partial work
        * that has been completed. 
        *
        * @param msgContext the <code>axis2_context_message</code> to process with this
        *                   <code>Handler</code>.
        */
        axis2_status_t (AXIS2_CALL *
		invoke) (struct axis2_handler * handler,
				 const axis2_env_t *env,
				 struct axis2_msg_ctx *msg_ctx);

      /**
        * Method getName
        *
        * @return
        */
        axis2_qname_t* (AXIS2_CALL *
		get_name) (struct axis2_handler * handler, 
                   const axis2_env_t *env);

       /**
         * Method getParameter
         *
         * @param name
         * @return
         */
         axis2_param_t* (AXIS2_CALL *
		get_param) (struct axis2_handler * handler, 
                    const axis2_env_t *env, 
                    axis2_char_t *name);

      /**
        * To get the phaseRule of a handler it is required to get the HnadlerDescription of the handler
        * so the argumnet pass when it call return as HnadlerDescription
        *
        * @return
        */
        struct axis2_handler_desc* (AXIS2_CALL *
		get_handler_desc) (struct axis2_handler * handler, 
                           const axis2_env_t *env);
      /**
        * derived struct accessor
        */        
        /*void* (AXIS2_CALL *get_derived)(struct axis2_handler * handler, 
                                                          const axis2_env_t *env);
        */
      /**
        * derived struct mutator
        */        
        /*axis2_status_t (AXIS2_CALL *set_derived)(struct axis2_handler * handler, 
                                                          const axis2_env_t *env, void* derived);
        */
    } axis2_handler_ops_t;
	
   /** 
    * \brief Handler struct
    */
    typedef struct axis2_handler
    {
        /** Handler related ops */
        axis2_handler_ops_t *ops;
    } axis2_handler_t;

    typedef axis2_handler_t *(AXIS2_CALL * 
    AXIS2_HANDLER_CREATE_FUNC) (const axis2_env_t *env, 
                                axis2_qname_t *qname);
    
/**
 * creates handler struct
 */
AXIS2_DECLARE(axis2_handler_t*) 
axis2_handler_create(const axis2_env_t *env);

AXIS2_DECLARE(axis2_handler_t*) 
axis2_ctx_handler_create(const axis2_env_t *env, axis2_qname_t *qname);
    
#define AXIS2_HANDLER_FREE(handler, env) \
	    ((handler)->ops->free(handler, env))
	
	
#define AXIS2_HANDLER_INIT(handler, env, handler_desc) \
	    ((handler)->ops->init(handler, env, handler_desc))
	
#define AXIS2_HANDLER_INVOKE(handler, env, msg_ctx) \
        ((handler)->ops->invoke(handler, env, msg_ctx))
		
#define AXIS2_HANDLER_SET_INVOKE(handler, env, invoke) \
        ((handler)->ops->set_invoke(handler, env, invoke))
		
#define AXIS2_HANDLER_GET_NAME(handler, env) \
        ((handler)->ops->get_name(handler, env))
		
#define AXIS2_HANDLER_GET_PARAM(handler, env, name) \
		((handler)->ops->get_param(handler, env, name))
		
#define AXIS2_HANDLER_GET_HANDLER_DESC(handler, env) \
		((handler)->ops->get_handler_desc(handler, env))
		
/*#define AXIS2_HANDLER_GET_DERIVED(handler, env) ((handler)->ops->get_derived(handler, env))
#define AXIS2_HANDLER_SET_DERIVED(handler, env, derived) ((handler)->ops->set_derived(handler, env, derived))
    */
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_HANDLER_H */
