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
#include <axiom_node_internal.h>

#include <CuTest.h>

void Testaxiom_node_add_child(CuTest *tc)
{
    axiom_node_t *parent;
    axiom_node_t *child;
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    axiom_node_t *actual;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    parent = axiom_node_create(env);
    child  = axiom_node_create(env);

    AXIOM_NODE_ADD_CHILD(parent, env, child);
    actual = AXIOM_NODE_GET_FIRST_CHILD(parent, env);
    CuAssertPtrEquals(tc, child, actual);
    /*axis2_env_free(env); */
}

void Testaxiom_node_set_parent(CuTest *tc)
{

    axiom_node_t *parent;
    axiom_node_t *child;
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    parent = axiom_node_create(env);
    child  = axiom_node_create(env);

    axiom_node_set_parent(child, env, parent);
    CuAssertPtrEquals(tc, parent, AXIOM_NODE_GET_PARENT(child, env));
    /*axis2_env_free(env);*/
}


void Testaxiom_node_insert_sibling_before(CuTest *tc)
{
    axiom_node_t *parent;
    axiom_node_t *child;
    axiom_node_t *sibling;
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    parent = axiom_node_create(env);
    child  = axiom_node_create(env);
    sibling = axiom_node_create(env);

    AXIOM_NODE_ADD_CHILD(parent,  env, child);
    AXIOM_NODE_INSERT_SIBLING_BEFORE(child, env, sibling);

    CuAssertPtrEquals(tc, sibling, AXIOM_NODE_GET_FIRST_CHILD(parent, env));
    /*axis2_env_free(env);*/
}
void Testaxiom_node_insert_sibling_after(CuTest *tc)
{
    axiom_node_t *parent;
    axiom_node_t *child;
    axiom_node_t *sibling;
    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    parent = axiom_node_create(env);
    child  = axiom_node_create(env);
    sibling = axiom_node_create(env);

    AXIOM_NODE_ADD_CHILD(parent, env, child);
    AXIOM_NODE_INSERT_SIBLING_AFTER(child, env, sibling);

    CuAssertPtrEquals(tc, sibling, AXIOM_NODE_GET_NEXT_SIBLING(child, env));
    /*axis2_env_free(env);*/
}
void Testaxiom_node_detach(CuTest *tc)
{
    axiom_node_t *parent;
    axiom_node_t *child;
    axiom_node_t *sibling1;
    axiom_node_t *sibling2;

    const axis2_env_t *env;
    axis2_allocator_t *allocator;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    parent = axiom_node_create(env);
    child  = axiom_node_create(env);
    sibling1 = axiom_node_create(env);
    sibling2 = axiom_node_create(env);


    AXIOM_NODE_ADD_CHILD(parent, env, child);
    AXIOM_NODE_ADD_CHILD(parent, env, sibling1);
    AXIOM_NODE_ADD_CHILD(parent, env, sibling2);

    CuAssertPtrEquals(tc, sibling1, AXIOM_NODE_GET_NEXT_SIBLING(child, env));

    AXIOM_NODE_DETACH(sibling1, env);

    CuAssertPtrEquals(tc, sibling2, AXIOM_NODE_GET_NEXT_SIBLING(child, env));
    /*axis2_env_free(env);*/
}
