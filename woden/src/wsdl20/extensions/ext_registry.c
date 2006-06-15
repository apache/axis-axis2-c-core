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
 
#include <woden_ext_registry.h>
#include <woden_component_exts.h>

#include <woden_wsdl_element.h>

#include <woden_soap_module_deserializer.h>
#include <woden_soap_header_block_deserializer.h>
#include <woden_soap_module.h>
#include <woden_soap_header_block.h>
#include <woden_soap_binding_exts.h>
#include <woden_soap_binding_fault_exts.h>
#include <woden_soap_binding_op_exts.h>
#include <woden_soap_binding_fault_ref_exts.h>
#include <woden_soap_binding_msg_ref_exts.h>

#include "soap/woden_soap_constants.h"

#include <woden_string_attr.h>
#include <woden_uri_attr.h>
#include <woden_qname_or_token_any_attr.h>
#include <woden_qname_list_or_token_any_attr.h>
#include <axis2_uri.h>
#include <axis2_hash.h>

typedef struct woden_ext_registry_impl woden_ext_registry_impl_t;

/** 
 * @brief Extension Registry Struct Impl
 *   Axis2 Extension Registry  
 */ 
struct woden_ext_registry_impl
{
    woden_ext_registry_t registry;
    /*
    This is a Map of Maps. The top-level Map is keyed by parent type,
    and the inner Maps are keyed by element QN.
    */
    axis2_hash_t *deserializer_reg;
    /*
    This is a Map of Maps. The top-level Map is keyed by parent type,
    and the inner Maps are keyed by element QN.
    */
    axis2_hash_t *ext_element_reg;
    /*
    This is a Map of Maps. The top-level Map is keyed by parent type,
    and the inner Maps are keyed by attr qname.
    */
    axis2_hash_t *ext_attr_reg;

    /*
    * This is a Map of Maps. The top-level Map is keyed by parent component
    * and the inner Map is keyed by (URI)extension namespace with a value of
    * component extensions.
    */
    axis2_hash_t *comp_ext_reg;

    axis2_array_list_t *key_set;

    woden_string_attr_t *string_attr;
    woden_uri_attr_t *uri_attr;
    woden_qname_or_token_any_attr_t *qname_or_token_any_attr;
    woden_qname_list_or_token_any_attr_t *qname_list_or_token_any_attr; 
    woden_soap_module_deserializer_t *soap_module_deser; 
    woden_soap_module_t *soap_module;
    woden_soap_header_block_deserializer_t *soap_header_block_deser; 
    woden_soap_header_block_t *soap_header_block;
    woden_soap_binding_exts_t *soap_binding_ext;
    woden_soap_binding_fault_exts_t *soap_binding_fault_ext;
    woden_soap_binding_op_exts_t *soap_binding_op_ext;
    woden_soap_binding_msg_ref_exts_t *soap_binding_msg_ref_ext;
    woden_soap_binding_fault_ref_exts_t *soap_binding_fault_ref_ext;
};

#define INTF_TO_IMPL(registry) ((woden_ext_registry_impl_t *) registry)

axis2_status_t AXIS2_CALL 
woden_ext_registry_free(
        void *registry,
        const axis2_env_t *envv);

axis2_status_t AXIS2_CALL
woden_ext_registry_register_deserializer(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        axis2_qname_t *element_qtype,
        void *ed);

void *AXIS2_CALL
woden_ext_registry_query_deserializer(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        axis2_qname_t *element_type);

void *AXIS2_CALL
woden_ext_registry_query_ext_element_type(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class,
        axis2_qname_t *elem_qn);

axis2_array_list_t *AXIS2_CALL
woden_ext_registry_get_allowable_exts(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_type);

axis2_status_t AXIS2_CALL
woden_ext_registry_register_ext_element_type(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        axis2_qname_t *element_qtype,
        void *element);

axis2_status_t AXIS2_CALL
woden_ext_registry_register_ext_attr_type(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *owner_class,
        axis2_qname_t *attr_qname,
        void *attr);

void *AXIS2_CALL
woden_ext_registry_query_ext_attr_type(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class,
        axis2_qname_t *attr_qn);

axis2_status_t AXIS2_CALL
woden_ext_registry_register_component_ext(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class,
        axis2_uri_t *ext_namespc,
        void *comp_ext);

void *AXIS2_CALL
woden_ext_registry_query_component_ext(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class,
        axis2_uri_t *ext_namespc);

axis2_array_list_t *AXIS2_CALL
woden_ext_registry_query_component_ext_namespaces(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class);

static axis2_status_t
woden_ext_registry_populate(
        void *registry,
        const axis2_env_t *env);

AXIS2_EXTERN woden_ext_registry_t * AXIS2_CALL
woden_ext_registry_create(
        const axis2_env_t *env)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_ext_registry_impl_t));

    registry_impl->deserializer_reg = NULL;
    registry_impl->ext_element_reg = NULL;
    registry_impl->ext_attr_reg = NULL;
    registry_impl->comp_ext_reg = NULL;
    registry_impl->key_set = NULL;

    registry_impl->string_attr = NULL;
    registry_impl->uri_attr = NULL;
    registry_impl->qname_or_token_any_attr = NULL;
    registry_impl->qname_list_or_token_any_attr = NULL; 
    registry_impl->soap_module_deser = NULL; 
    registry_impl->soap_module = NULL;
    registry_impl->soap_header_block_deser = NULL; 
    registry_impl->soap_header_block = NULL;
    registry_impl->soap_binding_ext = NULL;
    registry_impl->soap_binding_fault_ext = NULL;
    registry_impl->soap_binding_op_ext = NULL;
    registry_impl->soap_binding_msg_ref_ext = NULL;
    registry_impl->soap_binding_fault_ref_ext = NULL;

    registry_impl->registry.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_ext_registry_ops_t)); 
    
    registry_impl->registry.ops->free = 
            woden_ext_registry_free;
    registry_impl->registry.ops->register_deserializer = 
            woden_ext_registry_register_deserializer;
    registry_impl->registry.ops->query_deserializer = 
            woden_ext_registry_query_deserializer;
    registry_impl->registry.ops->query_ext_element_type = 
            woden_ext_registry_query_ext_element_type;
    registry_impl->registry.ops->get_allowable_exts = 
            woden_ext_registry_get_allowable_exts;
    registry_impl->registry.ops->register_ext_element_type = 
            woden_ext_registry_register_ext_element_type;
    registry_impl->registry.ops->register_ext_attr_type = 
            woden_ext_registry_register_ext_attr_type;
    registry_impl->registry.ops->query_ext_attr_type = 
            woden_ext_registry_query_ext_attr_type;
    registry_impl->registry.ops->register_component_ext = 
            woden_ext_registry_register_component_ext;
    registry_impl->registry.ops->query_component_ext = 
            woden_ext_registry_query_component_ext;
    registry_impl->registry.ops->query_component_ext_namespaces = 
            woden_ext_registry_query_component_ext_namespaces;
    
    registry_impl->ext_attr_reg = axis2_hash_make(env);
    if(!registry_impl->ext_attr_reg)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    registry_impl->deserializer_reg = axis2_hash_make(env);
    if(!registry_impl->deserializer_reg)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    woden_ext_registry_populate(&(registry_impl->registry), env);
    
    return &(registry_impl->registry);
}

axis2_status_t AXIS2_CALL
woden_ext_registry_free(
        void *registry,
        const axis2_env_t *env)
{
    woden_ext_registry_impl_t *registry_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    if(registry_impl->deserializer_reg)
    {
        /* TODO */
    }
    
    if(registry_impl->ext_element_reg)
    {
        /* TODO */
    }
    
    if(registry_impl->ext_attr_reg)
    {
        /* TODO */
    }
    
    if(registry_impl->comp_ext_reg)
    {
        /* TODO */
    }
    
    if((&(registry_impl->registry))->ops)
    {
        AXIS2_FREE(env->allocator, (&(registry_impl->registry))->ops);
        (&(registry_impl->registry))->ops = NULL;
    }

    if(registry_impl)
    {
        AXIS2_FREE(env->allocator, registry_impl);
        registry_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

/**
* Declare that the specified deserializer should be used to deserialize
* all extensibility elements with a qname matching elementQN, when
* encountered as immediate children of the element represented by the
* specified parentType.
*
* @param parent type a class object indicating where in the WSDL
* document this extensibility element was encountered. For
* example, woden_binding would be used to indicate
* this element was encountered as an immediate child of
* a &lt;wsdl:binding&gt; element.
* @param element QN the qname of the extensibility element
* @param ed the extension deserializer to use
*
*/
axis2_status_t AXIS2_CALL
woden_ext_registry_register_deserializer(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        axis2_qname_t *element_qtype,
        void *ed)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_deserializer_reg = NULL;
    axis2_char_t *element_type = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_deserializer_reg = axis2_hash_get(registry_impl->deserializer_reg, 
            parent_type, AXIS2_HASH_KEY_STRING);
    if(NULL == inner_deserializer_reg)
    {
        inner_deserializer_reg = axis2_hash_make(env);
        axis2_hash_set(registry_impl->deserializer_reg, parent_type, 
                AXIS2_HASH_KEY_STRING, inner_deserializer_reg);
    }
    element_type = AXIS2_QNAME_TO_STRING(element_qtype, env);
    axis2_hash_set(inner_deserializer_reg, element_type, AXIS2_HASH_KEY_STRING, 
            ed);
    return AXIS2_SUCCESS;
}

/**
* Look up the deserializer for the extensibility element with the
* qname element QN, which was encountered as an immediate child
* of the element represented by the specified parentType.
*
* @param parent type a class object indicating where in the WSDL
* document this extensibility element was encountered. For
* example, woden_binding would be used to indicate
* this element was encountered as an immediate child of
* a &lt;wsdl:binding&gt; element.
* @param element QN the qname of the extensibility element
*
* @return the extension deserializer, if one was found. If none was
* found. TODO Decide what to do if no deserializer found
*
*/
void *AXIS2_CALL
woden_ext_registry_query_deserializer(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        axis2_qname_t *element_type)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_deserializer_reg = NULL;
    axis2_char_t *elem_name = NULL;
    void *ed = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_deserializer_reg = axis2_hash_get(registry_impl->deserializer_reg, 
            parent_type, AXIS2_HASH_KEY_STRING);
    elem_name = AXIS2_QNAME_TO_STRING(element_type, env);
    if(NULL != inner_deserializer_reg)
    {
        ed = axis2_hash_get(inner_deserializer_reg, elem_name, 
                AXIS2_HASH_KEY_STRING);
    }
    return ed;
}

/**
* Look up the type of the extensibility element with the specified qname, which
* was defined as a child of the element represented by the specified parent class.
*
* @param parent type a class object indicating where in the WSDL
* document this extensibility attribute was encountered. For
* example, woden_binding would be used to indicate
* this attribute was defined on a &lt;wsdl:binding> element.
* @param attr name the qname of the extensibility attribute
*
* @return one of the constants defined on the Attribute Extensible class
*
*/
void *AXIS2_CALL
woden_ext_registry_query_ext_element_type(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class,
        axis2_qname_t *elem_qn)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_ext_attr_reg = NULL;
    axis2_char_t *elem_name = NULL;
    void *element = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_ext_attr_reg = axis2_hash_get(registry_impl->ext_attr_reg, 
            parent_class, AXIS2_HASH_KEY_STRING);
    elem_name = AXIS2_QNAME_TO_STRING(elem_qn, env);
    if(NULL != inner_ext_attr_reg)
    {
        element = axis2_hash_get(inner_ext_attr_reg, elem_name, 
                AXIS2_HASH_KEY_STRING);
    }
    return element;
}

/**
* Returns a set of QNames representing the extensibility elements
* that are allowed as children of the specified parent type.
* Basically, this method returns the keys associated with the set
* of extension deserializers registered for this parent type.
* Returns null if no extension deserializers are registered for
* this parent type.
*/
axis2_array_list_t *AXIS2_CALL
woden_ext_registry_get_allowable_exts(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_type)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_deserializer_reg = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_deserializer_reg = axis2_hash_get(registry_impl->deserializer_reg, 
            parent_type, AXIS2_HASH_KEY_STRING); 
    if(NULL != inner_deserializer_reg)
    {
        axis2_hash_index_t *i = NULL;

        registry_impl->key_set = axis2_array_list_create(env, 0);
        for (i = axis2_hash_first (inner_deserializer_reg, env); i; i = 
                axis2_hash_next (env, i))
        {
            void *v = NULL;

            axis2_hash_this (i, NULL, NULL, &v);
            AXIS2_ARRAY_LIST_ADD(registry_impl->key_set, env, v);
        }
    }
    return registry_impl->key_set;
}

/**
* Declare that the specified extensionType is the concrete
* class which should be used to represent extensibility elements
* with qnames matching elementQN, that are intended to exist as
* children of the specified parentType.
*
* @param parentType a class object indicating where in the WSDL
* definition this extension would exist. For example,
* woden_binding would be used to indicate
* this extensibility element would be added to the list of
* extensibility elements belonging to a javax.wsdl.Binding,
* after being instantiated.
* @param element QN the qname of the extensibility element
* @param extension type the concrete class which should be instantiated
*
*/
axis2_status_t AXIS2_CALL
woden_ext_registry_register_ext_element_type(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_type,
        axis2_qname_t *element_qtype,
        void *element)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_ext_type_reg = NULL;
    axis2_char_t *element_type = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_ext_type_reg = axis2_hash_get(registry_impl->ext_element_reg, 
            parent_type, AXIS2_HASH_KEY_STRING);
    if(NULL == inner_ext_type_reg)
    {
        inner_ext_type_reg = axis2_hash_make(env);
        axis2_hash_set(registry_impl->ext_element_reg, parent_type, 
                AXIS2_HASH_KEY_STRING, inner_ext_type_reg);
    }
    element_type = AXIS2_QNAME_TO_STRING(element_qtype, env);
    axis2_hash_set(inner_ext_type_reg, element_type, AXIS2_HASH_KEY_STRING, 
            element);
    return AXIS2_SUCCESS;
}


/**
* Declare that the type of the specified extension attribute, when it occurs
* as an attribute of the specified parent type, should be assumed to be
* attrType.
*
* @param parent_type a class object indicating where in the WSDL
* document this extensibility attribute was encountered. For
* example, woden_bindin would be used to indicate
* this attribute was defined on a &lt;wsdl:binding> element.
* @param attr_qname the qname of the extensibility attribute
* @param attr_type one of the constants defined on the Attribute Extensible
* class
*/
axis2_status_t AXIS2_CALL
woden_ext_registry_register_ext_attr_type(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *owner_class,
        axis2_qname_t *attr_qname,
        void *attr)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_ext_attr_reg = NULL;
    axis2_char_t *attr_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_ext_attr_reg = axis2_hash_get(registry_impl->ext_attr_reg, 
            owner_class, AXIS2_HASH_KEY_STRING);
    if(NULL == inner_ext_attr_reg)
    {
        inner_ext_attr_reg = axis2_hash_make(env);
        axis2_hash_set(registry_impl->ext_attr_reg, owner_class, 
                AXIS2_HASH_KEY_STRING, inner_ext_attr_reg);
    }
    attr_name = AXIS2_QNAME_TO_STRING(attr_qname, env);
    axis2_hash_set(inner_ext_attr_reg, attr_name, AXIS2_HASH_KEY_STRING, attr);
    return AXIS2_SUCCESS;
}

/**
* Look up the type of the extensibility attribute with the specified qname,
* which was defined on an element represented by the specified parent class.
*
* @param parentType a class object indicating where in the WSDL
* document this extensibility attribute was encountered. For
* example, woden_binding would be used to indicate
* this attribute was defined on a &lt;wsdl:binding> element.
* @param attr_qname the qname of the extensibility attribute
*
* @return one of the constants defined on the Attribute Extensible class
*/
void *AXIS2_CALL
woden_ext_registry_query_ext_attr_type(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class,
        axis2_qname_t *attr_qn)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_ext_attr_reg = NULL;
    void *attr = NULL;
    axis2_char_t *attr_name = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_ext_attr_reg = axis2_hash_get(registry_impl->ext_attr_reg, 
            parent_class, AXIS2_HASH_KEY_STRING);
    attr_name = AXIS2_QNAME_TO_STRING(attr_qn, env);
    if(NULL != inner_ext_attr_reg)
    {
        attr = axis2_hash_get(inner_ext_attr_reg, attr_name, 
                AXIS2_HASH_KEY_STRING);
    }
    return attr;
}

/**
* Register the object that will represent extensions from a specified 
* namespace that will extend the specified WSDL component class.
* 
* @param parent_class the WSDL component class
* @param ext_namespace the extension namespace
* @param comp_ext_class the obj representing these extensions
*/
axis2_status_t AXIS2_CALL
woden_ext_registry_register_component_ext(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class,
        axis2_uri_t *ext_namespc,
        void *comp_ext)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_comp_ext_reg = NULL;
    axis2_char_t *namespc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_comp_ext_reg = axis2_hash_get(registry_impl->comp_ext_reg, 
            parent_class, AXIS2_HASH_KEY_STRING);
    if(NULL == inner_comp_ext_reg)
    {
        inner_comp_ext_reg = axis2_hash_make(env);
        axis2_hash_set(registry_impl->comp_ext_reg, parent_class, 
                AXIS2_HASH_KEY_STRING, inner_comp_ext_reg);
    }
    namespc = AXIS2_URI_TO_STRING(ext_namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
    axis2_hash_set(inner_comp_ext_reg, namespc, AXIS2_HASH_KEY_STRING, comp_ext);
    return AXIS2_SUCCESS;
}

/**
* Return the object that represents the extensions from the specified
* namespace that extend the specified WSDL component class.
* 
* @param parent_class the WSDL component
* @param ext_namespace the extension namespace
* @return the object of the component extensions
*/
void *AXIS2_CALL
woden_ext_registry_query_component_ext(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class,
        axis2_uri_t *ext_namespc)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_comp_ext_reg = NULL;
    void *comp_ext = NULL;
    axis2_char_t *namespc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_comp_ext_reg = axis2_hash_get(registry_impl->comp_ext_reg, 
            parent_class, AXIS2_HASH_KEY_STRING);
    namespc = AXIS2_URI_TO_STRING(ext_namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
    if(NULL != inner_comp_ext_reg)
    {
        comp_ext = axis2_hash_get(inner_comp_ext_reg, namespc, 
                AXIS2_HASH_KEY_STRING);
    }
    return comp_ext;
}

/**
* Return the extension namespaces registered for the specified WSDL Component class.
* 
* @param parentClass the class of WSDL component extended by these namespaces
* @return an array of namespace URIs
*/
axis2_array_list_t *AXIS2_CALL
woden_ext_registry_query_component_ext_namespaces(
        void *registry,
        const axis2_env_t *env,
        axis2_char_t *parent_class)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_hash_t *inner_comp_ext_reg = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    inner_comp_ext_reg = axis2_hash_get(registry_impl->comp_ext_reg, 
            parent_class, AXIS2_HASH_KEY_STRING);
    registry_impl->key_set = axis2_array_list_create(env, 0);
    if(NULL != inner_comp_ext_reg)
    {
        axis2_hash_index_t *i = NULL;

        for (i = axis2_hash_first (inner_comp_ext_reg, env); i; i = 
                axis2_hash_next (env, i))
        {
            const void *key = NULL;

            axis2_hash_this (i, &key, NULL, NULL);
            AXIS2_ARRAY_LIST_ADD(registry_impl->key_set, env, key);
        }
    }
    return registry_impl->key_set;
}

static axis2_status_t
woden_ext_registry_populate(
        void *registry,
        const axis2_env_t *env)
{
    woden_ext_registry_impl_t *registry_impl = NULL;
    axis2_qname_t *q_attr_soap_version = NULL;
    axis2_qname_t *q_attr_soap_protocol = NULL;
    axis2_qname_t *q_attr_soap_mepdefault = NULL;
    axis2_qname_t *q_attr_soap_code = NULL;
    axis2_qname_t *q_attr_soap_subcodes = NULL;
    axis2_qname_t *q_attr_soap_mep = NULL;
    axis2_qname_t *q_attr_soap_action = NULL;
    axis2_qname_t *q_elem_soap_module = NULL;
    axis2_qname_t *q_elem_soap_header = NULL;
    axis2_uri_t *uri_ns_soap = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    registry_impl = INTF_TO_IMPL(registry);

    /*********************** SOAP extension attributes ************************/
   
    registry_impl->string_attr = woden_string_attr_create(env, NULL, NULL, 
            NULL, NULL);
    registry_impl->uri_attr = woden_uri_attr_create(env, NULL, NULL, NULL, 
            NULL);
    registry_impl->qname_or_token_any_attr = 
        woden_qname_or_token_any_attr_create(env, NULL, NULL, NULL, NULL);
    registry_impl->qname_list_or_token_any_attr = 
        woden_qname_list_or_token_any_attr_create(env, NULL, NULL, NULL, 
                NULL);

    q_attr_soap_version = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_VERSION);
    woden_ext_registry_register_ext_attr_type(registry, env,
        "binding_element", q_attr_soap_version, 
        registry_impl->string_attr);
   
    q_attr_soap_protocol = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_PROTOCOL);
    woden_ext_registry_register_ext_attr_type(registry, env,
        "binding_element", q_attr_soap_protocol, 
        registry_impl->uri_attr);
   
    q_attr_soap_mepdefault = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_MEPDEFAULT);
    woden_ext_registry_register_ext_attr_type(registry, env,
        "binding_element", q_attr_soap_mepdefault, 
        registry_impl->uri_attr);
   
    q_attr_soap_code = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_CODE);
    woden_ext_registry_register_ext_attr_type(registry, env, 
        "binding_fault_element", q_attr_soap_code, 
        registry_impl->qname_or_token_any_attr);
   
    q_attr_soap_subcodes = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_SUBCODES);
    woden_ext_registry_register_ext_attr_type(registry, env, 
        "binding_fault_element", q_attr_soap_subcodes, 
        registry_impl->qname_list_or_token_any_attr);
   
    q_attr_soap_mep = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_MEP);
    woden_ext_registry_register_ext_attr_type(registry, env, 
        "binding_op_element", q_attr_soap_mep, 
        registry_impl->uri_attr);
   
    q_attr_soap_action = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_ACTION);
    woden_ext_registry_register_ext_attr_type(registry, env, 
        "binding_op_element", q_attr_soap_action, 
        registry_impl->uri_attr);
    
    /************** SOAPModule extension elements *****************************/

    registry_impl->soap_module_deser = 
        woden_soap_module_deserializer_create(env);
    registry_impl->soap_module = woden_soap_module_create(env);
    
    q_elem_soap_module = axis2_qname_create_from_string(env, WODEN_Q_ELEM_SOAP_MODULE);
    
    woden_ext_registry_register_deserializer(registry, env, "binding_element",
            q_elem_soap_module,
            registry_impl->soap_module_deser);
    
    woden_ext_registry_register_ext_element_type(registry, env, "binding_element",
            q_elem_soap_module,
            registry_impl->soap_module);
    
    woden_ext_registry_register_deserializer(registry, env, "binding_fault_element",
            q_elem_soap_module,
            registry_impl->soap_module_deser);
    
    woden_ext_registry_register_ext_element_type(registry, env, "binding_fault_element",
            q_elem_soap_module,
            registry_impl->soap_module);
    
    woden_ext_registry_register_deserializer(registry, env, "binding_op_element",
            q_elem_soap_module,
            registry_impl->soap_module_deser);
    
    woden_ext_registry_register_ext_element_type(registry, env, "binding_op_element",
            q_elem_soap_module,
            registry_impl->soap_module);
    
    woden_ext_registry_register_deserializer(registry, env, "binding_msg_ref_element",
            q_elem_soap_module,
            registry_impl->soap_module_deser);
    
    woden_ext_registry_register_ext_element_type(registry, env, "binding_msg_ref_element",
            q_elem_soap_module,
            registry_impl->soap_module);
    
    woden_ext_registry_register_deserializer(registry, env, "binding_fault_ref_element",
            q_elem_soap_module,
            registry_impl->soap_module_deser);
    
    woden_ext_registry_register_ext_element_type(registry, env, "binding_fault_ref_element",
            q_elem_soap_module,
            registry_impl->soap_module);

    /**************** SOAPHeaderBlock extension elements **********************/
    registry_impl->soap_header_block_deser = 
        woden_soap_header_block_deserializer_create(env);
    registry_impl->soap_header_block = woden_soap_header_block_create(env);
    
    q_elem_soap_header = axis2_qname_create_from_string(env, WODEN_Q_ELEM_SOAP_HEADER);
    
    woden_ext_registry_register_deserializer(registry, env, "binding_fault_element",
            q_elem_soap_header,
            registry_impl->soap_header_block_deser);
    
    woden_ext_registry_register_ext_element_type(registry, env, "binding_fault_element",
            q_elem_soap_header,
            registry_impl->soap_header_block);
    
    woden_ext_registry_register_deserializer(registry, env, "binding_msg_ref_element",
            q_elem_soap_header,
            registry_impl->soap_header_block_deser);
    
    woden_ext_registry_register_ext_element_type(registry, env, "binding_msg_ref_element",
            q_elem_soap_header,
            registry_impl->soap_header_block);
    
    /*************** WSDL Component Extensions *******************************/ 
    registry_impl->soap_binding_ext = woden_soap_binding_exts_create(env);
    registry_impl->soap_binding_fault_ext = 
        woden_soap_binding_fault_exts_create(env);
    registry_impl->soap_binding_op_ext = 
        woden_soap_binding_op_exts_create(env);
    registry_impl->soap_binding_msg_ref_ext = 
        woden_soap_binding_msg_ref_exts_create(env);
    registry_impl->soap_binding_fault_ref_ext = 
        woden_soap_binding_fault_ref_exts_create(env);

    uri_ns_soap = axis2_uri_parse_string(env, WODEN_URI_NS_SOAP);

    woden_ext_registry_register_component_ext(registry, env, "binding",
            uri_ns_soap,
            registry_impl->soap_binding_ext);
    
    woden_ext_registry_register_component_ext(registry, env, "binding_fault",
            uri_ns_soap,
            registry_impl->soap_binding_fault_ext);
    
    woden_ext_registry_register_component_ext(registry, env, "binding_op",
            uri_ns_soap,
            registry_impl->soap_binding_op_ext);
    
    woden_ext_registry_register_component_ext(registry, env, "binding_msg_ref",
            uri_ns_soap,
            registry_impl->soap_binding_msg_ref_ext);
    
    woden_ext_registry_register_component_ext(registry, env, "binding_fault_ref",
            uri_ns_soap,
            registry_impl->soap_binding_fault_ref_ext);
    
    return AXIS2_SUCCESS;
}
