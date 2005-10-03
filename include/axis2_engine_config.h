#ifndef AXIS2_ENGINE_CONFIG_H
#define AXIS2_ENGINE_CONFIG_H

#include <stdio.h>
#include <stdlib.h>

#include <axis2_description_param_include.h>
#include <axis2_servicegroup_description.h>

/**
 * @file axis2_engine_config.h
 * @brief Global states of all the Axis can
 *        can be accessed here
 */
/**
 * The palce where all the global states of Axis is accessed.
 * All the global states kept in the <code>EngineRegistry</code> and all the
 * Service states kept in the <code>MessageContext</code>. Other runtime
 * artifacts does not keep states forward from the execution.
 */

/**
 * <p>This holds the information about engine. </p>
 */
typedef struct
{
    axis2_description_param_include_t * param_include;
    axis2_qname_t *name;
    apr_hash_t *service_groups;
    
} axis2_engine_config_t;

void axis2_engine_config_add_service_group(axis2_engine_config_t *engine_config
    , axis2_servicegroup_description_t *service_group_desc);

void axis2_engine_config_add_service(axis2_engine_config_t *engine_config, 
    axis2_service_description_t *service_desc);

void axis2_engine_config_get_service(axis2_engine_config_t *engine_config,
    char* service_name);

void axis2_engine_config_remove_service(axis2_engine_config_t *engine_config,
    char *name);

#endif /* AXIS2_ENGINE_CONFIG_H */
