#ifndef AXIS2_DESCRIPTION_SERVICEGROUP_H
#define AXIS2_DESCRIPTION_SERVICEGROUP_H

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

/**************************** Function pointers *******************************/

typedef axis2_status_t (*axis2_description_servicegroup_add_service_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t* service_group);

/*************************** End of function pointers *************************/

struct axis2_description_servicegroup_ops_s
{
	axis2_description_servicegroup_add_service_t add_service;	
};

/******************************************************************************/

axis2_description_servicegroup_ops_t *axis2_description_servicegroup_get_ops
		(axis2_environment_t *env);

#endif /* AXIS2_DESCRIPTION_SERVICEGROUP_H	*/
