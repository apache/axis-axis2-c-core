
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


#include <stdio.h>
#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_utils_defines.h>
#include <axis2_utils.h>
#include <axis2_env.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axiom_element.h>
#include <axiom_children_iterator.h>
#include <axiom_document.h>
#include <axiom_comment.h>
#include <oxs_c14n.h>
#include "c14n_sorted_list.h"

#define N_C14N_DEBUG

#define DEFAULT_STACK_SIZE 16 

#define c14n_ns_stack_push(save_stack, ctx) \
        (save_stack)->head = (ctx)->ns_stack->head; \
        (save_stack)->def_ns = (ctx)->ns_stack->def_ns;

#define c14n_ns_stack_pop(saved_stack, ctx) \
    (ctx)->ns_stack->head = (saved_stack)->head; \
    (ctx)->ns_stack->def_ns = (saved_stack)->def_ns;

#define c14n_ns_stack_set_default(ns, ctx) \
        ((ctx)->ns_stack->def_ns = (ns))

#define c14n_ns_stack_get_default(ctx) \
        ((ctx)->ns_stack->def_ns)


typedef struct c14n_ns_stack {
    int head; /*index of the currnt stack TOP*/
    int size; /*total size allocated for current stack*/
    axiom_namespace_t **stack; /*namespace array*/
    axiom_namespace_t *def_ns; /*default ns in current scope*/
} c14n_ns_stack_t;

typedef struct c14n_ctx {
    const axis2_env_t *env;
    const axiom_document_t *doc;
    axis2_bool_t comments;
    axis2_char_t **outbuf; 
    axis2_stream_t *outstream;
    axis2_bool_t exclusive;
    axis2_bool_t use_stream;
    const axis2_array_list_t *ns_prefixes;
    c14n_ns_stack_t *ns_stack;
} c14n_ctx_t;

/*Function prototypes for ns stack*/

static c14n_ns_stack_t*
c14n_ns_stack_create(
    const c14n_ctx_t *ctx
    );

static void
c14n_ns_stack_free(
    c14n_ctx_t *ctx
    );

static axis2_status_t 
c14n_ns_stack_find(
    const axiom_namespace_t *ns,
    const c14n_ctx_t *ctx
    );

static axis2_status_t
c14n_ns_stack_find_with_prefix_uri(
    const axis2_char_t *prefix,
    const axis2_char_t *uri,
    const c14n_ctx_t *ctx
    );

static axis2_status_t
c14n_ns_stack_add(
    axiom_namespace_t *ns,  
    const c14n_ctx_t *ctx
    );

/*ns stack implementation*/

static c14n_ns_stack_t*
c14n_ns_stack_create(
    const c14n_ctx_t *ctx
    )
{
    c14n_ns_stack_t *ns_stack = NULL;

    ns_stack = (c14n_ns_stack_t *)(AXIS2_MALLOC(ctx->env->allocator, 
                sizeof(c14n_ns_stack_t)));
   
    if (ns_stack)
    {
        ns_stack->head = 0;
        ns_stack->size = 0;
        ns_stack->stack = NULL;
        ns_stack->def_ns = NULL;
    }
    else
        AXIS2_ERROR_SET(ctx->env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);

    return ns_stack;
}

static axis2_status_t
c14n_ns_stack_add(
    axiom_namespace_t *ns,  
    const c14n_ctx_t *ctx
    )
{
    c14n_ns_stack_t *ns_stack = ctx->ns_stack;

    if (!ns_stack->stack)
    {
        ns_stack->stack = (axiom_namespace_t **)(AXIS2_MALLOC(
                ctx->env->allocator, 
                sizeof(axiom_namespace_t*) * DEFAULT_STACK_SIZE));
        if (!ns_stack->stack)
        {
            AXIS2_ERROR_SET(ctx->env->error, AXIS2_ERROR_NO_MEMORY, 
                    AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        else ns_stack->size = DEFAULT_STACK_SIZE;
    }
    else if (ns_stack->head >= ns_stack->size)
    {
        int size = 2 * ns_stack->size;
        axiom_namespace_t **tmp_stack = (axiom_namespace_t **)(AXIS2_MALLOC(
                ctx->env->allocator, sizeof(axiom_namespace_t*) * size));
        if (tmp_stack)
        {
            int i = 0;
            ns_stack->size = size;
            /* TODO: use memcpy for this.*/
            for (i=0; i<ns_stack->size; i++)
                tmp_stack[i] = (ns_stack->stack)[i];

            AXIS2_FREE(ctx->env->allocator, ns_stack->stack);
            ns_stack->stack = tmp_stack;
        }
        else
        {
            AXIS2_ERROR_SET(ctx->env->error, AXIS2_ERROR_NO_MEMORY, 
                    AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }

    /*if memory overflow occur we won't be here*/
    (ns_stack->stack)[ns_stack->head] = ns;
    (ns_stack->head)++;

    return AXIS2_SUCCESS;
}

/*
 * TODO: DONE
 * Find process should be changed ( if the ns has the same prefix but diff uri) 
 * Eg: <a xmlns:x="a">
 *      <b xmlns:x="b">
 *       <c xmlns:x="a"/>
 *      </b>
 *     </a>
 * */
static axis2_status_t
c14n_ns_stack_find_with_prefix_uri(
    const axis2_char_t *prefix,
    const axis2_char_t *uri,
    const c14n_ctx_t *ctx
    )
{
    int i;
    c14n_ns_stack_t *ns_stack = ctx->ns_stack;
    if (ns_stack->stack) /*Is this necessary?*/
    {
        for (i = ns_stack->head-1; i>=0; i--)
        {
            axis2_char_t *prefix_i = AXIOM_NAMESPACE_GET_PREFIX(
                    (ns_stack->stack)[i], ctx->env);

            if (AXIS2_STRCMP(prefix_i, prefix) == 0)
            {
                axis2_char_t *uri_i = AXIOM_NAMESPACE_GET_URI((ns_stack->stack)[i],
                        ctx->env);
                if (AXIS2_STRCMP(uri_i, uri) == 0)
                    return AXIS2_SUCCESS;
                else
                    return AXIS2_FALSE;
            }
            else
                continue;
                
        }
    }
    return AXIS2_FAILURE;
}

static axis2_status_t 
c14n_ns_stack_find(
    const axiom_namespace_t *ns,
    const c14n_ctx_t *ctx
    )
{ 
    return (c14n_ns_stack_find_with_prefix_uri(
            AXIOM_NAMESPACE_GET_PREFIX((axiom_namespace_t *)ns, ctx->env),
            AXIOM_NAMESPACE_GET_URI((axiom_namespace_t *)ns, ctx->env), ctx)
            );
}

static void 
c14n_ns_stack_free(
    c14n_ctx_t *ctx
    )
{
    if (ctx->ns_stack->stack)
    {
        AXIS2_FREE(ctx->env->allocator, ctx->ns_stack->stack);
        ctx->ns_stack->stack=NULL;
    }
    ctx->ns_stack->stack = NULL;
    ctx->ns_stack->head = 0;
    ctx->ns_stack->size = 0;

    /**/
    AXIS2_FREE(ctx->env->allocator, ctx->ns_stack);
    ctx->ns_stack = NULL;
}

/* Function Prototypes */

static axis2_status_t
c14n_apply_on_node(
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    );

static axis2_status_t
c14n_apply_on_element(
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    );

static axis2_status_t
c14n_apply_on_namespace_axis(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    );

static axis2_status_t
c14n_apply_on_namespace_axis_exclusive(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    );

static axis2_status_t
c14n_apply_on_attribute_axis(
    const axiom_element_t *ele,
    const c14n_ctx_t *ctx
    );

static axis2_status_t 
c14n_apply_on_node (
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    );

static void 
c14n_apply_on_comment (
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    );

static void 
c14n_output(
    const axis2_char_t *str, 
    const c14n_ctx_t *ctx
    );

static int
attr_compare(
    const void *a1,
    const void *a2,
    const void *context
    );

static int
ns_prefix_compare(
    const void *ns1,
    const void *ns2,
    const void *context
    );

static int
ns_uri_compare(
    const void *ns1,
    const void *ns2,
    const void *context
    );

static axis2_char_t*
c14n_normalize_attribute(
    axis2_char_t *attval,
    const c14n_ctx_t *ctx
    );

static axis2_char_t*
c14n_normalize_text(
    axis2_char_t *text,
    const c14n_ctx_t *ctx
    );

static void 
c14n_apply_on_namespace(
    const void *ns,
    const void *ctx
    );

static axis2_bool_t
c14n_need_to_declare_ns(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const axiom_namespace_t *ns,
    const c14n_ctx_t *ctx
    );

static axis2_bool_t
c14n_ns_visibly_utilized(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const axiom_namespace_t *ns,
    const c14n_ctx_t *ctx
    );

static axis2_bool_t
c14n_no_output_ancestor_uses_prefix(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const axiom_namespace_t *ns,
    const c14n_ctx_t *ctx
    );

/* Implementations */

static void
c14n_ctx_free(
    c14n_ctx_t *ctx
    )
{
    if (ctx)
    {
        c14n_ns_stack_free(ctx);
    }
    
    AXIS2_FREE(ctx->env->allocator, ctx);
}

static c14n_ctx_t* 
c14n_init(
    const axis2_env_t *env,
    const axiom_document_t *doc,
    axis2_bool_t comments,
    axis2_char_t **outbuf,
    axis2_stream_t *stream,
    const axis2_bool_t exclusive,
    const axis2_array_list_t *ns_prefixes,
    const axis2_bool_t use_stream
    )
{
    c14n_ctx_t *ctx = (c14n_ctx_t *)(AXIS2_MALLOC(env->allocator, 
                sizeof(c14n_ctx_t)));
    if (ctx)
    {
        ctx->env = env;
        ctx->doc = doc;
        ctx->comments = comments;
        ctx->outbuf = outbuf;
        ctx->exclusive = exclusive;
        ctx->ns_prefixes = ns_prefixes;
        ctx->use_stream = use_stream;
        if (use_stream)
            ctx->outstream = stream;

        /*this should come after ctx->env=env*/
        ctx->ns_stack = c14n_ns_stack_create(ctx);
    }
    else
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);

    return ctx;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_c14n_apply_stream(
    const axis2_env_t *env,
    const axiom_document_t *doc,
    axis2_bool_t comments,
    axis2_stream_t *stream,
    const axis2_bool_t exclusive,
    const axis2_array_list_t *ns_prefixes
    )
{
    c14n_ctx_t *ctx = NULL;
    axiom_node_t *root_node = NULL;
    axis2_status_t status = AXIS2_SUCCESS;    
    axiom_element_t *root_ele = NULL;
    /*axiom_children_iterator_t *child_itr = NULL;*/
    axis2_stream_t *outstream = NULL;

    ctx = c14n_init(env, doc, comments, NULL, stream, exclusive, ns_prefixes,
            AXIS2_TRUE);

    if (ctx && ctx->outstream)
    {

        root_node = AXIOM_DOCUMENT_GET_ROOT_ELEMENT((axiom_document_t *)doc,
                env); 
        /* shouldn't the called method's document be const?*/

        root_ele = AXIOM_NODE_GET_DATA_ELEMENT(root_node, env);
        status = c14n_apply_on_node(root_node, ctx);
        
        if (!status)
        {
            AXIS2_STREAM_FREE(ctx->outstream, env);
            ctx->outstream = NULL;
        }

        outstream = ctx->outstream;

#ifdef TEST 
        printf("--------------\n");
        axiom_namespace_t *ns1 = NULL;
        axiom_namespace_t *ns2 = NULL;
        axiom_namespace_t *ns3 = NULL;
        int i = 0;
        for (i=0; i<17; i++)
        {
            char uri[10], pfx[10];
            sprintf(uri, "urn:ns%d", i);
            sprintf(pfx, "ns%d", i);
            ns1 = axiom_namespace_create(ctx->env, uri, pfx); 
            c14n_ns_stack_add(ns1, ctx);
        }
        for (i=0; i<ctx->ns_stack->head; i++)
        {
            ns1 = ctx->ns_stack->stack[i];
            printf("%s:%s\n", AXIOM_NAMESPACE_GET_PREFIX(ns1, env), 
                    AXIOM_NAMESPACE_GET_URI(ns1, env));
        }
        printf("%d\n", ctx->ns_stack->size);
        
        ns1 = axiom_namespace_create(ctx->env, "urn:ns0", "ns0");
        ns2 = axiom_namespace_create(ctx->env, "urn:ns10", "ns10");
        ns3 = axiom_namespace_create(ctx->env, "urn:ns2", "ns3");
        
        if (c14n_ns_stack_find(ns1, ctx)) printf("ns1 found\n");
        if (c14n_ns_stack_find(ns2, ctx)) printf("ns2 found\n");
        if (c14n_ns_stack_find(ns3, ctx)) printf("ns3 found\n");
#endif
        
        c14n_ctx_free(ctx);
        ctx = NULL;
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        
        if (ctx)
        {
            c14n_ctx_free(ctx);
            ctx = NULL;
        }
        status = AXIS2_FAILURE;
    }

    return status;

    /*if (status)
        return outstream;
    else
        return NULL;*/
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_c14n_apply(
    const axis2_env_t *env,
    const axiom_document_t *doc,
    axis2_bool_t comments,
    axis2_char_t **outbuf,
    const axis2_bool_t exclusive,
    const axis2_array_list_t *ns_prefixes
    )
{
    axis2_stream_t *stream = axis2_stream_create_basic(env);
    axis2_status_t ret = oxs_c14n_apply_stream(env, doc, comments, 
            stream, exclusive, ns_prefixes);

    *outbuf = NULL;

    if (!ret)
    {
        return AXIS2_FAILURE;
    }
    else
    {
        int len = AXIS2_STREAM_BASIC_GET_LEN(stream, env)+1;
        if (len>0)
        {
            *outbuf = (axis2_char_t *)(AXIS2_MALLOC(env->allocator, 
                        sizeof(axis2_char_t)*len));
            AXIS2_STREAM_READ(stream, env, *outbuf, len);
            AXIS2_STREAM_FREE(stream, env);
            stream = NULL;
            return AXIS2_SUCCESS;
        }
        else
        {
            AXIS2_STREAM_FREE(stream, env);
            stream = NULL;
            return AXIS2_FAILURE;
        }
    }
}

static axis2_status_t 
c14n_apply_on_text (
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    )
{   
    axiom_text_t *text = NULL;
    text = (axiom_text_t *)AXIOM_NODE_GET_DATA_ELEMENT((axiom_node_t *)node,
            ctx->env);
    
    if (text)
    {
        axis2_char_t *textval = AXIOM_TEXT_GET_VALUE(text, ctx->env);
        textval = c14n_normalize_text(textval, ctx);
        c14n_output(textval, ctx);
        if (textval)
        {
            AXIS2_FREE(ctx->env->allocator, textval);
            textval = NULL;
        }
    }
    
    return AXIS2_SUCCESS;
}

static axis2_status_t 
c14n_apply_on_node (
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    )
{
/*    printf("%d %d %d\n", AXIOM_NODE_GET_NODE_TYPE((axiom_node_t *)node, ctx->env), AXIOM_COMMENT, AXIOM_ELEMENT); */
    switch (AXIOM_NODE_GET_NODE_TYPE((axiom_node_t *)node, ctx->env))
    {
        case AXIOM_ELEMENT:
            c14n_apply_on_element(node, ctx);
            break;
        case AXIOM_TEXT:
            c14n_apply_on_text(node, ctx);
            break;
        case AXIOM_COMMENT:
            if (ctx->comments)
            {
                c14n_apply_on_comment(node, ctx);
                break;
            }
        case AXIOM_DOCTYPE:
        case AXIOM_PROCESSING_INSTRUCTION:
        default:;
    }
    
    return AXIS2_SUCCESS;
}

static void
c14n_apply_on_comment (
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    )
{
    /*TODO: HACK*/
    c14n_output("<!--", ctx);
    c14n_output(AXIOM_COMMENT_GET_VALUE(
                        (axiom_comment_t*)AXIOM_NODE_GET_DATA_ELEMENT(
                        (axiom_node_t*)node, ctx->env),ctx->env), ctx);
    c14n_output("-->", ctx);
}

static axis2_status_t
c14n_apply_on_element(
    const axiom_node_t const *node,
    const c14n_ctx_t *ctx
    )
{
    axis2_status_t res = AXIS2_SUCCESS;
    axiom_element_t *ele = NULL;
    axiom_namespace_t *ns = NULL;
    axiom_children_iterator_t *child_itr = NULL;
    c14n_ns_stack_t *save_stack = NULL;

    ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT((axiom_node_t *)node, 
            ctx->env);

    if (!ele) return AXIS2_FAILURE; /*should it be failure?*/
    
    ns = AXIOM_ELEMENT_GET_NAMESPACE(ele, ctx->env, (axiom_node_t *)node);
    
    save_stack = c14n_ns_stack_create(ctx);
    c14n_ns_stack_push(save_stack, ctx); /*save current ns stack*/
    
    /*print qname*/
    c14n_output("<", ctx);
    
    if (ns)
    {
        axis2_char_t *prefix = AXIOM_NAMESPACE_GET_PREFIX(ns, ctx->env);

        if (AXIS2_STRLEN(prefix) > 0)
        {
            c14n_output(prefix, ctx);
            c14n_output(":", ctx);
        }
    }
    c14n_output(AXIOM_ELEMENT_GET_LOCALNAME(ele, ctx->env), ctx);

    if (ctx->exclusive)
        res = c14n_apply_on_namespace_axis_exclusive(ele, node, ctx);
    else
        res = c14n_apply_on_namespace_axis(ele, node, ctx);
    
    /*
     * edited the code so that the same fn does both exc and non-exc.
     * have to be careful here!
     */
    
    if (!res) return res;
    
    res = c14n_apply_on_attribute_axis(ele, ctx);

    if (!res) return res;

    c14n_output(">", ctx);

#ifdef C14N_DEBUG
    c14n_output("\n", ctx);
#endif

    /*process child elements*/

    child_itr = AXIOM_ELEMENT_GET_CHILDREN(ele, ctx->env, (axiom_node_t*)node);
    if (child_itr)
    {
        /*AXIOM_CHILDREN_ITERATOR_RESET(child_itr, ctx->env);*/

        while(AXIOM_CHILDREN_ITERATOR_HAS_NEXT(child_itr, ctx->env))
        {
            axiom_node_t *child_node = NULL;
            child_node = AXIOM_CHILDREN_ITERATOR_NEXT(child_itr, ctx->env);
            
            if (child_node)
            {
                c14n_apply_on_node(child_node, ctx);
            }
        }
    }
    

    /*print qname*/
    c14n_output("</", ctx);
    
    if (ns)
    {
        axis2_char_t *prefix = AXIOM_NAMESPACE_GET_PREFIX(ns, ctx->env);

        if (AXIS2_STRLEN(prefix) > 0)
        {
            c14n_output(prefix, ctx);
            c14n_output(":", ctx);
        }
    }
    c14n_output(AXIOM_ELEMENT_GET_LOCALNAME(ele, ctx->env), ctx);

    c14n_output(">", ctx);

    c14n_ns_stack_pop(save_stack, ctx); /*restore to previous ns stack */
    /*TODO: save_stack free*/
    /*since save_stack is used just to memorize the head of the stack,
     * we don't have to worry about freeing its members*/
    
    /**/
    AXIS2_FREE(ctx->env->allocator, save_stack);
    save_stack = NULL;

#ifdef C14N_DEBUG
    c14n_output("\n", ctx);
#endif

    return res;
}

static int
ns_uri_compare(
    const void *ns1,
    const void *ns2,
    const void *context
    )
{
    c14n_ctx_t *ctx = (c14n_ctx_t *)context;

    if (ns1 == ns2) return 0;
    if (!ns1) return -1;
    if (!ns2) return 1;

    return (AXIS2_STRCMP(
            (const axis2_char_t *)AXIOM_NAMESPACE_GET_URI(
                (axiom_namespace_t *)ns1, ctx->env),
            (const axis2_char_t *)AXIOM_NAMESPACE_GET_URI(
                (axiom_namespace_t *)ns2, ctx->env)));
}

static int
ns_prefix_compare(
    const void *ns1,
    const void *ns2,
    const void *context
    )
{
    c14n_ctx_t *ctx = (c14n_ctx_t *)context;

    if (ns1 == ns2) return 0;
    if (!ns1) return -1;
    if (!ns2) return 1;

    return (AXIS2_STRCMP(
            (const axis2_char_t *)AXIOM_NAMESPACE_GET_PREFIX(
                (axiom_namespace_t *)ns1, ctx->env),
            (const axis2_char_t *)AXIOM_NAMESPACE_GET_PREFIX(
                (axiom_namespace_t *)ns2, ctx->env)));
}

static int
attr_compare(
    const void *a1,
    const void *a2,
    const void *context
    )
{
    c14n_ctx_t *ctx = (c14n_ctx_t *)context;
    axiom_attribute_t *attr1 = NULL;
    axiom_attribute_t *attr2 = NULL;
    axiom_namespace_t *ns1 = NULL;
    axiom_namespace_t *ns2 = NULL;
    int res;

    if (a1 == a2) return 0;
    if (!a1) return -1;
    if (!a2) return 1;
    
    attr1 = (axiom_attribute_t *)a1;
    attr2 = (axiom_attribute_t *)a2;
    ns1 = AXIOM_ATTRIBUTE_GET_NAMESPACE((axiom_attribute_t *)a1, ctx->env);
    ns2 = AXIOM_ATTRIBUTE_GET_NAMESPACE((axiom_attribute_t *)a2, ctx->env);

    if (ns1 == ns2)
        return AXIS2_STRCMP(
                (const axis2_char_t *)AXIOM_ATTRIBUTE_GET_LOCALNAME(
                    (axiom_attribute_t *)a1, ctx->env),
                (const axis2_char_t *)AXIOM_ATTRIBUTE_GET_LOCALNAME(
                    (axiom_attribute_t *)a2, ctx->env));
    
    if (!ns1) return -1;
    if (!ns2) return 1;

    res = AXIS2_STRCMP(
            AXIOM_NAMESPACE_GET_URI(ns1, ctx->env),
            AXIOM_NAMESPACE_GET_URI(ns2, ctx->env));

    if (res == 0)
        return AXIS2_STRCMP(
                (const axis2_char_t *)AXIOM_ATTRIBUTE_GET_LOCALNAME(
                    (axiom_attribute_t *)a1, ctx->env),
                (const axis2_char_t *)AXIOM_ATTRIBUTE_GET_LOCALNAME(
                    (axiom_attribute_t *)a2, ctx->env));
    else
        return res;
    
}



static void
c14n_apply_on_attribute(
    const void *attribute,
    const void *context
    )
{
    c14n_ctx_t *ctx = (c14n_ctx_t *) context;
    axiom_attribute_t *attr = (axiom_attribute_t *) attribute;
    axiom_namespace_t *ns = AXIOM_ATTRIBUTE_GET_NAMESPACE(attr, ctx->env);
    axis2_char_t *attvalue = NULL;

    c14n_output(" ", ctx);
    if (ns)
    {
        axis2_char_t *prefix = AXIOM_NAMESPACE_GET_PREFIX(ns, ctx->env);

        if (AXIS2_STRLEN(prefix) > 0)
        {
            c14n_output(prefix, ctx);
            c14n_output(":", ctx);
        }
    }
    c14n_output(AXIOM_ATTRIBUTE_GET_LOCALNAME(attr, ctx->env), ctx);
    c14n_output("=\"", ctx);
    
    /* TODO:DONE Normalize the text before output */
    attvalue = AXIOM_ATTRIBUTE_GET_VALUE(attr, ctx->env);
    attvalue = c14n_normalize_attribute(attvalue, (c14n_ctx_t const *)context);

    c14n_output(attvalue, ctx);
    
    c14n_output("\"", ctx); 
    
    if (attvalue)
    {
        AXIS2_FREE(ctx->env->allocator, attvalue);
        attvalue = NULL;
    }
}

static axis2_status_t
c14n_apply_on_attribute_axis(
    const axiom_element_t *ele,
    const c14n_ctx_t *ctx
    )
{
    axis2_hash_t *attr_ht = NULL;
    axis2_hash_index_t *hi = NULL;
    c14n_sorted_list_t *attr_list = c14n_sorted_list_create(ctx->env);
 
    attr_ht = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES((axiom_element_t *)ele, 
            ctx->env);
    
    if(attr_ht)
    {
        for (hi = axis2_hash_first(attr_ht, ctx->env); hi; 
                hi = axis2_hash_next(ctx->env, hi)) 
        {
            void *v = NULL;
            axis2_hash_this(hi, NULL, NULL, &v);
    
            if (v)
            {
                AXIS2_SORTED_LIST_INSERT(&attr_list, v, ctx, attr_compare, 
                        ctx->env);
            }
        }

        AXIS2_SORTED_LIST_ITERATE(attr_list, ctx, c14n_apply_on_attribute, 
                ctx->env);
    }
    /*TODO: AXIS2_SORTED_LIST_FREE();*/
    return AXIS2_SUCCESS;

    /* TODO: Still need to add the "xml" attrs of the parents in case of doc subsets
     * and non-exclusive c14n
     * */
}

static axis2_char_t*
c14n_normalize_text(
    axis2_char_t *text,
    const c14n_ctx_t *ctx
    )
{   
    axis2_char_t *buf = NULL;
    axis2_char_t *p = NULL;
    axis2_char_t *old = NULL;
    /* TODO: a better buffer implementation */
    buf = (axis2_char_t *)(AXIS2_MALLOC(ctx->env->allocator, 
                sizeof(axis2_char_t)*256));
    p = buf;
    old = text;

    while (*old !='\0')
    {
        switch (*old)
        {
            case '&':
                *p++ = '&'; 
                *p++ = 'a'; 
                *p++ = 'm'; 
                *p++ = 'p'; 
                *p++ = ';'; 
                break;
            case '>':
                *p++ = '&'; 
                *p++ = 'g'; 
                *p++ = 't'; 
                *p++ = ';'; 
                break;
            case '<':
                *p++ = '&'; 
                *p++ = 'l'; 
                *p++ = 't'; 
                *p++ = ';'; 
                break;
            case '\x0D':
                *p++ = '&';
                *p++ = '#'; 
                *p++ = 'x'; 
                *p++ = 'D'; 
                *p++ = ';'; 
                break;
            default:
                *p++ = *old;
        }
        old ++;
    }
    *p++ = '\0';
    return buf;
}

static axis2_char_t*
c14n_normalize_attribute(
    axis2_char_t *attval,
    const c14n_ctx_t *ctx
    )
{   
    axis2_char_t *buf = NULL;
    axis2_char_t *p = NULL;
    axis2_char_t *old = NULL;
    /* TODO: a better buffer implementation */
    buf = (axis2_char_t *)(AXIS2_MALLOC(ctx->env->allocator, 
                sizeof(axis2_char_t)*1024));
    p = buf;
    old = attval;

    while (*old !='\0')
    {
        switch (*old)
        {
            case '&':
                *p++ = '&'; 
                *p++ = 'a'; 
                *p++ = 'm'; 
                *p++ = 'p'; 
                *p++ = ';'; 
                break;
            case '<':
                *p++ = '&'; 
                *p++ = 'l'; 
                *p++ = 't'; 
                *p++ = ';'; 
                break;
            case '"':
                *p++ = '&'; 
                *p++ = 'q'; 
                *p++ = 'u'; 
                *p++ = 'o'; 
                *p++ = 't'; 
                *p++ = ';'; 
                break;
            case '\x09':
                *p++ = '&'; 
                *p++ = '#'; 
                *p++ = 'x'; 
                *p++ = '9'; 
                *p++ = ';'; 
                break;
            case '\x0A':
                *p++ = '&'; 
                *p++ = '#'; 
                *p++ = 'x'; 
                *p++ = 'A'; 
                *p++ = ';'; 
                break;
            case '\x0D':
                *p++ = '&'; *p++ = '#'; *p++ = 'x'; *p++ = 'D'; *p++ = ';'; 
                break;
            default:
                *p++ = *old;
        }
        old ++;
    }
    *p++ = '\0';
    return buf;
}


static axis2_status_t
c14n_apply_on_namespace_axis(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    )
{   
    axis2_hash_t *ns_ht = NULL;
    axis2_hash_index_t *hi = NULL;
    
    c14n_sorted_list_t *out_list = c14n_sorted_list_create(ctx->env);


    ns_ht = AXIOM_ELEMENT_GET_NAMESPACES((axiom_element_t *)ele, ctx->env);
    
    if(ns_ht)
    {
        for (hi = axis2_hash_first(ns_ht, ctx->env); hi; 
                hi = axis2_hash_next(ctx->env, hi)) 
        {
            void *v = NULL;
            axis2_hash_this(hi, NULL, NULL, &v);
    
            if (v)
            {
                axiom_namespace_t *ns = (axiom_namespace_t *) v;
                 
                axis2_char_t *pfx = AXIOM_NAMESPACE_GET_PREFIX(ns, ctx->env);
                axis2_char_t *uri = AXIOM_NAMESPACE_GET_URI(ns, ctx->env);
                
                if (AXIS2_STRLEN(pfx) == 0)
                {
                    /*process for default namespace*/
                    /*int nsc = ns_prefix_compare(c14n_ns_stack_get_default(ctx), ns, ctx); 
                    int len = AXIS2_STRLEN(uri);*/

                    if (AXIS2_STRLEN(uri) == 0) 
                    {
                        if (c14n_ns_stack_get_default(ctx)!=NULL)
                        {
                            c14n_ns_stack_set_default(ns, ctx);
                            AXIS2_SORTED_LIST_INSERT(&out_list, (void *)ns, ctx,
                                    ns_prefix_compare, ctx->env);
                        }
                             
                    }
                    else
                    {
                        axiom_namespace_t *prev_def = 
                            c14n_ns_stack_get_default(ctx);

                        axis2_char_t *prev_def_uri = ((prev_def) ? 
                            AXIOM_NAMESPACE_GET_URI(prev_def, ctx->env) : NULL);
                        

                        if (!prev_def_uri || AXIS2_STRCMP(prev_def_uri, uri) != 0)
                        {
                            c14n_ns_stack_set_default(ns, ctx);
                            AXIS2_SORTED_LIST_INSERT(&out_list, (void *)ns, ctx,
                                    ns_prefix_compare, ctx->env);
                        }
                    }
                }
                else if (!c14n_ns_stack_find(ns, ctx))
                {
                    /*non-default namespace*/
                    c14n_ns_stack_add(ns, ctx);
                    AXIS2_SORTED_LIST_INSERT(&out_list, (void *)ns, ctx,
                            ns_prefix_compare, ctx->env);
                }
            }
        }
    }
    
    AXIS2_SORTED_LIST_ITERATE(out_list, ctx, c14n_apply_on_namespace, ctx->env);
    
    /*TODO: AXIS2_SORTED_LIST_FREE();*/
    return AXIS2_SUCCESS;
}

static axis2_status_t
c14n_apply_on_namespace_axis_exclusive(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const c14n_ctx_t *ctx
    )
{   
    axis2_hash_t *ns_ht = NULL;
    axis2_hash_index_t *hi = NULL;
    axiom_node_t *pnode = NULL;
    axiom_element_t *pele = NULL;
    axiom_namespace_t *ns = NULL;

    c14n_sorted_list_t *out_list = c14n_sorted_list_create(ctx->env);
    
    pele = (axiom_element_t *)ele; 
    pnode = (axiom_node_t *)node;

    /*treat the default namespace specially*/

    ns = AXIOM_ELEMENT_GET_NAMESPACE(pele, ctx->env,
            pnode);

    if (ns)
    {
        if (AXIS2_STRLEN(AXIOM_NAMESPACE_GET_PREFIX((axiom_namespace_t *)ns,
                        ctx->env)) == 0)
        {
            axiom_namespace_t *def_ns = c14n_ns_stack_get_default(ctx);
            if (def_ns || AXIS2_STRLEN(AXIOM_NAMESPACE_GET_URI(
                            (axiom_namespace_t *)ns, ctx->env)) != 0)
            {
                if (ns_uri_compare(ns, def_ns, ctx) != 0)
                {
                    c14n_ns_stack_set_default(ns, ctx);
                    AXIS2_SORTED_LIST_INSERT(&out_list, (void *)ns,
                            ctx, ns_prefix_compare, ctx->env);
                }
            }
        }
    }

    while (pnode)
    {
        pele = AXIOM_NODE_GET_DATA_ELEMENT((axiom_node_t *)pnode, ctx->env);
        ns_ht = AXIOM_ELEMENT_GET_NAMESPACES((axiom_element_t *)pele, ctx->env);
        
        if (ns_ht)
        {
            for (hi = axis2_hash_first(ns_ht, ctx->env); hi; 
                    hi = axis2_hash_next(ctx->env, hi)) 
            {
                void *v = NULL;
                axis2_hash_this(hi, NULL, NULL, &v);
        
                if (v)
                {
                    ns = (axiom_namespace_t *) v;
                     
                    axis2_char_t *pfx = AXIOM_NAMESPACE_GET_PREFIX(ns, ctx->env);
                    /*axis2_char_t *uri = AXIOM_NAMESPACE_GET_URI(ns, ctx->env);*/
                    
                    if (AXIS2_STRLEN(pfx) == 0)
                    {
                        /* process for default namespace. 
                         * NOTE: This part was taken out of here due to the 
                         * search thruogh parent-axis
                         * */
                    }
                    else if (!c14n_ns_stack_find(ns, ctx))
                    {
                        /*non-default namespace*/
                        if (c14n_need_to_declare_ns(ele, node, ns, ctx))
                        {
                            c14n_ns_stack_add(ns, ctx);
                            AXIS2_SORTED_LIST_INSERT(&out_list, (void *)ns, ctx,
                                    ns_prefix_compare, ctx->env);
                        }
                    }
                }
            }
        }
        pnode = AXIOM_NODE_GET_PARENT((axiom_node_t *)pnode, ctx->env);
    } /*while*/
    AXIS2_SORTED_LIST_ITERATE(out_list, ctx, c14n_apply_on_namespace , ctx->env);
    

    /*TODO: AXIS2_SORTED_LIST_FREE();*/
    return AXIS2_SUCCESS;
}

static void
c14n_apply_on_namespace(
    const void *namespace,
    const void *context
    )
{
    axiom_namespace_t *ns = (axiom_namespace_t *)namespace;
    c14n_ctx_t *ctx = (c14n_ctx_t *)context;
 
    axis2_char_t *pfx = AXIOM_NAMESPACE_GET_PREFIX(ns, ctx->env);
    axis2_char_t *uri = AXIOM_NAMESPACE_GET_URI(ns, ctx->env);
    
    /*c14n_output(" *", ctx);
    c14n_output(AXIOM_NAMESPACE_TO_STRING(ns, ctx->env), ctx);
    c14n_output("*", ctx);*/

    if (AXIS2_STRLEN(pfx) > 0)
    {
        c14n_output(" xmlns:", ctx);
        c14n_output(pfx, ctx);
    }
    else
        c14n_output(" xmlns", ctx);
    
    c14n_output("=\"", ctx);
    
    if (AXIS2_STRLEN(uri) > 0)
        c14n_output(uri, ctx);
    c14n_output("\"", ctx);
    
}

static void 
c14n_output(
    const axis2_char_t *str, 
    const c14n_ctx_t *ctx
    )
{
#ifdef C14N_DEBUG
    printf("%s", str);
#else
    if (ctx->use_stream)
    {
        AXIS2_STREAM_WRITE(ctx->outstream, ctx->env, str,
                AXIS2_STRLEN(str)*sizeof(axis2_char_t));
    }
#endif
}

static axis2_bool_t
c14n_need_to_declare_ns(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const axiom_namespace_t *ns,
    const c14n_ctx_t *ctx
    )
{
    axis2_bool_t vu = c14n_ns_visibly_utilized(ele, node, ns, ctx);

    if (vu || (ctx->ns_prefixes && AXIS2_ARRAY_LIST_CONTAINS(
                (axis2_array_list_t*)(ctx->ns_prefixes), ctx->env, 
                (void*)(AXIOM_NAMESPACE_GET_PREFIX((axiom_namespace_t*)ns,
                        ctx->env)))))
        return c14n_no_output_ancestor_uses_prefix(ele, node, ns, ctx);

    return AXIS2_FALSE;
}

static axis2_bool_t
c14n_ns_visibly_utilized(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const axiom_namespace_t *ns,
    const c14n_ctx_t *ctx
    )
{
    axis2_bool_t vu = AXIS2_FALSE;
    axiom_namespace_t *ns_ele = NULL;

    axis2_char_t *pfx = AXIOM_NAMESPACE_GET_PREFIX((axiom_namespace_t*)ns, 
            ctx->env);
    axis2_char_t *uri = AXIOM_NAMESPACE_GET_URI((axiom_namespace_t *)ns, 
            ctx->env);
    axis2_char_t *pfx_ele = NULL;
    axis2_char_t *uri_ele = NULL;
    ns_ele = AXIOM_ELEMENT_GET_NAMESPACE((axiom_element_t*)ele, ctx->env,
            (axiom_node_t *)node);
    
    if (ns_ele)  /* return AXIS2_FALSE; TODO:check */
    {
        pfx_ele = AXIOM_NAMESPACE_GET_PREFIX(ns_ele, ctx->env);
        uri_ele = AXIOM_NAMESPACE_GET_URI(ns_ele, ctx->env);
    }
    if ((AXIS2_STRCMP(pfx, pfx_ele) == 0) && (AXIS2_STRCMP(uri, uri_ele) == 0))
        vu = AXIS2_TRUE;
    else
    {
        axis2_hash_t *attr_ht = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(
                (axiom_element_t *)ele, ctx->env);
        axis2_hash_index_t *hi = NULL;
        if (attr_ht)
        {
            for (hi = axis2_hash_first(attr_ht, ctx->env); hi; 
                    hi = axis2_hash_next(ctx->env, hi)) 
            {
                void *v = NULL;
                axis2_hash_this(hi, NULL, NULL, &v);
        
                if (v)
                {
                    axiom_attribute_t *attr = (axiom_attribute_t*)v;
                    axiom_namespace_t *ns_attr = AXIOM_ATTRIBUTE_GET_NAMESPACE(
                            attr, ctx->env);
                    axis2_char_t *attr_pfx = NULL;
                    
                    /*if in_nodelist(attr) {*/
                    if (ns_attr)
                        attr_pfx = AXIOM_NAMESPACE_GET_PREFIX(
                                ns_attr, ctx->env);

                    if (AXIS2_STRCMP(attr_pfx, pfx) == 0)
                    {
                        vu = AXIS2_TRUE;
                        break;
                    }
                    /**/
                }
            }
        }
     
    }

    return vu;
}

static axis2_bool_t
c14n_no_output_ancestor_uses_prefix(
    const axiom_element_t *ele,
    const axiom_node_t *node,
    const axiom_namespace_t *ns,
    const c14n_ctx_t *ctx
    )
{
    axis2_char_t *pfx = AXIOM_NAMESPACE_GET_PREFIX((axiom_namespace_t*)ns, 
            ctx->env);
    axis2_char_t *uri = AXIOM_NAMESPACE_GET_URI((axiom_namespace_t *)ns, 
            ctx->env);
    
    axiom_node_t *parent_node = AXIOM_NODE_GET_PARENT((axiom_node_t *)node, 
            ctx->env);
    axiom_element_t *parent_element = NULL; 
    axiom_namespace_t *parent_ns = NULL;
    axis2_char_t *parent_pfx = NULL;
    axis2_char_t *parent_uri = NULL;

    /* assuming the parent  of an element is always an element node in AXIOM*/
    while (parent_node) 
    {
        axis2_hash_index_t *hi = NULL;
        axis2_hash_t *attr_ht = NULL;
        
        /* if (in_nodeset(parent)){*/
        parent_element = AXIOM_NODE_GET_DATA_ELEMENT(
                (axiom_node_t *)parent_node, ctx->env);
        parent_ns = AXIOM_ELEMENT_GET_NAMESPACE((axiom_element_t *) 
                parent_element, ctx->env, (axiom_node_t *)parent_node);
        
        if (parent_ns)  
        {    
            parent_pfx = AXIOM_NAMESPACE_GET_PREFIX((axiom_namespace_t *)parent_ns,
                    ctx->env);
            if (AXIS2_STRCMP(pfx, parent_pfx) == 0)
            {
                parent_uri = AXIOM_NAMESPACE_GET_URI((axiom_namespace_t*)parent_ns,
                        ctx->env);
                return (!(AXIS2_STRCMP(uri, parent_uri) == 0));
            }
        }

        attr_ht = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(
                (axiom_element_t *)parent_element, ctx->env);
        if (attr_ht)
        {
            for (hi = axis2_hash_first(attr_ht, ctx->env); hi; 
                    hi = axis2_hash_next(ctx->env, hi)) 
            {
                void *v = NULL;
                axis2_hash_this(hi, NULL, NULL, &v);
                
                if (v)
                {
                    axiom_attribute_t *attr = (axiom_attribute_t*)v;
                    axiom_namespace_t *attr_ns = AXIOM_ATTRIBUTE_GET_NAMESPACE(
                            attr, ctx->env);
                    axis2_char_t *attr_pfx = NULL;
                    axis2_char_t *attr_uri = NULL;

                    if (attr_ns)
                    {
                        attr_pfx = AXIOM_NAMESPACE_GET_PREFIX(
                                attr_ns, ctx->env);
                        attr_uri = AXIOM_NAMESPACE_GET_URI(attr_ns, 
                                ctx->env);

                        if (AXIS2_STRCMP(attr_pfx, pfx) == 0)
                            return (!(AXIS2_STRCMP(attr_uri, uri) == 0)); 
                            /*test for this case*/
                   }
                }
            }
        }
        /*}*/
        parent_node = AXIOM_NODE_GET_PARENT((axiom_node_t *)parent_node, 
                ctx->env);
    }

    return AXIS2_TRUE;
}
