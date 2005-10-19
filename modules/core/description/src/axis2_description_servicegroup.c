#include <axis2_description_servicegroup.h>

/** @struct axis2_description_servicegroup_s
  * @brief DESCRIPTION servicegroup
  * This holds the information about the servicegroup_name
  */
struct axis2_description_servicegroup_s
{
	axis2_description_param_include_t *param_include;
	axis2_qname_t *servicegroup_name;
	axis2_hash_t *services;
	
};

/***************************** Function headers *******************************/

axis2_status_t axis2_description_servicegroup_ops_add_service
		(axis2_environment_t *env
		, axis2_description_servicegroup_t* service_group);

/***************************** End of function headers ************************/

axis2_description_servicegroup_ops_t *axis2_description_servicegroup_get_ops
		(axis2_environment_t *env);

/******************************************************************************/

axis2_status_t axis2_description_servicegroup_ops_add_service
		(axis2_environment_t *env
		, axis2_description_servicegroup_t* service_group)
{
	return 0;
}
