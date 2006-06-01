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

#ifndef AXIS2_WODEN_URI_ATTR_H
#define AXIS2_WODEN_URI_ATTR_H

/**
 * @file axis2_woden_uri_attr.h
 * @brief Axis2 URI Attribute Interface
 *          This class represents XML attribute information items of type xs:anyURI.
 */

#include <woden/axis2_woden.h>
#include <woden/xml/axis2_woden_xml_attr.h>

/** @defgroup axis2_woden_uri_attr URI Attribute
  * @ingroup axis2_wsdl
  * @{
  */

typedef struct axis2_woden_uri_attr axis2_woden_uri_attr_t;
typedef struct axis2_woden_uri_attr_ops axis2_woden_uri_attr_ops_t;
struct axis2_om_element;
struct axis2_om_node;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_uri_attr_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *uri_attr,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_uri_attr_free) (
            void *uri_attr,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *uri_attr,
            const axis2_env_t *env);

    /**
     * @return the base implementation class
     */
    axis2_woden_xml_attr_t *(AXIS2_CALL *
    get_base_impl) (
            void *uri_attr,
            const axis2_env_t *env);

    /* ************************************************************
     *  QNameAttr interface declared methods 
     * ************************************************************/

    struct axis2_url *(AXIS2_CALL *
    get_uri) (
            void *uri_attr,
            const axis2_env_t *env);

    /* ************************************************************
     *  Non-API implementation methods 
     * ************************************************************/
    
    /*
     * Convert a string of type xs:anyURI to a axis2_url_t.
     * An empty string argument will return an empty string URI.
     * A null argument will return a null value.
     * Any conversion error will be reported and a null value will be returned.
     */
    void *(AXIS2_CALL *
    convert) (
            void *uri_attr,
            const axis2_env_t *env,
            struct axis2_om_element *owner_el,
            struct axis2_om_node *owner_node,
            axis2_char_t *attr_value);

  
};

struct axis2_woden_uri_attr
{
    axis2_woden_xml_attr_t base;
    axis2_woden_uri_attr_ops_t *ops;
};

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_EXTERN axis2_woden_uri_attr_t * AXIS2_CALL
axis2_woden_uri_attr_create(
        const axis2_env_t *env,
        struct axis2_om_element *owner_el,
        struct axis2_om_node *owner_node,
        axis2_qname_t *attr_type,
        axis2_char_t *attr_value);


/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_uri_attr_resolve_methods(
        axis2_woden_uri_attr_t *uri_attr,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_URI_ATTR_FREE(uri_attr, env) \
		(((axis2_woden_uri_attr_t *) uri_attr)->ops->free(uri_attr, env))

#define AXIS2_WODEN_URI_ATTR_TO_URI_ATTR_FREE(uri_attr, env) \
		(((axis2_woden_uri_attr_t *) uri_attr)->ops->\
         to_uri_attr_free(uri_attr, env))

#define AXIS2_WODEN_URI_ATTR_TYPE(uri_attr, env) \
		(((axis2_woden_uri_attr_t *) uri_attr)->ops->type(uri_attr, env))

#define AXIS2_WODEN_URI_ATTR_GET_BASE_IMPL(uri_attr, env) \
		(((axis2_woden_uri_attr_t *) uri_attr)->ops->get_base_impl(uri_attr, \
                                                                        env))

#define AXIS2_WODEN_URI_ATTR_GET_URI(uri_attr, env) \
		(((axis2_woden_uri_attr_t *) uri_attr)->ops->get_uri(uri_attr, \
                                                                    env))

#define AXIS2_WODEN_URI_ATTR_CONVERT(uri_attr, env) \
		(((axis2_woden_uri_attr_t *) uri_attr)->ops->convert(uri_attr, \
                                                                       env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_URI_ATTR_H */
