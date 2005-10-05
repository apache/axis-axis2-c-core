#ifndef AXIS2_OPERATION_DESCRIPTION_H
#define AXIS2_OPERATION_DESCRIPTION_H

#include <axis2_operation_description.h>
#include <axis2_description_param_include.h>

typedef struct axis2_operation_description_s axis2_operation_description_t;
typedef struct axis2_service_description_s axis2_service_description_t;
typedef struct axis2_servicegroup_description_s axis2_servicegroup_description_t;

/**
 * <p>This holds the information about operation. </p>
 */
struct axis2_operation_description_s
{
	axis2_description_param_include_t *param_include;
	axis2_service_description_t *parent;
	axis2_qname_t *name;	

} axis2_operation_description_s;

/**
 * <p>This holds the information about service. </p>
 */
struct axis2_service_description_s
{
	axis2_description_param_include_t *param_include;
	axis2_servicegroup_description_t *parent;    

} axis2_service_description_s;

/**
 * <p> This holds the information about the servicegroup_name
 */
struct axis2_servicegroup_description_s
{
	axis2_description_param_include_t *param_include;
	axis2_qname_t *servicegroup_name;
	apr_hash_t *services;
	
} axis2_servicegroup_description_s;

axis2_operation_description_t 
	*create_axis2_operation_description ();

axis2_operation_description_t 
	*create_axis2_operation_description_with_param_include (
	axis2_description_param_include_t* param_include);

axis2_operation_description_t *create_axis2_operation_description_with_name 
	(axis2_description_param_include_t *param_include, axis2_qname_t *name);

int free_axis2_operation_description (axis2_operation_description_t
    *operation_desc);

int axis2_operation_description_add_parameter(axis2_operation_description_t
	*operation_desc, axis2_parameter_t *param);

axis2_parameter_t *axis2_operation_description_get_parameter(
	axis2_operation_description_t *operation_desc, char *name);

axis2_parameter_t *axis2_operation_description_get_parameters(
	axis2_operation_description_t * operation_desc);

int axis2_operation_description_set_parent(axis2_operation_description_t
	*operation_desc, axis2_service_description_t *service_description);

axis2_service_description_t *axis2_operation_description_get_parent
	(axis2_operation_description_t *operation_desc);

#endif //AXIS2_OPERATION_DESCRIPTION_H
