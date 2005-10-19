#ifndef AXIS2_DESCRIPTION_OPERATION_H
#define AXIS2_DESCRIPTION_OPERATION_H

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#include <axis2_core.h>
#include <axis2_description_param_include.h>
#include <axis2_description_service.h>

/************************** Start of function macros **************************/
	
#define axis2_description_operation_free(env, operation_desc) \
		(axis2_description_operation_get_ops(env, \
		operation_desc)->free (env, operation_desc));

#define axis2_description_operation_add_param(env, operation_desc, param) \
		(axis2_description_operation_get_ops(env, \
		operation_desc)->add_param (env, operation_desc, param));

#define axis2_description_operation_get_param get_param(env, operation_desc) \
		(axis2_description_operation_get_ops(env, \
		operation_desc)->get_param (env, operation_desc));

#define axis2_description_operation_get_params get_params(env, operation_desc) \
		(axis2_description_operation_get_ops(env, \
		operation_desc)->get_params (env, operation_desc));

#define axis2_description_operation_set_parent(env, operation_desc, \
		service_desc) (axis2_description_operation_get_ops(env, \
		operation_desc)->set_parent (env, operation_desc, service_desc));

#define axis2_description_operation_get_parent(env, operation_desc) \
		(axis2_description_operation_get_ops(env, \
		operation_desc)->get_parent (env, operation_desc));
	

/************************** End of function macros ****************************/

/************************** Start of function pointers ************************/

typedef axis2_status_t (*axis2_description_operation_free_t) 
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);

typedef axis2_status_t (*axis2_description_operation_add_param_t)
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_description_param_t *param);

typedef axis2_description_param_t *(*axis2_description_operation_get_param_t)
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, const axis2_char_t *name);

typedef axis2_hash_t *(*axis2_description_operation_get_params_t)
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc);

typedef axis2_status_t (*axis2_description_operation_set_parent_t)
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc
		, axis2_description_service_t *service_desc);

typedef axis2_description_service_t *(*axis2_description_operation_get_parent_t)
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);
		
/**************************** End of function pointers ************************/

struct axis2_description_operation_ops_s
{
	axis2_description_operation_free_t free;

	axis2_description_operation_add_param_t add_param;

	axis2_description_operation_get_param_t get_param;

	axis2_description_operation_get_params_t get_params;

	axis2_description_operation_set_parent_t set_parent;

	axis2_description_operation_get_parent_t get_parent;
};

axis2_description_operation_t *axis2_description_operation_get_ops
		(axis2_environment_t *env);

axis2_description_operation_t 
		*axis2_description_operation_create (axis2_environment_t *env);

axis2_description_operation_t *axis2_description_operation_create_with_name 
		(axis2_environment_t *env, axis2_qname_t *name);
		
#endif /* AXIS2_DESCRIPTION_OPERATION_H */
