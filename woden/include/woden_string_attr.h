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

#ifndef WODEN_STRING_ATTR_H
#define WODEN_STRING_ATTR_H

/**
 * @file woden_string_attr.h
 * @brief Axis2 String Attribute Interface
 *          This class represents XML attribute information items of type xs:string.
 */

#include <woden_xml_attr.h>
#include <woden/woden.h>

/** @defgroup woden_string_attr String Attribute
  * @ingroup axis2_wsdl
  * @{
  */

typedef struct woden_string_attr woden_string_attr_t;
typedef struct woden_string_attr_ops woden_string_attr_ops_t;
struct axiom_element;
struct axiom_node;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_string_attr_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *string_attr,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_string_attr_free) (
            void *string_attr,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *string_attr,
            const axis2_env_t *env);

    /**
     * @return the base implementation class
     */
    woden_xml_attr_t *(AXIS2_CALL *
    get_base_impl) (
            void *string_attr,
            const axis2_env_t *env);

    /* ************************************************************
     *  QNameAttr interface declared methods 
     * ************************************************************/

    axis2_char_t *(AXIS2_CALL *
    get_string) (
            void *string_attr,
            const axis2_env_t *env);

    /* ************************************************************
     *  Non-API implementation methods 
     * ************************************************************/
  
    /*
     * Convert a string of type xs:string to a axis2_char_t.
     * A null argument will return a null value.
     * Any conversion error will be reported and a null value will be returned.
     */
    void *(AXIS2_CALL *
    convert) (
            void *string_attr,
            const axis2_env_t *env,
            struct axiom_element *owner_el,
            struct axiom_node *owner_node,
            axis2_char_t *attr_value);

  
};

struct woden_string_attr
{
    woden_xml_attr_t base;
    woden_string_attr_ops_t *ops;
};

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_EXTERN woden_string_attr_t * AXIS2_CALL
woden_string_attr_create(
        const axis2_env_t *env,
        struct axiom_element *owner_el,
        struct axiom_node *owner_node,
        axis2_qname_t *attr_type,
        axis2_char_t *attr_value);


/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_string_attr_resolve_methods(
        woden_string_attr_t *string_attr,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_STRING_ATTR_FREE(string_attr, env) \
      (((woden_string_attr_t *) string_attr)->ops->\
         free(string_attr, env))

#define WODEN_STRING_ATTR_TYPE(string_attr, env) \
      (((woden_string_attr_t *) string_attr)->ops->\
         type(string_attr, env))

#define WODEN_STRING_ATTR_GET_BASE_IMPL(string_attr, env) \
      (((woden_string_attr_t *) string_attr)->ops->\
         get_base_impl(string_attr, env))

#define WODEN_STRING_ATTR_GET_STRING(string_attr, env) \
      (((woden_string_attr_t *) string_attr)->ops->\
         get_string(string_attr, env))

#define WODEN_STRING_ATTR_CONVERT(string_attr, env) \
      (((woden_string_attr_t *) string_attr)->ops->\
         convert(string_attr, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_STRING_ATTR_H */
