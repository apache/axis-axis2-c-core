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

#ifndef XML_SCHEMA_H
#define XML_SCHEMA_H

/**
 * @file xml_schema.h
 * @brief Axis2 Xml Schema Interface
 *          Contains the definition of a schema. All XML Schema definition 
 *          language (XSD) elements are children of the schema element. 
 *          Represents the World Wide Web Consortium (W3C) schema element
 */

#include <xml_schema_defines.h>
#include <xml_schema_annotated.h>
#include <axiom_element.h>
#include <axiom_node.h>
#include <axis2_hash.h>

/** @defgroup xml_schema Xml Schema
  * @ingroup xml_schema
  * @{
  */



#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema xml_schema_t;
typedef struct xml_schema_ops xml_schema_ops_t;
struct xml_schema_form;
struct xml_schema_obj_table;
struct xml_schema_derivation_method;
struct xml_schema_element;
struct xml_schema_type;
struct xml_schema_obj_collection;
struct xml_schema_collection;

#define XML_SCHEMA_NS "http://www.w3.org/2001/XMLSchema"

struct xml_schema_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free)(
            void *schema,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs)(
            void *schema,
            const axis2_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type)(  void *schema,
            const axis2_env_t *env);
    
    xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl)(
            void *schema,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_namespace)(
            void *schema,
            const axis2_env_t *env,
            axis2_char_t *prefix);

    struct xml_schema_form *(AXIS2_CALL *
    get_attr_form_default)(
            void *schema,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_attr_form_default)(
            void *schema,
            const axis2_env_t *env,
            struct xml_schema_form *value);

    struct xml_schema_obj_table *(AXIS2_CALL *
    get_attr_groups)(
            void *schema,
            const axis2_env_t *env);

    struct xml_schema_obj_table *(AXIS2_CALL *
    get_attrs)(
            void *schema,
            const axis2_env_t *env);

    struct xml_schema_derivation_method *(AXIS2_CALL *
    get_block_default)(
            void *schema,
            const axis2_env_t *env); 

    axis2_status_t (AXIS2_CALL *
    set_block_default)(
            void *schema,
            const axis2_env_t *env,
            struct xml_schema_derivation_method *block_default); 

    struct xml_schema_form *(AXIS2_CALL *
    get_element_form_default)(
            void *schema,
            const axis2_env_t *env); 

    axis2_status_t (AXIS2_CALL *
    set_element_form_default)(
            void *schema,
            const axis2_env_t *env,
            struct xml_schema_form *element_form_default); 

    struct xml_schema_obj_table *(AXIS2_CALL *
    get_elements)(
            void *schema,
            const axis2_env_t *env); 

    struct xml_schema_element *(AXIS2_CALL *
    get_element_by_qname)(
            void *schema,
            const axis2_env_t *env,
            axis2_qname_t *qname); 

    struct xml_schema_type *(AXIS2_CALL *
    get_type_by_qname)(
            void *schema,
            const axis2_env_t *env,
            axis2_qname_t *qname); 

    struct xml_schema_derivation_method *(AXIS2_CALL *
    get_final_default)(
            void *schema,
            const axis2_env_t *env); 

    axis2_status_t (AXIS2_CALL *
    set_final_default)(
            void *schema,
            const axis2_env_t *env,
            struct xml_schema_derivation_method *final_default); 

    struct xml_schema_obj_table *(AXIS2_CALL *
    get_groups)(
            void *schema,
            const axis2_env_t *env); 

    struct xml_schema_obj_collection *(AXIS2_CALL *
    get_includes)(
            void *schema,
            const axis2_env_t *env); 

    axis2_bool_t (AXIS2_CALL *
    is_compiled)(
            void *schema,
            const axis2_env_t *env); 

    struct xml_schema_obj_collection *(AXIS2_CALL *
    get_items)(
            void *schema,
            const axis2_env_t *env); 

    struct xml_schema_obj_table *(AXIS2_CALL *
    get_notations)(
            void *schema,
            const axis2_env_t *env); 

    struct xml_schema_obj_table *(AXIS2_CALL *
    get_schema_types)(
            void *schema,
            const axis2_env_t *env); 

    axis2_char_t *(AXIS2_CALL *
    get_target_namespace)(
            void *schema,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_target_namespace)(
            void *schema,
            const axis2_env_t *env,
            axis2_char_t *target_namespace); 

    axis2_char_t *(AXIS2_CALL *
    get_version)(
            void *schema,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    compile)(void *schema,
             const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    write_with_out)(
            void *schema,
            const axis2_env_t *env,
            void *outstream); 

    axis2_status_t (AXIS2_CALL *
    write_with_writer)(
            void *schema,
            void *writer); 

    axis2_hash_t *(AXIS2_CALL *
    get_prefix_to_namespace_map)(
            void *schema,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_prefix_to_namespace_map)(
            void *schema,
            const axis2_env_t *env,
            axis2_hash_t *map);

    axis2_status_t (AXIS2_CALL *
    add_type)(
            void *schema,
            const axis2_env_t *env,
            struct xml_schema_type *type);
            
    axis2_status_t (AXIS2_CALL *
    set_schema_ns_prefix)(
            void *schema,
            const axis2_env_t *env,
            axis2_char_t *ns_prefix);
            
    axis2_char_t* (AXIS2_CALL*
    serialize)(
            void *schema,
            const axis2_env_t *env);
            
    axis2_status_t (AXIS2_CALL*
    set_root_node)(
            void *schema,
            const axis2_env_t *env,
            axiom_node_t *node);            
            
};

struct xml_schema
{
    xml_schema_annotated_t base;
    xml_schema_ops_t *ops;
};

/**
 * Creates new Xml Schema
 */
AXIS2_EXTERN xml_schema_t * AXIS2_CALL
xml_schema_create(const axis2_env_t *env,
                                axis2_char_t *namespc,
                                struct xml_schema_collection *parent);

#define XML_SCHEMA_FREE(schema, env) \
      (((xml_schema_t *) schema)->ops->\
            free(schema, env))

#define XML_SCHEMA_SUPER_OBJS(schema, env) \
      (((xml_schema_t *) schema)->ops->\
            super_objs(schema, env))

#define XML_SCHEMA_GET_TYPE(schema, env) \
      (((xml_schema_t *) schema)->ops->\
            get_type(schema, env))

#define XML_SCHEMA_GET_BASE_IMPL(schema, env) \
      (((xml_schema_t *) schema)->ops->\
            get_base_impl(schema, env))
            
#define XML_SCHEMA_GET_NAMESPACE(schema, env, prefix) \
        (((xml_schema_t *) schema)->ops->\
            get_namespace(schema, env, prefix))            

#define XML_SCHEMA_GET_ATTR_FORM_DEFAULT(schema, env) \
      (((xml_schema_t *) schema)->ops->\
            get_attr_form_default(schema, env))

#define XML_SCHEMA_SET_ATTR_FORM_DEFAULT(schema, env, attr_form) \
      (((xml_schema_t *) schema)->ops->\
            set_attr_form_default(schema, env, attr_form))

#define XML_SCHEMA_GET_ATTR_GROUPS(schema, env) \
      (((xml_schema_t *) schema)->ops->\
           get_attr_groups(schema, env))
           
#define XML_SCHEMA_GET_ATTRS(schema, env) \
      (((xml_schema_t *) schema)->ops->\
           get_attrs(schema, env))           

#define XML_SCHEMA_GET_BLOCK_DEFAULT(schema, env) \
      (((xml_schema_t *) schema)->ops->\
           get_block_default(schema, env))
           

#define XML_SCHEMA_SET_BLOCK_DEFAULT(schema, env, block_default) \
      (((xml_schema_t *) schema)->ops->\
           set_block_default(schema, env, block_default))
           
#define XML_SCHEMA_GET_ELEMENT_FORM_DEFAULT(schema, env) \
      (((xml_schema_t *) schema)->ops->\
           get_element_form_default(schema, env))

#define XML_SCHEMA_SET_ELEMENT_FORM_DEFAULT(schema, env, ele_form_default) \
      (((xml_schema_t *) schema)->ops->\
           set_element_form_default(schema, env, ele_form_default))
           
#define XML_SCHEMA_GET_ELEMENTS(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           get_elements(schema, env))                         

#define XML_SCHEMA_GET_ELEMENT_BY_QNAME(schema, env, qname) \
        (((xml_schema_t *) schema)->ops->\
           get_element_by_qname(schema, env, qname))  
           
#define XML_SCHEMA_GET_TYPE_BY_QNAME(schema, env, qname) \
        (((xml_schema_t *) schema)->ops->\
           get_type_by_qname(schema, env, qname))                  

#define XML_SCHEMA_SET_FINAL_DEFAULT(schema, env, final_default) \
      (((xml_schema_t *) schema)->ops->\
           set_final_default(schema, env, final_default))

#define XML_SCHEMA_GET_FINAL_DEFAULT(schema, env) \
      (((xml_schema_t *) schema)->ops->\
           get_final_default(schema, env))
           
#define XML_SCHEMA_GET_GROUPS(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           get_groups(schema, env))      
           
           
#define XML_SCHEMA_GET_INCLUDES(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           get_includes(schema, env))                  

#define XML_SCHEMA_IS_COMPILED(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           is_compiled(schema, env)) 

#define XML_SCHEMA_GET_ITEMS(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           get_items(schema, env)) 
           
#define XML_SCHEMA_GET_NOTATIONS(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           get_items(schema, env))  
           
#define XML_SCHEMA_GET_SCHEMA_TYPES(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           get_schema_types(schema, env))            
                               
#define XML_SCHEMA_GET_TARGET_NAMESPACE(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           get_target_namespace(schema, env))    
           
#define XML_SCHEMA_SET_TARGET_NAMESPACE(schema, env, target_ns) \
        (((xml_schema_t *) schema)->ops->\
           set_target_namespace(schema, env, target_ns))                    
           
#define XML_SCHEMA_GET_VERSION(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           get_version(schema, env)) 

#define XML_SCHEMA_COMPILE(schema, env) \
        (((xml_schema_t *) schema)->ops->\
           compile(schema, env))   
           
#define XML_SCHEMA_WRITE_WITH_OUT(schema, env, out) \
        (((xml_schema_t *) schema)->ops->\
            write_with_out(schema, env, out))
            
#define XML_SCHEMA_WRITE_WITH_WRITER(schema, env, writer) \
        (((xml_schema_t *) schema)->ops->\
            write_with_writer(schema, env, writer))     
            
#define XML_SCHEMA_GET_PREFIX_TO_NAMESPACE_MAP(schema, env)\
        (((xml_schema_t *) schema)->ops->\
            get_prefix_to_namespace_map(schema, env))    
            
#define XML_SCHEMA_SET_PREFIX_TO_NAMESPACE_MAP(schema, env, map)\
        (((xml_schema_t *) schema)->ops->\
            set_prefix_to_namespace_map(schema, env, map))         
            
#define XML_SCHEMA_ADD_TYPE(schema, env, type)\
        (((xml_schema_t *) schema)->ops->\
            add_type(schema, env, type)) 
            
#define XML_SCHEMA_SET_SCHEMA_NS_PREFIX(schema, env, ns_prefix) \
        (((xml_schema_t *) schema)->ops->\
            set_schema_ns_prefix(schema, env, ns_prefix))
            
#define XML_SCHEMA_SERIALIZE(schema, env) \
        (((xml_schema_t *) schema)->ops->\
            serialize(schema, env))                                              
                      
#define XML_SCHEMA_SET_ROOT_NODE(schema, env, root_node) \
        (((xml_schema_t *) schema)->ops->\
            set_root_node(schema, env, root_node))
                                  
                      
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_H */
