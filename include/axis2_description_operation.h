#ifndef AXIS2_DESCRIPTION_OPERATION_H
#define AXIS2_DESCRIPTION_OPERATION_H

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>

#include <axis2_description_param_include.h>

struct axis2_description_operation_s;
struct axis2_description_operation_ops_s;
typedef struct axis2_description_operation_s axis2_description_operation_t;
typedef struct axis2_description_operation_ops_s 
		axis2_description_operation_ops_t;


axis2_description_operation_t 
		*axis2_description_operation_create (axis2_environment_t *env);

axis2_description_operation_t 
		*create_axis2_description_operation_with_param_include (
		, axis2_environment_t *env
		, axis2_description_param_include_t* param_include);

axis2_description_operation_t *create_axis2_description_operation_with_name 
		(axis2_environment_t *env
		, axis2_description_param_include_t *param_include
		, axis2_qname_t *name);

int axis2_description_operation_free (axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);

int axis2_description_operation_add_parameter(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc
		, axis2_description_param_t *param);

axis2_description_param_t *axis2_description_operation_get_parameter
		(axis2_description_operation_t *operation_desc
		, const axis2_char_t *name);

axis2_description_param_t *axis2_description_operation_get_params
		(axis2_description_operation_t *operation_desc);

int axis2_description_operation_set_parent(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc
		, axis2_description_service_t *service_desc);

axis2_service_description_t *axis2_description_operation_get_parent
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);

#endif //AXIS2_DESCRIPTION_OPERATION_H
