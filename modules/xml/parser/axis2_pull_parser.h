/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
 
#ifndef AXIS2_PULL_PARSER_H
#define AXIS2_PULL_PARSER_H

/**
 *@file axis2_pull_parser.h	
 *@brief this is the parser abstraction layer for axis2 
 */

#include <axis2_env.h>


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_pull_parser_ops axis2_pull_parser_ops_t;
typedef struct axis2_pull_parser axis2_pull_parser_t;

/**
 * @defgroup axis2_pull_parser 
 * @ingroup axis2_om_parser
 * @{
 */

enum axis2_pull_parser_event_types
{
    AXIS2_PULL_PARSER_START_DOCUMENT = 0,
    AXIS2_PULL_PARSER_START_ELEMENT,
    AXIS2_PULL_PARSER_END_ELEMENT,
    AXIS2_PULL_PARSER_SPACE,
    AXIS2_PULL_PARSER_EMPTY_ELEMENT,
    AXIS2_PULL_PARSER_CHARACTER,
    AXIS2_PULL_PARSER_ENTITY_REFERANCE,
    AXIS2_PULL_PARSER_COMMENT,
    AXIS2_PULL_PARSER_PROCESSING_INSTRUCTION,
    AXIS2_PULL_PARSER_CDATA,
    AXIS2_PULL_PARSER_DOCUMENT_TYPE
};

  /**
    * \brief AXIS2_PULL_PARSER operations
    * Encapsulator struct for operations of axis2_pull_parser
    */

AXIS2_DECLARE_DATA struct axis2_pull_parser_ops
    {
        /**
         * 
         * @param parser axis2_pull_parser struct 
         * @param env    axis2_environment struct 
         * @returns one of the events defined in 
         *                  axis2_pull_parser_event_types
         */
        
        int (AXIS2_CALL *next)(axis2_pull_parser_t *parser,
                               axis2_env_t **env);
         /**
         * free pull_parser
         * @param parser axis2_pull_parser struct 
         * @param env    axis2_environment struct 
         * @returns axis2_status_code
         */
        
        axis2_status_t (AXIS2_CALL *free)(axis2_pull_parser_t *parser,
                                          axis2_env_t **env);
        /**
         * Get the Number of attributes in the current element 
         * @param parser axis2_pull_parser struct 
         * @param env    axis2_environment struct 
         * @returns Number of attributes , AXIS2_FAILURE on error 
         */  
        
        int (AXIS2_CALL *get_attribute_count)(axis2_pull_parser_t *parser,
                                              axis2_env_t **env);
       /** This is used to get an attribute's localname using an index relative to 
         * current element The iterations are not zero based. 
         * To access the first attribute use 1 for parameter i
         * @param parser parser struct 
         * @param env environment struct
         * @param i attribute index
         * @returns the attribute localname 
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro         
         */
        axis2_char_t* (AXIS2_CALL *get_attribute_name_by_number)
                                                    (axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
                                                     
       /** This is used to get an attribute's prefix using an index relative to 
         * current element. The iterations are not zero based. 
         * To access the first attribute use 1 for parameter i
         * @param parser parser struct 
         * @param env environment struct
         * @param i attribute index
         * @returns the attribute prefix, returns NULL on error  
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro         
         */                                                     
        
        axis2_char_t* (AXIS2_CALL *get_attribute_prefix_by_number)
                                                    (axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
        /** This is used to get an attribute's value using an index relative to 
         * current element. The iterations are not zero based. 
         * To access the first attribute use 1 for parameter i
         * @param parser parser struct 
         * @param env environment struct
         * @param i attribute index
         * @returns the attribute value, returns NULL on error
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro         
         */                                            
        axis2_char_t* (AXIS2_CALL *get_attribute_value_by_number)
                                                    (axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
        /** This is used to get an attribute's namespace uri using an index relative to 
         * current element. The iterations are not zero based. 
         * To access the first attribute use 1 for parameter i
         * @param parser parser struct 
         * @param env environment struct
         * @param i attribute index
         * @returns the attribute value, returns NULL on error  
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro        
         */ 
        
        axis2_char_t* (AXIS2_CALL *get_attribute_namespace_by_number)
                                                    (axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
       /** returns the text value of current element
         * @param parser 
         * @param env
         * @returns Text Value, NULL on error 
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro 
         */
                                
        axis2_char_t* (AXIS2_CALL *get_value)(axis2_pull_parser_t *parser,
                                              axis2_env_t **env);
        
        /**
         * Returns the namespace count of current element 
         *@param parser parser struct 
         *@param env environment
         *@returns namespace count , caller must free the value using 
         *                           AXIS2_PULL_PARSER_XML_FREE macro
         */
        int (AXIS2_CALL *get_namespace_count)(axis2_pull_parser_t *parser,
                                              axis2_env_t **env);
        /**
         * access the namespace uri of the namespaces declared in current element 
         * using an index 
         * @param parser parser struct 
         * @param env environment 
         * @param i index
         * @returns namespace uri of corresponding namespace 
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro         
         */
        axis2_char_t* (AXIS2_CALL *get_namespace_uri_by_number)
                                              (axis2_pull_parser_t *parser,
                                               axis2_env_t **env,
                                               int i);
        /**
         * access the namespace prefix of the namespaces declared in current element 
         * using an index 
         * @param parser parser struct 
         * @param env environment 
         * @param i index
         * @returns namespace prefix of corresponding namespace 
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro         
         */                                               
        axis2_char_t* (AXIS2_CALL *get_namespace_prefix_by_number)
                                               (axis2_pull_parser_t *parser,
                                                axis2_env_t **env,
                                                int i); 
        /**
         *  Used to obtain the current element prefix
         * @param parser parser struct 
         * @param env environment struct
         * @returns prefix , NULL on error 
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro
         */         
        
        axis2_char_t* (AXIS2_CALL *get_prefix)(axis2_pull_parser_t *parser,
                                               axis2_env_t **env);   
        /**
         *  Used to obtain the current element localname
         * @param parser parser struct 
         * @param env environment struct
         * @returns localname , NULL on error 
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro
         */         
        axis2_char_t* (AXIS2_CALL *get_name)(axis2_pull_parser_t *parser,
                                             axis2_env_t **env); 
        /**
         * Used to get the processingInstruction target
         *@param parser parser struct
         *@param env environment
         *@returns target value of processingInstruction
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro
         */        
        axis2_char_t* (AXIS2_CALL *get_pi_target)(axis2_pull_parser_t *parser,
                                                  axis2_env_t **env);
        /**
         * Used to get the processingInstruction data
         *@param parser parser struct
         *@param env environment
         *@returns data of processingInstruction
         *          caller must free the value using AXIS2_PULL_PARSER_XML_FREE macro
         */        


        axis2_char_t* (AXIS2_CALL *get_pi_data)(axis2_pull_parser_t *parser,
                                                axis2_env_t **env);
        /**
         *  used to get the DTD 
         *
         */         
        axis2_char_t* (AXIS2_CALL *get_dtd)(axis2_pull_parser_t *parser,
                                            axis2_env_t **env);                                                
        /**
         *  Free function , this function wraps the underlying parser's 
         *  free function
         * @param data data values to be destroyed         
         */         
        axis2_status_t (AXIS2_CALL *xml_free)(axis2_pull_parser_t *parser,
                                   axis2_env_t **env,
                                   void *data);                                                
    };
     
/** 
 * @brief axis2_pull_parser struct
  *	Axis2 OM pull_parser
 */
struct axis2_pull_parser
{
    axis2_pull_parser_ops_t *ops;  
};    

/**
 * create pull parser struct
 * @param env environment struct , must not be null
 * @param filename filename of an xml document
 * @returns a pointer to xml_pull_parser_t struct
 *          NULL on error with error code set in the environment's error 
 */

AXIS2_DECLARE(axis2_pull_parser_t *)
axis2_pull_parser_create_for_file(axis2_env_t **env,
                                  char *filename,
                                  const axis2_char_t *encoding);

AXIS2_DECLARE(axis2_pull_parser_t *)
axis2_pull_parser_create_for_memory(axis2_env_t **env,
                                    int (*read_input_callback)(char *buffer,int size),
                                    const axis2_char_t *encoding);
                                    
/********************************* Macros *************************************/

/** Macros corresponding to above declared function pointers */

#define AXIS2_PULL_PARSER_NEXT(parser, env) \
        ((parser)->ops->next(parser, env))

#define AXIS2_PULL_PARSER_FREE(parser, env) \
        ((parser)->ops->free(parser, env))

#define AXIS2_PULL_PARSER_GET_ATTRIBUTE_COUNT(parser, env) \
        ((parser)->ops->get_attribute_count(parser, env))

#define AXIS2_PULL_PARSER_GET_ATTRIBUTE_NAME_BY_NUMBER(parser, env, i) \
        ((parser)->ops->get_attribute_name_by_number(parser, env, i))
        
#define AXIS2_PULL_PARSER_GET_ATTRIBUTE_VALUE_BY_NUMBER(parser, env, i) \
        ((parser)->ops->get_attribute_value_by_number(parser,env, i))
        
#define AXIS2_PULL_PARSER_GET_ATTRIBUTE_NAMESPACE_BY_NUMBER(parser, env, i) \
        ((parser)->ops->get_attribute_namespace_by_number(parser, env, i))

#define AXIS2_PULL_PARSER_GET_ATTRIBUTE_PREFIX_BY_NUMBER(parser, env, i) \
        ((parser)->ops->get_attribute_prefix_by_number(parser, env, i))

#define AXIS2_PULL_PARSER_GET_NAME(parser, env) \
        ((parser)->ops->get_name(parser, env))

#define AXIS2_PULL_PARSER_GET_VALUE(parser, env) \
        ((parser)->ops->get_value(parser, env))
        
#define AXIS2_PULL_PARSER_GET_PREFIX(parser, env) \
        ((parser)->ops->get_prefix(parser, env))

#define AXIS2_PULL_PARSER_GET_NAMESPACE_COUNT(parser, env) \
        ((parser)->ops->get_namespace_count(parser, env))
        
#define AXIS2_PULL_PARSER_GET_NAMESPACE_URI_BY_NUMBER(parser, env, i) \
        ((parser)->ops->get_namespace_uri_by_number(parser, env, i))
        
#define AXIS2_PULL_PARSER_GET_NAMESPACE_PREFIX_BY_NUMBER(parser, env, i) \
        ((parser)->ops->get_namespace_prefix_by_number(parser, env, i))
        
#define AXIS2_PULL_PARSER_GET_PI_DATA(parser, env) \
        ((parser)->ops->get_pi_data(parser, env))

#define AXIS2_PULL_PARSER_GET_PI_TARGET(parser, env) \
        ((parser)->ops->get_pi_target(parser, env))
        
#define AXIS2_PULL_PARSER_GET_DTD(parser, env) \
        ((parser)->ops->get_dtd(parser, env))
        
#define AXIS2_PULL_PARSER_XML_FREE(parser, env, data) \
        ((parser)->ops->xml_free(parser, env, data))


/******************************* End Macros ***********************************/
/** @} */


#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_PULL_PARSER_H */
