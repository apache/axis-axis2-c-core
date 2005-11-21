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
 
#include <axis2_pull_parser.h>
#include <axis2_env.h>
#include <axis2.h>
#include <guththila_xml_pull_parser.h>

/**************** function prototypes ******************************************/



int AXIS2_CALL 
axis2_guththila_wrapper_next(axis2_pull_parser_t *parser,
                             axis2_env_t **env);

        
axis2_status_t AXIS2_CALL 
axis2_guththila_wrapper_free(axis2_pull_parser_t *parser,
                             axis2_env_t **env);
           
        
int AXIS2_CALL 
axis2_guththila_wrapper_get_attribute_count(axis2_pull_parser_t *parser,
                                            axis2_env_t **env);
        
axis2_char_t* AXIS2_CALL 
axis2_guththila_wrapper_get_attribute_name_by_number(axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i);
        
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_attribute_prefix_by_number(axis2_pull_parser_t *parser,
                                                       axis2_env_t **env,
                                                       int i);
                                                     
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_attribute_value_by_number(
                                                axis2_pull_parser_t *parser,
                                                axis2_env_t **env,
                                                int i);
        
        
axis2_char_t* AXIS2_CALL 
axis2_guththila_wrapper_get_attribute_namespace_by_number(
                                                 axis2_pull_parser_t *parser,
                                                 axis2_env_t **env,
                                                 int i);

                                
axis2_char_t* AXIS2_CALL 
axis2_guththila_wrapper_get_value(axis2_pull_parser_t *parser,
                                  axis2_env_t **env);
                                                      
int AXIS2_CALL 
axis2_guththila_wrapper_get_namespace_count(axis2_pull_parser_t *parser,
                                            axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_namespace_uri_by_number(axis2_pull_parser_t *parser,
                                                    axis2_env_t **env,
                                                    int i);
                                               
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_namespace_prefix_by_number(
                                                axis2_pull_parser_t *parser,
                                                axis2_env_t **env,
                                                int i); 
                                                
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_prefix(axis2_pull_parser_t *parser,
                                   axis2_env_t **env);   
        
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_name(axis2_pull_parser_t *parser,
                                 axis2_env_t **env); 

axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_pi_target(axis2_pull_parser_t *parser,
                                      axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_pi_data(axis2_pull_parser_t *parser,
                                     axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_dtd(axis2_pull_parser_t *parser,
                                     axis2_env_t **env);                                     

axis2_status_t AXIS2_CALL
axis2_guththila_wrapper_xml_free(axis2_pull_parser_t *parser,
                                 axis2_env_t **env,
                                 void *data);


/*********** axis2_guththila_wrapper_impl_t wrapper struct   *******************/

typedef struct axis2_guththila_wrapper_impl
{
    axis2_pull_parser_t parser;    
    guththila_xml_pull_parser_t *guththila_parser;
    guththila_environment_t *guththila_env;
    int event_map[10];
    
}axis2_guththila_wrapper_impl_t;

/********************************* Macro ***************************************/

#define AXIS2_INTF_TO_IMPL(p) ((axis2_guththila_wrapper_impl_t*)p)

#define AXIS2_IMPL_TO_INTF(p) &(p->parser)

/**********End macros  --  event_map initializing function *********************/

static axis2_status_t axis2_guththila_wrapper_init_map(
                            axis2_guththila_wrapper_impl_t *parser)
{
    if(parser)
    {
        parser->event_map[GUTHTHILA_START_DOCUMENT] = AXIS2_PULL_PARSER_START_DOCUMENT;
        parser->event_map[GUTHTHILA_START_ELEMENT]  = AXIS2_PULL_PARSER_START_ELEMENT;
        parser->event_map[GUTHTHILA_END_ELEMENT]    = AXIS2_PULL_PARSER_END_ELEMENT;
        parser->event_map[GUTHTHILA_SPACE]          = AXIS2_PULL_PARSER_SPACE;
        parser->event_map[GUTHTHILA_EMPTY_ELEMENT]  = AXIS2_PULL_PARSER_EMPTY_ELEMENT;
        parser->event_map[GUTHTHILA_CHARACTER]      = AXIS2_PULL_PARSER_CHARACTER;
        parser->event_map[GUTHTHILA_ENTITY_REFERANCE] = AXIS2_PULL_PARSER_ENTITY_REFERANCE;
        parser->event_map[GUTHTHILA_COMMENT]          = AXIS2_PULL_PARSER_COMMENT;
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

/********************************************************************************/

AXIS2_DECLARE(axis2_pull_parser_t *)
axis2_pull_parser_create_for_file(axis2_env_t **env,char* filename)
{
    axis2_guththila_wrapper_impl_t *guththila_impl = NULL;
    guththila_allocator_t *allocator = NULL;
    guththila_environment_t *guththila_env = NULL;
    guththila_reader_t *reader = NULL;
    guththila_xml_pull_parser_t *guththila = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    guththila_impl = AXIS2_MALLOC((*env)->allocator,
                             sizeof(axis2_guththila_wrapper_impl_t));
    
    if(!guththila_impl)
          AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    allocator = guththila_allocator_init(NULL);
    guththila_env  = guththila_environment_create(allocator,NULL, NULL);
    
    reader = guththila_reader_create_for_file(guththila_env, filename);
    
    if(!reader)
    {
        AXIS2_FREE((*env)->allocator,guththila_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }    
    
    guththila = guththila_xml_pull_parser_create(guththila_env,reader);
    if(!guththila)
    {
        AXIS2_FREE((*env)->allocator,guththila_impl);
        AXIS2_FREE((*env)->allocator,reader);
        AXIS2_ERROR_SET((*env)->error,AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    
    guththila_impl->guththila_parser = guththila;
    guththila_impl->guththila_env = guththila_env;
    guththila_impl->parser.ops = NULL;
    guththila_impl->parser.ops = (axis2_pull_parser_ops_t*)AXIS2_MALLOC(
                                      (*env)->allocator,
                                       sizeof(axis2_pull_parser_ops_t));
    if(!(guththila_impl->parser.ops))
    {   guththila_xml_pull_parser_free( guththila_env, guththila);
        AXIS2_FREE((*env)->allocator,guththila_impl);
        AXIS2_FREE((*env)->allocator,reader);
        AXIS2_ERROR_SET((*env)->error,AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    
    axis2_guththila_wrapper_init_map(guththila_impl);
    
    
/************** operations *****/    
    guththila_impl->parser.ops->next = axis2_guththila_wrapper_next;
    
    guththila_impl->parser.ops->free = axis2_guththila_wrapper_free;
    
    guththila_impl->parser.ops->get_attribute_count =
        axis2_guththila_wrapper_get_attribute_count;
    guththila_impl->parser.ops->get_attribute_name_by_number =
        axis2_guththila_wrapper_get_attribute_name_by_number;
    guththila_impl->parser.ops->get_attribute_value_by_number =
        axis2_guththila_wrapper_get_attribute_value_by_number;
    guththila_impl->parser.ops->get_attribute_prefix_by_number =
        axis2_guththila_wrapper_get_attribute_prefix_by_number;
    guththila_impl->parser.ops->get_attribute_namespace_by_number =
        axis2_guththila_wrapper_get_attribute_namespace_by_number;
    
    guththila_impl->parser.ops->get_value = axis2_guththila_wrapper_get_value;
    guththila_impl->parser.ops->get_name  = axis2_guththila_wrapper_get_name;
    guththila_impl->parser.ops->get_prefix = axis2_guththila_wrapper_get_prefix;
    
    guththila_impl->parser.ops->get_namespace_count =
        axis2_guththila_wrapper_get_namespace_count;
    guththila_impl->parser.ops->get_namespace_uri_by_number =
        axis2_guththila_wrapper_get_namespace_uri_by_number;
    guththila_impl->parser.ops->get_namespace_prefix_by_number =
        axis2_guththila_wrapper_get_namespace_prefix_by_number;

    guththila_impl->parser.ops->get_dtd=
        axis2_guththila_wrapper_get_dtd;

    guththila_impl->parser.ops->xml_free = axis2_guththila_wrapper_xml_free;        

    return &(guththila_impl->parser);
}

/****** pull parser for memory create function ***************************/


AXIS2_DECLARE(axis2_pull_parser_t *)
axis2_pull_parser_create_for_memory(axis2_env_t **env,
                                    int (*read_input_callback)(char *buffer,int size),
                                    void (*close_input_callback)(void))
{
    axis2_guththila_wrapper_impl_t *guththila_impl = NULL;
    guththila_allocator_t *allocator = NULL;
    guththila_environment_t *guththila_env = NULL;
    guththila_reader_t *reader = NULL;
    guththila_xml_pull_parser_t *guththila = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    guththila_impl = AXIS2_MALLOC((*env)->allocator,
                             sizeof(axis2_guththila_wrapper_impl_t));
    
    if(!guththila_impl)
          AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    allocator = guththila_allocator_init(NULL);
    guththila_env  = guththila_environment_create(allocator,NULL, NULL);
    
    /*-------difference of two create function is here--------*/
    reader = guththila_reader_create_for_memory(guththila_env,
                     read_input_callback, close_input_callback); 
                                                   
    if(!reader)
    {
        AXIS2_FREE((*env)->allocator,guththila_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }    
    
    guththila = guththila_xml_pull_parser_create(guththila_env,reader);
    if(!guththila)
    {
        AXIS2_FREE((*env)->allocator,guththila_impl);
        AXIS2_FREE((*env)->allocator,reader);
        AXIS2_ERROR_SET((*env)->error,AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    
    guththila_impl->guththila_parser = guththila;
    guththila_impl->guththila_env = guththila_env;
    guththila_impl->parser.ops = NULL;
    guththila_impl->parser.ops = (axis2_pull_parser_ops_t*)AXIS2_MALLOC(
                                      (*env)->allocator,
                                       sizeof(axis2_pull_parser_ops_t));
    if(!(guththila_impl->parser.ops))
    {   guththila_xml_pull_parser_free( guththila_env, guththila);
        AXIS2_FREE((*env)->allocator,guththila_impl);
        AXIS2_FREE((*env)->allocator,reader);
        AXIS2_ERROR_SET((*env)->error,AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    
    axis2_guththila_wrapper_init_map(guththila_impl);
    
    
/************** operations *****/    
    guththila_impl->parser.ops->next = axis2_guththila_wrapper_next;
    
    guththila_impl->parser.ops->free = axis2_guththila_wrapper_free;
    
    guththila_impl->parser.ops->get_attribute_count =
        axis2_guththila_wrapper_get_attribute_count;
    guththila_impl->parser.ops->get_attribute_name_by_number =
        axis2_guththila_wrapper_get_attribute_name_by_number;
    guththila_impl->parser.ops->get_attribute_value_by_number =
        axis2_guththila_wrapper_get_attribute_value_by_number;
    guththila_impl->parser.ops->get_attribute_prefix_by_number =
        axis2_guththila_wrapper_get_attribute_prefix_by_number;
    guththila_impl->parser.ops->get_attribute_namespace_by_number =
        axis2_guththila_wrapper_get_attribute_namespace_by_number;
    
    guththila_impl->parser.ops->get_value = axis2_guththila_wrapper_get_value;
    guththila_impl->parser.ops->get_name  = axis2_guththila_wrapper_get_name;
    guththila_impl->parser.ops->get_prefix = axis2_guththila_wrapper_get_prefix;
    
    guththila_impl->parser.ops->get_namespace_count =
        axis2_guththila_wrapper_get_namespace_count;
    guththila_impl->parser.ops->get_namespace_uri_by_number =
        axis2_guththila_wrapper_get_namespace_uri_by_number;
    guththila_impl->parser.ops->get_namespace_prefix_by_number =
        axis2_guththila_wrapper_get_namespace_prefix_by_number;
    guththila_impl->parser.ops->get_dtd=
        axis2_guththila_wrapper_get_dtd;
        
    guththila_impl->parser.ops->xml_free = axis2_guththila_wrapper_xml_free;
    return &(guththila_impl->parser);
}
int AXIS2_CALL 
axis2_guththila_wrapper_next(axis2_pull_parser_t *parser,
                             axis2_env_t **env)
{
    int i =-1;
    AXIS2_FUNC_PARAM_CHECK(parser,env, -1);
    i = guththila_xml_pull_parser_next(AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
            AXIS2_INTF_TO_IMPL(parser)->guththila_parser); 
    return i == -1 ? -1 : AXIS2_INTF_TO_IMPL(parser)->event_map[i]; 
    
}

        
axis2_status_t AXIS2_CALL 
axis2_guththila_wrapper_free(axis2_pull_parser_t *parser,
                             axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env,AXIS2_FAILURE);
    if(AXIS2_INTF_TO_IMPL(parser)->guththila_parser)
        guththila_xml_pull_parser_free(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env,
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser);
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(parser)->guththila_env);
    
    if(parser->ops)
        AXIS2_FREE((*env)->allocator, parser->ops);
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(parser));
    return AXIS2_SUCCESS;   
}
           
        
int AXIS2_CALL 
axis2_guththila_wrapper_get_attribute_count(axis2_pull_parser_t *parser,
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(parser, env, AXIS2_FAILURE);
    return guththila_xml_pull_parser_get_attribute_count(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser);
}
        
axis2_char_t* AXIS2_CALL 
axis2_guththila_wrapper_get_attribute_name_by_number(axis2_pull_parser_t *parser,
                                                     axis2_env_t **env,
                                                     int i)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_attribute_name_by_number(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser, i);    
    
    
}
        
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_attribute_prefix_by_number(axis2_pull_parser_t *parser,
                                                       axis2_env_t **env,
                                                       int i)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_attribute_prefix_by_number(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser, i);    
    
    
}

                                                     
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_attribute_value_by_number(axis2_pull_parser_t *parser,
                                                      axis2_env_t **env,
                                                      int i)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_attribute_value_by_number(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser, i);    
    
    
}
        
        
axis2_char_t* AXIS2_CALL 
axis2_guththila_wrapper_get_attribute_namespace_by_number(
                                                 axis2_pull_parser_t *parser,
                                                 axis2_env_t **env,
                                                 int i)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_attribute_namespace_by_number(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser, i);    
    
    
}

                                
axis2_char_t* AXIS2_CALL 
axis2_guththila_wrapper_get_value(axis2_pull_parser_t *parser,
                                  axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_value(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser);    
    
    
}
                                                      
int AXIS2_CALL 
axis2_guththila_wrapper_get_namespace_count(axis2_pull_parser_t *parser,
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, AXIS2_FAILURE);
    return guththila_xml_pull_parser_get_namespace_count(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser);    
    
    
}

axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_namespace_uri_by_number(axis2_pull_parser_t *parser,
                                                    axis2_env_t **env,
                                                    int i)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_namespace_uri_by_number(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser, i);    
    
    
}
                                               
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_namespace_prefix_by_number(
                                                axis2_pull_parser_t *parser,
                                                axis2_env_t **env,
                                                int i)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_namespace_prefix_by_number(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser, i);    
    
    
}
                                                
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_prefix(axis2_pull_parser_t *parser,
                                   axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_prefix(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser);    
    
    
}
        
axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_name(axis2_pull_parser_t *parser,
                                 axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(parser,env, NULL);
    return guththila_xml_pull_parser_get_name(
                    AXIS2_INTF_TO_IMPL(parser)->guththila_env, 
                    AXIS2_INTF_TO_IMPL(parser)->guththila_parser);    
    
    
}

axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_pi_target(axis2_pull_parser_t *parser,
                                      axis2_env_t **env)
{   
    /* guththila_does not support pi's yet*/
    return NULL;   
 }

axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_pi_data(axis2_pull_parser_t *parser,
                                     axis2_env_t **env)
 {
    /* guththila_dose not support yet */
    return NULL;
 }

 axis2_char_t* AXIS2_CALL
axis2_guththila_wrapper_get_dtd(axis2_pull_parser_t *parser,
                                     axis2_env_t **env)
{
    printf("not implemented in guththila");
}
 
axis2_status_t AXIS2_CALL
axis2_guththila_wrapper_xml_free(axis2_pull_parser_t *parser,
                                 axis2_env_t **env,
                                 void *data)
{
    AXIS2_FUNC_PARAM_CHECK(parser, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
    GUTHTHILA_FREE(AXIS2_INTF_TO_IMPL(parser)->guththila_env->allocator,
                   data);
    return AXIS2_SUCCESS;
}
