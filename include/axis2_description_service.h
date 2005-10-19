#ifndef AXIS2_DESCRIPTION_SERVICE_H
#define AXIS2_DESCRIPTION_SERVICE_H

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#include <axis2_core.h>
#include <axis2_description_param_include.h>
#include <axis2_description_operation.h>
#include <axis2_description_servicegroup.h>
#include <axis2_qname.h>
	
/**************************** Function pointers *******************************/

typedef axis2_status_t *(*axis2_description_service_free_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

typedef axis2_status_t (*axis2_description_service_add_operation_t) 
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_description_operation_t *operation_desc);

typedef axis2_status_t (*axis2_description_service_add_param_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_description_param_t *param);

typedef axis2_description_param_t *(*axis2_description_service_get_param_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t *name);

typedef axis2_hash_t *(*axis2_description_service_get_params_t) 
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

typedef axis2_description_operation_t *(*axis2_description_service_get_operation_with_qname_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_qname_t *operation_name);
		
typedef axis2_description_operation_t 
		*(*axis2_description_service_get_operation_with_name_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t* operation_name);

typedef axis2_hash_t *(*axis2_description_service_get_operations_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc);
	
typedef axis2_status_t (*axis2_description_service_set_parent_t) 
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		,axis2_description_servicegroup_t *servicegroup_desc);

typedef axis2_description_servicegroup_t 
		*(*axis2_description_service_get_parent_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

/************************ End function pointers *******************************/

struct axis2_description_service_ops_s
{
	axis2_description_service_free_t free;
	
	axis2_description_service_add_operation_t add_operation;

	axis2_description_service_add_param_t add_param;

	axis2_description_service_get_param_t get_param;

	axis2_description_service_get_params_t get_params;

	axis2_description_service_get_operation_with_qname_t 
		get_operation_with_qname;

	axis2_description_service_get_operation_with_name_t get_operation_with_name;

	axis2_description_service_get_operations_t get_operations;
	
	axis2_description_service_set_parent_t set_parent;

	axis2_description_service_get_parent_t get_parent;

};

/******************************************************************************/
axis2_description_service_ops_t * axis2_description_service_get_ops
		(axis2_environment_t *env);

axis2_description_service_t *axis2_description_service_create_with_qname
		(axis2_environment_t *env);

#endif /* AXIS2_DESCRIPTION_SERVICE_H */
