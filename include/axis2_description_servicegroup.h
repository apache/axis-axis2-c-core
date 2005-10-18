#ifndef AXIS2_DESCRIPTION_SERVICEGROUP_H
#define AXIS2_DESCRIPTION_SERVICEGROUP_H

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>

#include <axis2_description_param_include.h>
#include <axis2_description_service.h>

struct axis2_description_servicegroup_s;
struct axis2_description_servicegroup_ops_s;
typedef axis2_description_servicegroup_s axis2_description_servicegroup_t;
typedef axis2_description_servicegroup_ops_s 
		axis2_description_servicegroup_ops_t;

void axis2_description_servicegroup_add_service(axis2_environment_t *env
		, axis2_description_servicegroup_t* service_group);


#endif /* AXIS2_DESCRIPTION_SERVICEGROUP_H	*/
