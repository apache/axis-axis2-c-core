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

#ifndef AXIS2_WODEN_EXT_REGISTRY_H
#define AXIS2_WODEN_EXT_REGISTRY_H

/**
 * @file axis2_woden_ext_registry.h
 * @brief Axis2 Extension Registry Interface
 * 
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <axis2_array_list.h>
#include <woden/axis2_woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_ext_registry axis2_woden_ext_registry_t;
typedef struct axis2_woden_ext_registry_ops axis2_woden_ext_registry_ops_t;
struct axis2_woden_wsdl_element;
    
/** @defgroup axis2_woden_ext_registry Extension Registry
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_ext_registry_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *ext_registry,
            const axis2_env_t *env);
 
    /**
    * Declare that the specified deserializer should be used to deserialize
    * all extensibility elements with a qname matching elementQN, when
    * encountered as immediate children of the element represented by the
    * specified parentType.
    *
    * @param parent type a class object indicating where in the WSDL
    * document this extensibility element was encountered. For
    * example, axis2_woden_binding would be used to indicate
    * this element was encountered as an immediate child of
    * a &lt;wsdl:binding&gt; element.
    * @param element QN the qname of the extensibility element
    * @param ed the extension deserializer to use
    *
    */
    axis2_status_t (AXIS2_CALL *
    register_deserializer) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_type,
            axis2_qname_t *element_qtype,
            void *ed);

    /**
    * Look up the deserializer for the extensibility element with the
    * qname element QN, which was encountered as an immediate child
    * of the element represented by the specified parentType.
    *
    * @param parent type a class object indicating where in the WSDL
    * document this extensibility element was encountered. For
    * example, axis2_woden_binding would be used to indicate
    * this element was encountered as an immediate child of
    * a &lt;wsdl:binding&gt; element.
    * @param element QN the qname of the extensibility element
    *
    * @return the extension deserializer, if one was found. If none was
    * found. TODO Decide what to do if no deserializer found
    *
    */
    void *(AXIS2_CALL *
    query_deserializer) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_type,
            axis2_qname_t *element_type);

    /**
    * Look up the type of the extensibility element with the specified qname, which
    * was defined as a child of the element represented by the specified parent class.
    *
    * @param parent type a class object indicating where in the WSDL
    * document this extensibility attribute was encountered. For
    * example, axis2_woden_binding would be used to indicate
    * this attribute was defined on a &lt;wsdl:binding> element.
    * @param attr name the qname of the extensibility attribute
    *
    * @return one of the constants defined on the Attribute Extensible class
    *
    */
    void *(AXIS2_CALL *
    query_ext_element_type) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_class,
            axis2_qname_t *elem_qn);

    /**
    * Returns a set of QNames representing the extensibility elements
    * that are allowed as children of the specified parent type.
    * Basically, this method returns the keys associated with the set
    * of extension deserializers registered for this parent type.
    * Returns null if no extension deserializers are registered for
    * this parent type.
    */
    axis2_array_list_t *(AXIS2_CALL *
    get_allowable_exts) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_type);

    /**
    * Declare that the specified extensionType is the concrete
    * class which should be used to represent extensibility elements
    * with qnames matching elementQN, that are intended to exist as
    * children of the specified parentType.
    *
    * @param parentType a class object indicating where in the WSDL
    * definition this extension would exist. For example,
    * axis2_woden_binding would be used to indicate
    * this extensibility element would be added to the list of
    * extensibility elements belonging to a javax.wsdl.Binding,
    * after being instantiated.
    * @param element QN the qname of the extensibility element
    * @param extension type the concrete class which should be instantiated
    *
    */
    axis2_status_t (AXIS2_CALL *
    register_ext_element_type) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_type,
            axis2_qname_t *element_qtype,
            void *element);


    /**
    * Declare that the type of the specified extension attribute, when it occurs
    * as an attribute of the specified parent type, should be assumed to be
    * attrType.
    *
    * @param parent_type a class object indicating where in the WSDL
    * document this extensibility attribute was encountered. For
    * example, axis2_woden_bindin would be used to indicate
    * this attribute was defined on a &lt;wsdl:binding> element.
    * @param attr_qname the qname of the extensibility attribute
    * @param attr_type one of the constants defined on the Attribute Extensible
    * class
    */
    axis2_status_t (AXIS2_CALL *
    register_ext_attr_type) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *owner_class,
            axis2_qname_t *attr_qname,
            void *attr);

    /**
    * Look up the type of the extensibility attribute with the specified qname,
    * which was defined on an element represented by the specified parent class.
    *
    * @param parentType a class object indicating where in the WSDL
    * document this extensibility attribute was encountered. For
    * example, axis2_woden_binding would be used to indicate
    * this attribute was defined on a &lt;wsdl:binding> element.
    * @param attr_qname the qname of the extensibility attribute
    *
    * @return one of the constants defined on the Attribute Extensible class
    */
    void *(AXIS2_CALL *
    query_ext_attr_type) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_class,
            axis2_qname_t *attr_qn);

    /**
    * Register the object that will represent extensions from a specified 
    * namespace that will extend the specified WSDL component class.
    * 
    * @param parent_class the WSDL component class
    * @param ext_namespace the extension namespace
    * @param comp_ext_class the obj representing these extensions
    */
    axis2_status_t (AXIS2_CALL *
    register_component_ext) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_class,
            axis2_url_t *ext_namespc,
            void *comp_ext);

    /**
    * Return the object that represents the extensions from the specified
    * namespace that extend the specified WSDL component class.
    * 
    * @param parent_class the WSDL component
    * @param ext_namespace the extension namespace
    * @return the object of the component extensions
    */
    void *(AXIS2_CALL *
    query_component_ext) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_class,
            axis2_url_t *ext_namespc);

    /**
    * Return the extension namespaces registered for the specified WSDL Component class.
    * 
    * @param parentClass the class of WSDL component extended by these namespaces
    * @return an array of namespace URIs
    */
    axis2_array_list_t *(AXIS2_CALL *
    query_component_ext_namespaces) (
            void *registry,
            const axis2_env_t *env,
            axis2_char_t *parent_class);
};

struct axis2_woden_ext_registry
{
    axis2_woden_ext_registry_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_ext_registry_t * AXIS2_CALL
axis2_woden_ext_registry_create(const axis2_env_t *env);

#define AXIS2_WODEN_EXT_REGISTRY_FREE(ext_registry, env) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         free (ext_registry, env))

#define AXIS2_WODEN_EXT_REGISTRY_REGISTER_DESERIALIZER(ext_registry, env, parent_type, element_qtype, ed) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         register_deserializer (ext_registry, env, parent_type, element_qtype, ed))

#define AXIS2_WODEN_EXT_REGISTRY_QUERY_DESERIALIZER(ext_registry, env, parent_type, element_type) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         query_deserializer (ext_registry, env, parent_type, element_type))

#define AXIS2_WODEN_EXT_REGISTRY_QUERY_EXT_ELEMENT_TYPE(ext_registry, env, parent_class, elem_qn) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         query_ext_element_type (ext_registry, env, parent_class, elem_qn))

#define AXIS2_WODEN_EXT_REGISTRY_GET_ALLOWABLE_EXTS(ext_registry, env, parent_type) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         get_allowable_exts (ext_registry, env, parent_type))

#define AXIS2_WODEN_EXT_REGISTRY_REGISTER_EXT_ELEMENT_TYPE(ext_registry, env, parent_type, element_qtype, element) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         register_ext_element_type (ext_registry, env, parent_type, element_qtype, element))

#define AXIS2_WODEN_EXT_REGISTRY_REGISTER_EXT_ATTR_TYPE(ext_registry, env, owner_class, attr_qname, attr) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         register_ext_attr_type (ext_registry, env, owner_class, attr_qname, attr))

#define AXIS2_WODEN_EXT_REGISTRY_QUERY_EXT_ATTR_TYPE(ext_registry, env, parent_class, attr_qn) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         query_ext_attr_type (ext_registry, env, parent_class, attr_qn))

#define AXIS2_WODEN_EXT_REGISTRY_REGISTER_COMPONENT_EXT(ext_registry, env, parent_class, ext_namespc, comp_ext) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         register_component_ext (ext_registry, env, parent_class, ext_namespc, comp_ext))

#define AXIS2_WODEN_EXT_REGISTRY_QUERY_COMPONENT_EXT(ext_registry, env, parent_class, ext_namespc) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         query_component_ext (ext_registry, env, parent_class, ext_namespc))

#define AXIS2_WODEN_EXT_REGISTRY_QUERY_COMPONENT_EXT_NAMESPACES(ext_registry, env, parent_class) \
      (((axis2_woden_ext_registry_t *) ext_registry)->ops->\
         query_component_ext_namespaces (ext_registry, env, parent_class))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_EXT_REGISTRY_H */
