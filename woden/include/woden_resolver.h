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

#ifndef WODEN_RESOLVER_H
#define WODEN_RESOLVER_H

/**
 * @file woden_resolver.h
 * @brief Woden Wsdl Reader Interface
 * Implements the wsdl_resolver behaviour for AXIOM-based parsing.
 * 
 */

#include <woden.h>
#include <axiom_document.h>
#include <axiom_node.h>

/** @defgroup woden_resolver Woden Wsdl Reader
  * @ingroup woden
  * @{
  */

typedef union woden_resolver_base woden_resolver_base_t;
typedef struct woden_resolver woden_resolver_t;
typedef struct woden_resolver_ops woden_resolver_ops_t;

#define WODEN_WSDL10 10
#define WODEN_WSDL20 20

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_resolver_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *resolver,
            const axis2_env_t *env);
 
    void *(AXIS2_CALL *
    read) (
            void *resolver,
            const axis2_env_t *env,
            axiom_document_t *om_doc,
            const axis2_char_t *doc_base_uri);

    int (AXIS2_CALL *
    get_spec) (
            void *resolver,
            const axis2_env_t *env);

   
};

struct woden_resolver
{
    woden_resolver_ops_t *ops;
};

AXIS2_EXTERN woden_resolver_t * AXIS2_CALL
woden_resolver_create(
        const axis2_env_t *env);

#define WODEN_RESOLVER_FREE(resolver, env) \
      (((woden_resolver_t *) resolver)->ops->free(resolver, env))

#define WODEN_RESOLVER_READ(resolver, env, om_doc, doc_base_uri) \
      (((woden_resolver_t *) resolver)->ops->\
      read (resolver, env, om_doc, doc_base_uri))

#define WODEN_RESOLVER_GET_SPEC(resolver, env) \
      (((woden_resolver_t *) resolver)->ops->\
       get_spec(resolver, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_RESOLVER_H */
