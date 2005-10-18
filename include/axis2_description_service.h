#ifndef AXIS2_DESCRIPTION_SERVICE_H
#define AXIS2_DESCRIPTION_SERVICE_H

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>

#include <axis2_description_param_include.h>
#include <axis2_description_operation.h>
#include <axis2_description_servicegroup.h>

struct axis2_description_service_s;
struct axis2_description_service_ops_s;
typedef axis2_description_service_s axis2_description_service_t;
typedef axis2_description_service_ops_s axis2_description_service_ops_t;

axis2_description_operation_t *axis2_description_service_get_operation_with_qname(
	axis2_description_service_t *service_desc, axis2_qname_t *operation_name);

void axis2_description_service_add_operation(axis2_description_service_t
	*service_desc, axis2_description_operation_t *operation_desc);

void axis2_description_service_add_parameter(axis2_description_service_t
	*operation_desc, axis2_parameter_t *param);

axis2_parameter_t *axis2_description_service_get_parameter(
	axis2_description_service_t *service_desc, axis2_char_t *name);

axis2_parameter_t *axis2_description_service_get_parameters(
	axis2_description_operation_t *service_desc);

axis2_description_operation_t *axis2_description_service_get_operation_with_char(
	axis2_description_service_t *service_desc, axis2_char_t* operation_name);

apr_hash_t *axis2_description_service_get_operations(
	axis2_description_service_t *service_desc);
	
void setParent(axis2_description_service_t *service_description
	,axis2_servicegroup_description_t *servicegroup_desc);

axis2_servicegroup_description_t *axis2_description_service_getParent
(axis2_description_service_t *service_desc);

axis2_description_service_t *axis2_description_service_create_with_qname
	(axis2_environment_t *env);

#endif /* AXIS2_DESCRIPTION_SERVICE_H */
