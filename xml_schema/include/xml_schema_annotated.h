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

#ifndef XML_SCHEMA_ANNOTATED_H
#define XML_SCHEMA_ANNOTATED_H

/** @defgroup xml_schema_annotated 
  * The is the base struct for any element that can contain annotation elements.
  * @ingroup xml_schema
  * @{
  */


/**
 * @file xml_schema_annotated.h
 * @brief 
 *  _______________        ______________________
 * |xml_schema_obj | ---> | xml_schema_annotated |
 * |_______________|      |______________________|
 */

#include <xml_schema_defines.h>
#include <xml_schema_obj.h>
                    
#ifdef __cplusplus
extern "C"
{
#endif

/** Type name for struct xml_schema_annotated. */
typedef struct xml_schema_annotated xml_schema_annotated_t;

/** Type name for struct xml_scheam_annoated_ops */
typedef struct xml_schema_annotated_ops xml_schema_annotated_ops_t;
                    
/** xml_schema_annoated_ops struct 
  * Encapsulator struct for operations 
  */
struct xml_schema_annotated_ops
{
    /**
     * Free xml_schema_annoated struct instance .
     * @param annotated annotated struct instance or its derived types 
     * @param env environment 
     * @returns AXIS2_SUCCESS
     */
    axis2_status_t (AXIS2_CALL *
    free)(void *annotated,
          const axutil_env_t *env);
    
    /**
     * Get hashtable of struct instances .
     * @param annotated pointer to xml_schema_annoated struct
     * @param env environment
     * @returns pointer to hashtable
     */
    axis2_hash_t *(AXIS2_CALL *
    super_objs)(
            void *annotated,
            const axutil_env_t *env);
    
    /**
     * Type of struct instance , can be XML_SCHEMA_ANNOTATED or 
     * one of its derived types.
     * @param annotated pointer to xml_schema_annoated struct instance
     * @param env environment
     * @returns one of xml_schema_types_t enum 
     */
    xml_schema_types_t (AXIS2_CALL *
    get_type)(
            void *annotated,
            const axutil_env_t *env);
    /**
     * Get pointer to xml_schema_obj struct 
     * @param annotated pointer to xml_schema_annoated struct instance
     * @param env environment
     * @returns pointer to xml_schema_obj struct
     */
    xml_schema_obj_t *(AXIS2_CALL *
    get_base_impl)(
            void *annotated,
            const axutil_env_t *env);

    /**
     * Get id .
     * @param annotated pointer to  xml_schema_annotated or derived type
     * @param env environment 
     * @returns id string 
     */
    axis2_char_t *(AXIS2_CALL *
    get_id) (void *annotated,
             const axutil_env_t *env);

    
    /** 
     * Set Id.
     * @param annotated pointer to  xml_schema_annotated or derived type
     * @param env environment
     * @param id id string
     * @returns AXIS2_SUCCESS on success, AXIS2_FAILURE otherwise
     */
    axis2_status_t (AXIS2_CALL *
    set_id) (
            void *annotated,
            const axutil_env_t *env,
            axis2_char_t *id);

    /**
     * Get the xml_schema_annoatation instance
     * @param annotated xml_schema_annotated pointer
     * @param env environment
     * @return pointer to annotation struct
     */
    struct xml_schema_annotation *(AXIS2_CALL *
    get_annotation) (
            void *annotated,
            const axutil_env_t *env);
    /**
     * Set annotation struct instance.
     * @param annotated pointer to xml_schema_annotated
     * @param env environment 
     * @param  annotation pointer to xml_schema_annotation struct
     * @returns AXIS2_SUCCESS on success, AXIS2_FAILURE otherwise
     */
    axis2_status_t (AXIS2_CALL *
    set_annotation)(
            void *annotated,
            const axutil_env_t *env,
            struct xml_schema_annotation *annotation);

    /**
     * Get Stored qualified attributes that do not 
     * belong to the schema target namespace.
     * @param annotated pointer to annotated struct
     * @param env environment
     ^ @returns a pointer to axutil_array_list_t if attributes exists
     * NULL otherwise
     */
    axutil_array_list_t *(AXIS2_CALL *
    get_unhandled_attrs) (
            void *annotated,
            const axutil_env_t *env);
   /** Set qualified attributes that do not belong
    * to the schema target namespace.
    * @param annotated pointer to xml_schema_annotated instance
    * @param env environment 
    * @param unhandled_attrs array list of unhandled attrs
    * @returns AXIS2_SUCCESS on success, AXIS2_FAILURE otherwise
    */
    axis2_status_t (AXIS2_CALL *
    set_unhandled_attrs) (
            void *annotated,
            const axutil_env_t *env,
            axutil_array_list_t *unhandled_attrs);
};

/**
 *  xml_schema_annotated struct
 */

struct xml_schema_annotated
{
    xml_schema_obj_t base;
    xml_schema_annotated_ops_t *ops;
};

/**
 * create an xml_schema_annotated struct instance 
 * @param env environment
 * @returns pointer to xml_schema_annotated_t 
 */
AXIS2_EXTERN xml_schema_annotated_t * AXIS2_CALL
xml_schema_annotated_create(const axutil_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_annotated_resolve_methods(
                                xml_schema_annotated_t *annotated,
                                const axutil_env_t *env,
                                xml_schema_annotated_t *annotated_impl,
                                XML_SCHEMA_SUPER_OBJS_FN super_objs,
                                XML_SCHEMA_GET_TYPE_FN get_type,
                                XML_SCHEMA_FREE_FN free_fn);
                                
/** Free xml_schema_annotated instance 
    @sa xml_schema_annotated_ops#free */
#define XML_SCHEMA_ANNOTATED_FREE(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->free(\
            annotated, env))

/** get base types 
    @sa xml_schema_annotated_ops#super_objs */
#define XML_SCHEMA_ANNOTATED_SUPER_OBJS(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->super_objs(\
            annotated, env))

/** get type 
   @sa xml_schema_annotated_ops#get_type */
#define XML_SCHEMA_ANNOTATED_GET_TYPE(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_type(\
            annotated, env))

/** get base struct xml_schema_obj
    @sa xml_schema_annotated_ops#get_base_impl */
#define XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_base_impl(\
            annotated, env))

/** get string id
    @sa xml_schema_annotated_ops#get_id */
#define XML_SCHEMA_ANNOTATED_GET_ID(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_id(\
            annotated, env))

/** set id
   @sa xml_schema_annotated_ops#set_id */
#define XML_SCHEMA_ANNOTATED_SET_ID(annotated, env, id) \
      (((xml_schema_annotated_t *) annotated)->ops->set_id(\
            annotated, env, id))

/** get annotation
  @sa xml_schema_annotated_ops#get_annotation */
#define XML_SCHEMA_ANNOTATED_GET_ANNOTATION(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_annotation(\
            annotated, env))
/** set annotation.
    @sa xml_schema_annotated_ops#set_annotation */
#define XML_SCHEMA_ANNOTATED_SET_ANNOTATION(annotated, env, \
        annotation) \
      (((xml_schema_annotated_t *) annotated)->ops->set_annotation(\
            annotated, env, annotation))

/** get attributes
    @sa xml_schema_annotated_ops#get_unhandled_attrs */
#define XML_SCHEMA_ANNOTATED_GET_UNHANDLED_ATTRS(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_unhandled_attrs(\
            annotated, env))

/** set attributes 
    @sa xml_schema_annotated_ops#set_unhandled_attrs */
#define XML_SCHEMA_ANNOTATED_SET_UNHANDLED_ATTRS(annotated, \
        env, unhandled_attrs) \
      (((xml_schema_annotated_t *) annotated)->ops->set_unhandled_attrs(\
            annotated, env, unhandled_attrs))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_ANNOTATED_H */
