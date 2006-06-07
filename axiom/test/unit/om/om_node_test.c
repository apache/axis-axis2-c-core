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

#include "om_node_test.h"
#include <axis2_om_node_internal.h>

#include <CuTest.h>

void Testaxis2_om_node_add_child(CuTest *tc)
{
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_om_node_t *actual;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(env);   
    child  = axis2_om_node_create(env); 
    
    AXIS2_OM_NODE_ADD_CHILD(parent, env, child);
    actual = AXIS2_OM_NODE_GET_FIRST_CHILD(parent, env);
    CuAssertPtrEquals(tc, child, actual);  
    /*axis2_env_free(env); */
}

void Testaxis2_om_node_set_parent(CuTest *tc)
{
    
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(env);   
    child  = axis2_om_node_create(env); 
    
    axis2_om_node_set_parent(child, env, parent);
    CuAssertPtrEquals(tc, parent, AXIS2_OM_NODE_GET_PARENT(child, env));
    /*axis2_env_free(env);*/
}

    
void Testaxis2_om_node_insert_sibling_before(CuTest *tc)
{
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    axis2_om_node_t *sibling;
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(env);   
    child  = axis2_om_node_create(env);
    sibling = axis2_om_node_create(env);

    AXIS2_OM_NODE_ADD_CHILD(parent,  env, child);
    AXIS2_OM_NODE_INSERT_SIBLING_BEFORE(child, env, sibling);
    
    CuAssertPtrEquals(tc, sibling,AXIS2_OM_NODE_GET_FIRST_CHILD(parent, env));    
    /*axis2_env_free(env);*/
}
void Testaxis2_om_node_insert_sibling_after(CuTest *tc)
{
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    axis2_om_node_t *sibling;
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(env);   
    child  = axis2_om_node_create(env);
    sibling = axis2_om_node_create(env);

    AXIS2_OM_NODE_ADD_CHILD(parent, env, child);
    AXIS2_OM_NODE_INSERT_SIBLING_AFTER(child, env, sibling);
    
    CuAssertPtrEquals(tc, sibling,AXIS2_OM_NODE_GET_NEXT_SIBLING(child, env));
    /*axis2_env_free(env);*/
}
void Testaxis2_om_node_detach(CuTest *tc)
{
    axis2_om_node_t *parent;
    axis2_om_node_t *child;
    axis2_om_node_t *sibling1;
    axis2_om_node_t *sibling2;
    
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    parent = axis2_om_node_create(env);   
    child  = axis2_om_node_create(env);
    sibling1 = axis2_om_node_create(env);
    sibling2 = axis2_om_node_create(env);
    
    
    AXIS2_OM_NODE_ADD_CHILD(parent, env, child);
    AXIS2_OM_NODE_ADD_CHILD(parent, env, sibling1);
    AXIS2_OM_NODE_ADD_CHILD(parent, env, sibling2);

    CuAssertPtrEquals(tc, sibling1, AXIS2_OM_NODE_GET_NEXT_SIBLING(child, env));

    AXIS2_OM_NODE_DETACH(sibling1, env);
    
    CuAssertPtrEquals(tc, sibling2, AXIS2_OM_NODE_GET_NEXT_SIBLING(child, env));
    /*axis2_env_free(env);*/
}
