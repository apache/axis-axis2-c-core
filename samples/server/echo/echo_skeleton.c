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
#include <axis2_svc_skeleton.h>
#include "echo.h"
#include <axutil_array_list.h>
#include <axis2_msg_ctx.h>
#include <stdio.h>
#include <dlfcn.h>

int AXIS2_CALL

 my_init(axis2_svc_skeleton_t *svc_skeleton,

                         const axutil_env_t *env)

 {

   /*SpsErrorLog("\nSPSAS: Entered my_init()\n");*/

   fflush(stdout);

     svc_skeleton->func_array = axutil_array_list_create(env, 0);

     axutil_array_list_add(svc_skeleton->func_array, env, (void*)"create");

     axutil_array_list_add(svc_skeleton->func_array, env, (void*)"delete");

     axutil_array_list_add(svc_skeleton->func_array, env, (void*)"update");



     /* Any initialization stuff goes here */

     return AXIS2_SUCCESS;

 }



 int AXIS2_CALL

 my_free(axis2_svc_skeleton_t *svc_skeleton,

             const axutil_env_t *env)

 {

/*     TraceLog("\n[AG-DEBUG] entered my_free\n");*/

     fflush(stdout);

     

     if(svc_skeleton)

     {

         AXIS2_FREE((env)->allocator, svc_skeleton);

         svc_skeleton = NULL;

     }

     return AXIS2_SUCCESS;

 }



  axiom_node_t* AXIS2_CALL

 acc_invoke(axis2_svc_skeleton_t *svc_skeleton,

             const axutil_env_t *env,

             axiom_node_t *node,

             axis2_msg_ctx_t *msg_ctx)

 {

    fflush(stdout);

     /* Depending on the function name invoke the

      *  corresponding method

      */

     if (node)

     {

         if (axiom_node_get_node_type(node, env) == AXIOM_ELEMENT)

         {

             axiom_element_t *element = NULL;

             element = (axiom_element_t *)axiom_node_get_data_element(node, env);

             if (element)

             {

                 axis2_char_t *op_name = axiom_element_get_localname(element, env);

                 if (op_name)

                 {

                     /*if ( axutil_strcmp(op_name, "create") == 0 )

                         return axis2_my_create1(env, node);

                     if ( axutil_strcmp(op_name, "delete") == 0 )

                         return axis2_my_delete1(env, node);

                     if ( axutil_strcmp(op_name, "modify") == 0 )

                         return axis2_my_modify1(env, node);

                     if ( axutil_strcmp(op_name, "readList) == 0 )

                         return axis2_my_readList(env, node);*/

                 }

             }

         }

     }

   else

   {

/*       ErrorLog("ERROR: invalid OM parameters in request\n");*/

   }



     /** TODO: return a SOAP fault here */

     return node;

 }



    static const axis2_svc_skeleton_ops_t my_svc_skeleton_ops_var = {

                     my_init,

                     acc_invoke,

                     NULL,

                     my_free

     };



 axis2_svc_skeleton_t *

 axis2_my_create(const axutil_env_t *env)

 {



     axis2_svc_skeleton_t *svc_skeleton = NULL;

     svc_skeleton = (axis2_svc_skeleton_t*) AXIS2_MALLOC((env)->allocator,

         sizeof(axis2_svc_skeleton_t));





     svc_skeleton->ops = (axis2_svc_skeleton_ops_t*)AXIS2_MALLOC(

         (env)->allocator, sizeof(axis2_svc_skeleton_ops_t));



     svc_skeleton->func_array = NULL;

     svc_skeleton->ops = &my_svc_skeleton_ops_var;



     return svc_skeleton;

 }



  AXIS2_EXPORT int axis2_get_instance(struct axis2_svc_skeleton **inst,

                         const axutil_env_t *env)

 {

/*   SpsTraceLog("\nCalling dlopen() inside axis2_get_instance\n");*/

   void* ptr = dlopen("/root/lib/libMy.so",RTLD_LAZY|RTLD_GLOBAL);

   if(NULL == ptr)

   {

/*     SpsErrorLog("\ndlopen returned NULL");

     SpsErrorLog("\nFor dlopen()...........dlerror says the following\n%s\n",dlerror());*/

   }

/*   SpsTraceLog("\ndlopen returned ptr with addr %u",ptr);*/





   *inst = axis2_my_create(env);

     if(NULL == (*inst))

     {

         return AXIS2_FAILURE;

     }



     return AXIS2_SUCCESS;

 }



 AXIS2_EXPORT int axis2_remove_instance(axis2_svc_skeleton_t *inst,

                             const axutil_env_t *env)

 {

     axis2_status_t status = AXIS2_FAILURE;

   if (inst)

   {

         status = AXIS2_SVC_SKELETON_FREE(inst, env);

     }

     return status;

 }


