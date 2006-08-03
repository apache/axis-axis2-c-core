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

#include <woden_resolver.h>
#include <woden_reader.h>
#include <woden_wsdl10_reader.h>
#include <woden_schema_constants.h>
#include <woden_qname_util.h>
#include <axis2_uri.h>

#include "../wsdl20/woden_constants.h"
#include "../wsdl10/woden_wsdl10_constants.h"
#include "../util/woden_om_util.h"

#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_attribute.h>
#include <axiom_child_element_iterator.h>
#include <axiom_namespace.h>
#include <axiom_text.h>
#include <axiom_document.h>
#include <axiom_stax_builder.h>
#include <axiom_xml_reader.h>
#include <axiom_util.h>
#include <axis2_utils.h>


typedef struct woden_resolver_impl woden_resolver_impl_t;

/** 
 * @brief Woden Wsdl Reader Struct Impl
 *   Woden Wsdl Reader  
 */ 
struct woden_resolver_impl
{
    woden_resolver_t resolver;

    axiom_document_t *om_doc;
    axiom_node_t *root_node;
    int spec;
};

#define INTF_TO_IMPL(resolver) ((woden_resolver_impl_t *) resolver)

axis2_status_t AXIS2_CALL 
woden_resolver_free(
        void *resolver,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_resolver_read(
        void *resolver,
        const axis2_env_t *env,
        axiom_document_t *om_doc,
        const axis2_char_t *doc_base_uri);

int AXIS2_CALL
woden_resolver_get_spec(
        void *resolver,
        const axis2_env_t *env);

/* Parse the attributes and child elements of the <description> element.
 * As per the WSDL 2.0 spec, the child elements must be in the 
 * following order if present:
 * <documentation>
 * <import> <include> or WSDL extension elements in any order
 * <types>
 * <interface> <binding> <service> or WSDL extension elements in any order.
 * TODO validate that the elements are in correct order
 */ 
static void *
yomu(
        void *resolver,
        const axis2_env_t *env,
        axiom_node_t *desc_el_node,
        const axis2_char_t *doc_base_uri);

/* ************************************************************
 *  Utility/helper methods
 * ************************************************************/

/**
 * Check the actual element encountered against the expected qname
 * 
 * @param el actual element encountered
 * @param qname expected element's qname
 */
static axis2_status_t
check_element_qname(
        void *resolver,
        const axis2_env_t *env,
        axiom_node_t *el_node,
        axis2_qname_t *qname);
    
static woden_resolver_t *
create(
        const axis2_env_t *env)
{
    woden_resolver_impl_t *resolver_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    resolver_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_resolver_impl_t));

    resolver_impl->root_node = NULL;
    resolver_impl->om_doc = NULL;
    resolver_impl->spec = 0;
    
    resolver_impl->resolver.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_resolver_ops_t));
    
    resolver_impl->resolver.ops->free = woden_resolver_free;
    resolver_impl->resolver.ops->read = woden_resolver_read;
    resolver_impl->resolver.ops->get_spec = woden_resolver_get_spec;

    return &(resolver_impl->resolver);
}

AXIS2_EXTERN woden_resolver_t * AXIS2_CALL
woden_resolver_create(
        const axis2_env_t *env)
{
    woden_resolver_impl_t *resolver_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    resolver_impl = (woden_resolver_impl_t *) create(env);

    return &(resolver_impl->resolver);
}

axis2_status_t AXIS2_CALL
woden_resolver_free(
        void *resolver,
        const axis2_env_t *env)
{
    woden_resolver_impl_t *resolver_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    resolver_impl = INTF_TO_IMPL(resolver);

    if((&(resolver_impl->resolver))->ops)
    {
        AXIS2_FREE(env->allocator, (&(resolver_impl->resolver))->ops);
        (&(resolver_impl->resolver))->ops = NULL;
    }

    if(resolver_impl)
    {
        AXIS2_FREE(env->allocator, resolver_impl);
        resolver_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  API public methods
 * ************************************************************/
void *AXIS2_CALL
woden_resolver_read(
        void *resolver,
        const axis2_env_t *env,
        axiom_document_t *om_doc,
        const axis2_char_t *doc_base_uri)
{
    woden_resolver_impl_t *resolver_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_doc, AXIS2_FAILURE);
    resolver_impl = INTF_TO_IMPL(resolver);
        
    /* TODO add WSDL locator for resolving URIs */
    resolver_impl->om_doc = om_doc;
    resolver_impl->root_node = AXIOM_DOCUMENT_GET_ROOT_ELEMENT(om_doc, env);            
    if(!resolver_impl->root_node)
        return AXIS2_FAILURE;
    return yomu(resolver, env, resolver_impl->root_node, doc_base_uri);
}

int AXIS2_CALL
woden_resolver_get_spec(
        void *resolver,
        const axis2_env_t *env)
{
    woden_resolver_impl_t *resolver_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    resolver_impl = INTF_TO_IMPL(resolver);

    return resolver_impl->spec;
}

static void *
yomu(
        void *resolver,
        const axis2_env_t *env,
        axiom_node_t *desc_el_node,
        const axis2_char_t *doc_base_uri)
{
    woden_resolver_impl_t *resolver_impl = NULL;
    axis2_qname_t *qname = NULL;
    axis2_bool_t check = AXIS2_FALSE;
    void *desc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, desc_el_node, AXIS2_FAILURE);
    resolver_impl = INTF_TO_IMPL(resolver);

    qname = axis2_qname_create_from_string(env, WODEN_Q_ELEM_DESCRIPTION);
    check = check_element_qname(resolver, env, desc_el_node, qname);
    if(AXIS2_TRUE == check)
    {
        woden_reader_t *reader = NULL;
        
        resolver_impl->spec = WODEN_WSDL20;
        reader = woden_reader_create(env);
        desc = WODEN_READER_READ_WSDL(reader, env, resolver_impl->root_node, 
                doc_base_uri); 
        WODEN_READER_FREE(reader, env);

    }
    AXIS2_QNAME_FREE(qname, env);
    qname = axis2_qname_create_from_string(env, WODEN_WSDL10_Q_ELEM_DEFINITIONS);
    check = check_element_qname(resolver, env, desc_el_node, qname);
    if(AXIS2_TRUE == check)
    {
        woden_wsdl10_reader_t *reader = NULL;
        
        resolver_impl->spec = WODEN_WSDL10;
        reader = woden_wsdl10_reader_create(env);
        desc = WODEN_WSDL10_READER_READ_WSDL(reader, env, resolver_impl->root_node, 
                doc_base_uri); 
        WODEN_WSDL10_READER_FREE(reader, env);
    }
    return desc;
}

/* ************************************************************
 *  Utility/helper methods
 * ************************************************************/

/**
 * Check the actual element encountered against the expected qname
 * 
 * @param el actual element encountered
 * @param qname expected element's qname
 */
static axis2_status_t
check_element_qname(
        void *resolver,
        const axis2_env_t *env,
        axiom_node_t *el_node,
        axis2_qname_t *qname)
{
    if(AXIS2_TRUE != axis2_qname_util_matches(env, qname, el_node))
    {
        /* TODO woden_wsdl_exc_set_location(wsdl_exc, env, xpath);*/
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
    
