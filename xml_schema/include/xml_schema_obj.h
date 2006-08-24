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

#ifndef XML_SCHEMA_OBJ_H
#define XML_SCHEMA_OBJ_H

/**
 * @defgroup xml_schema_obj xml_schema_obj
 * @ingroup xml_schema
 * xml_schema_obj defines a set of functions that are common to
 * other interfaces in xml_schema
 * @{
 */

/**
 * @file xml_schema_obj.h
 * @brief xml_schema_obj defines common function
 * set that is used by other xml_schema structures
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils_defines.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <xml_schema_defines.h>
#include <xml_schema_constants.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /* Type name for struct xml_schema_obj */
    typedef struct xml_schema_obj xml_schema_obj_t;
    /* Type name for struct xml_schema_obj */
    typedef struct xml_schema_obj_ops xml_schema_obj_ops_t;

    /** xml_schema_obj_ops struct
     * Encapsulator struct for operations of the xml_schema_obj
     */
    struct xml_schema_obj_ops
    {
        /**
         * Free the xml_schema_obj 
         * @param obj pointer to xml_schema_obj struct  
         * @param env pointer to environment struct
         * @returns AXIS2_SUCCESS on success else FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free) (
                    void *obj,
                    const axis2_env_t *env);
        /**
         * returns a hashtable containing pointers to base struct instances
         * @param obj xml_schema_obj struct 
         * @param env environment struct 
         * @returns pointer to hashtable if available ,
         * NULL otherwise
         */

        axis2_hash_t *(AXIS2_CALL *
                super_objs) (
                    void *obj,
                    const axis2_env_t *env);

        /**
         * get the type of the derived struct instance
         * eg :- xml_schema_annotated_t is a derived type of 
         * xml_schema_obj, If this function is called on a xml_schema_annotated 
         * pointer it will return XML_SCHEMA_ANNOTATED enum value
         * xml_schema_types_t is defined as an enumaration
         * @param obj xml_schema_obj
         * @param env pointer to environment struct
         * @returns xml_schema_types_t 
         */ 
        xml_schema_types_t (AXIS2_CALL *
                get_type) (
                    void *obj,
                    const axis2_env_t *env);

        /**
         * get the line number in xml document corresponding to 
         * this struct instance
         * @param obj pointer to xml_schema_obj struct instance
         * @param env pointer to environment
         * @returns the line number
         */
        int (AXIS2_CALL *
                get_line_num) (void *obj,
                        const axis2_env_t *env);
        /**
         * set the line number in xml document corresponding to
         * this struct instance
         * @param obj pointer to xml_schema_obj struct instance
         * @param env pointer to environment
         * @returns AXIS2_SUCCESS
         */
        axis2_status_t (AXIS2_CALL *
                set_line_num) (void *obj,
                        const axis2_env_t *env,
                        int line_num);

        /**
         * get line positon in xml document corresponding to 
         * this struct instance
         * @param obj pointer to xml_schema_obj struct instance
         * @param env environment struct
         * @returns the line position
         */
        int (AXIS2_CALL *
                get_line_pos) (void *obj,
                        const axis2_env_t *env);

        /**
         * set line position in xml document corresponding to
         * this struct instance
         * @param obj pointer to xml_schema_obj or its derived struct instance
         * @param env environment struct
         * @param line_pos line position
         * @returns AXIS2_SUCCESS
         */
        axis2_status_t (AXIS2_CALL *
                set_line_pos) (void *obj,
                        const axis2_env_t *env,
                        int line_pos);
        /**
         * get the source uri of xml schema obj
         * @param obj xml_schema_obj struct instance
         * @param env pointer to environment
         * @returns pointer to source uri string
         */

        axis2_char_t * (AXIS2_CALL *
                get_source_uri) (void *obj,
                        const axis2_env_t *env);
        
        /**
         * get the source uri of xml schema obj
         * @param obj xml_schema_obj struct instance
         * @param env pointer to environment
         * @param source_uri source uri string
         * @returns env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                set_source_uri) (void *obj,
                        const axis2_env_t *env,
                        axis2_char_t *source_uri);
        
        /**
         * compares two xml_schema_obj instances
         * @param obj xml_schema_obj instance
         * @param env environment struct
         * @param obj_comp xml_schema_obj instance to be compared
         * @param AXIS2_TRUE if equals , AXIS2_FALSE otherwise
         */
        axis2_bool_t (AXIS2_CALL *
                equals) (void *obj,
                        const axis2_env_t *env,
                        void *obj_comp);

        /**
         * Add a value to meta info hashtable 
         * if hashtable will be created, when this function is called 
         * for the first time .
         * @param obj xml_schema_obj instance
         * @param env environment struct
         * @param key key
         * @param value value 
         * @returns AXIS2_SUCCESS on success, AXIS2_FAILURE otherwise
         */
        axis2_status_t (AXIS2_CALL *
                add_meta_info)(void *obj,
                        const axis2_env_t *env,
                        axis2_char_t *key,
                        void *value);
        
        /**
         * get meta info hashtable , will be null if it does not 
         * contain any meta infomation.
         * @param obj xml_schema_obj or its subtype 
         * @param env environment struct
         * @returns pointer to axis2_hash_t , NULL of not intialized
         */
        axis2_hash_t* (AXIS2_CALL *
                get_meta_info)(void *obj,
                        const axis2_env_t *env);

    };

    /**
     * xml_schema_obj struct
     */

    struct xml_schema_obj
    {
        /** operations of the xml_schema_obj */
        xml_schema_obj_ops_t *ops;
    };

    /**
     * Creates an xml_schema_obj instance
     * @param env pointer to environment struct
     * @returns xml_schema_obj instance 
     */

    AXIS2_EXTERN xml_schema_obj_t * AXIS2_CALL
    xml_schema_obj_create(const axis2_env_t *env);

    /**
     * This method is internal to Axis2 C. 
     * It is called from Child Constructor
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    xml_schema_obj_resolve_methods(xml_schema_obj_t *schema_obj,
            const axis2_env_t *env,
            xml_schema_obj_t *schema_obj_impl,
            XML_SCHEMA_SUPER_OBJS_FN super_objs,
            XML_SCHEMA_GET_TYPE_FN get_type,
            XML_SCHEMA_FREE_FN free_fn);


/**  Free xml_schema_obj struct instance.
     @sa xml_schema_obj_ops#free  */
#define XML_SCHEMA_OBJ_FREE(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->free (obj, env))

/** Get hashtable containing pointers to base struct instances.
    @sa xml_schema_obj_ops#super_objs */
#define XML_SCHEMA_OBJ_SUPER_OBJS(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->super_objs (obj, env))
      
/** Get the type of struct type one of xml_schema_types enu.
    @sa xml_schema_obj_ops#get_type */
#define XML_SCHEMA_OBJ_GET_TYPE(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->get_type (obj, env))

/** Get the line number in xml document .
     @sa xml_schema_obj_ops#get_line_num */
#define XML_SCHEMA_OBJ_GET_LINE_NUM(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->get_line_num (obj, env))

/** Set line number.
    @sa xml_schema_obj_ops#set_line_num */
#define XML_SCHEMA_OBJ_SET_LINE_NUM(obj, env, line_num) \
      (((xml_schema_obj_t *) obj)->ops->set_line_num (obj, env, line_num))

/** Get line position.
    @sa xml_schema_obj_ops#get_line_ops */
#define XML_SCHEMA_OBJ_GET_LINE_POS(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->get_line_pos (obj, env))

/** Set Line position.
    @sa xml_schema_obj_ops#set_line_pos */
#define XML_SCHEMA_OBJ_SET_LINE_POS(obj, env, line_pos) \
      (((xml_schema_obj_t *) obj)->ops->set_line_pos (obj, env, line_pos))

/** Get source uri.
    @sa xml_schema_obj_ops#get_source_uri */
#define XML_SCHEMA_OBJ_GET_SOURCE_URI(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->get_source_uri (obj, env))

/** Set source uri.
    @sa xml_schema_obj_ops#set_source_uri */
#define XML_SCHEMA_OBJ_SET_SOURCE_URI(obj, env, source_uri) \
      (((xml_schema_obj_t *) obj)->ops->set_source_uri (obj, env, source_uri))

/** Equals 
    @sa xml_schema_obj_ops#equals */
#define XML_SCHEMA_OBJ_EQUALS(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->equals (obj, env))

/** Get meta infomation
    @sa xml_schema_obj_ops#get_meta_info */
#define XML_SCHEMA_OBJ_GET_META_INFO(obj , env) \
    (((xml_schema_obj_t*) obj)->ops->get_meta_info(obj, env))

/** Add meta info
    @sa xml_schema_obj#add_meta_info */
#define XML_SCHEMA_OBJ_ADD_META_INFO(obj, env) \
    (((xml_schema_obj_t*) obj)->ops->add_meta_info(obj, env))
    /** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_OBJ_H */
