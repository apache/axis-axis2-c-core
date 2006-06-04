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

#ifndef AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_H
#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_H

/**
 * @file axis2_woden_qname_or_token_any_attr.h
 * @brief Axis2 QName or Token Any Attribute Interface
 *          This class represents XML attribute information items of type
 *          "Union of QName or xs:token #any"
 *          (e.g. the wsoap:code extension attribute of binding fault).
 */

#include <woden/axis2_woden.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <axis2_array_list.h>

/** @defgroup axis2_woden_qname_or_token_any_attr QName or Token Any Attribute
  * @ingroup axis2_wsdl
  * @{
  */

typedef struct axis2_woden_qname_or_token_any_attr 
        axis2_woden_qname_or_token_any_attr_t;
typedef struct axis2_woden_qname_or_token_any_attr_ops 
        axis2_woden_qname_or_token_any_attr_ops_t;
struct axis2_om_element;
struct axis2_om_node;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_qname_or_token_any_attr_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *token_attr,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_qname_or_token_any_attr_free) (
            void *token_attr,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *token_attr,
            const axis2_env_t *env);

    /**
     * @return the base implementation class
     */
    axis2_woden_xml_attr_t *(AXIS2_CALL *
    get_base_impl) (
            void *token_attr,
            const axis2_env_t *env);

    /* ************************************************************
     *  QNameAttr interface declared methods 
     * ************************************************************/

    axis2_bool_t (AXIS2_CALL *
    is_qname) (
            void *token_attr,
            const axis2_env_t *env); 

    axis2_bool_t (AXIS2_CALL *
    is_token) (
            void *token_attr,
            const axis2_env_t *env);


    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *token_attr,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_token) (
            void *token_attr,
            const axis2_env_t *env);

    /* ************************************************************
     *  Non-API implementation methods 
     * ************************************************************/
   
    /*
     * Convert a string of type "Union of xs:QName or xs:token #any" to a 
     * axis2_qname_t or a axis2_char_t.
     * A null argument will return a null value.
     * Any conversion error will be reported and a null value will be returned.
     */
    void *(AXIS2_CALL *
    convert) (
            void *token_attr,
            const axis2_env_t *env,
            struct axis2_om_element *owner_el,
            struct axis2_om_node *owner_node,
            axis2_char_t *attr_value);

};

struct axis2_woden_qname_or_token_any_attr
{
    axis2_woden_xml_attr_t base;
    axis2_woden_qname_or_token_any_attr_ops_t *ops;
};

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_EXTERN axis2_woden_qname_or_token_any_attr_t * AXIS2_CALL
axis2_woden_qname_or_token_any_attr_create(
        const axis2_env_t *env,
        struct axis2_om_element *owner_el,
        struct axis2_om_node *owner_node,
        axis2_qname_t *attr_type,
        axis2_char_t *attr_value);


/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_resolve_methods(
        axis2_woden_qname_or_token_any_attr_t *token_attr,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_FREE(token_attr, env) \
      (((axis2_woden_qname_or_token_any_attr_t *) \
          token_attr)->ops->free(token_attr, env))

#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_TYPE(token_attr, env) \
      (((axis2_woden_qname_or_token_any_attr_t *) token_attr)->ops->\
         type(token_attr, env))

#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_GET_BASE_IMPL(token_attr, \
        env) \
      (((axis2_woden_qname_or_token_any_attr_t *) token_attr)->ops->\
         get_base_impl(token_attr, env))

#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_IS_QNAME(token_attr, env) \
      (((axis2_woden_qname_or_token_any_attr_t *) token_attr)->ops->\
         is_qname(token_attr, env))

#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_IS_TOKEN(token_attr, env) \
      (((axis2_woden_qname_or_token_any_attr_t *) \
          token_attr)->ops->is_token(token_attr, env))

#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_GET_QNAME(token_attr, env) \
      (((axis2_woden_qname_or_token_any_attr_t *) token_attr)->ops->\
         get_qname(token_attr, env))

#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_GET_TOKEN(token_attr, env) \
      (((axis2_woden_qname_or_token_any_attr_t *) token_attr)->ops->\
         get_token(token_attr, env))

#define AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_CONVERT(token_attr, env) \
      (((axis2_woden_qname_or_token_any_attr_t *) token_attr)->ops->\
         convert(token_attr, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR_H */
