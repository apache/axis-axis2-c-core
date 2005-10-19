#include <axis2_description_service.h>

/**
  * @struct axis2_description_service
  * @brief DESCRIPTION service struct
  * This holds the information about service
  */
struct axis2_description_service_s
{
	axis2_description_service_ops_t *ops;
	axis2_description_param_include_t *param_include;
	axis2_description_servicegroup_t *parent;
	axis2_hash_t *wasaction_opeartionmap;    

};

/*************************** Function pointers ********************************/

axis2_status_t axis2_description_service_ops_free (axis2_environment_t *env
		, axis2_description_service_t *service_desc);

axis2_status_t axis2_description_service_ops_add_operation
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_description_operation_t *operation_desc);

axis2_status_t axis2_description_service_ops_add_param(axis2_environment_t *env
		, axis2_description_service_t *operation_desc
		, axis2_description_param_t *param);

axis2_description_param_t *axis2_description_service_ops_get_param
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t *name);

axis2_hash_t *axis2_description_service_ops_get_params
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

axis2_description_operation_t *
		axis2_description_service_ops_get_operation_with_qname(
		axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_qname_t *operation_name);
		
axis2_description_operation_t *
		axis2_description_service_ops_get_operation_with_name
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t* operation_name);

axis2_hash_t *axis2_description_service_ops_get_operations(
		axis2_environment_t *env, axis2_description_service_t *service_desc);
	
axis2_status_t axis2_description_service_ops_set_parent(axis2_environment_t *env
		, axis2_description_service_t *service_desc
		,axis2_description_servicegroup_t *servicegroup_desc);

axis2_description_servicegroup_t *axis2_description_service_ops_get_parent
		(axis2_environment_t *env, axis2_description_service_t *service_desc);
		
/************************* End of function pointers ***************************/

axis2_description_service_ops_t * axis2_description_service_get_ops
		(axis2_environment_t *env)
{
	return NULL;
}

axis2_description_service_t *axis2_description_service_create_with_qname
		(axis2_environment_t *env)
{
	return NULL;
}

/******************************************************************************/

axis2_status_t axis2_description_service_ops_free
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	if(service_desc)
		axis2_free(env->allocator, service_desc);
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_description_service_ops_add_operation
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_description_operation_t *operation_desc)
{
	return AXIS2_ERROR_INVALID_NULL_PARAMETER;
}

axis2_status_t axis2_description_service_ops_add_param(axis2_environment_t *env
		, axis2_description_service_t *operation_desc
		, axis2_description_param_t *param)
{
	return NULL;
}

axis2_description_param_t *axis2_description_service_ops_get_param(
		axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t *name)
{
	return NULL;
}

axis2_hash_t *axis2_description_service_ops_get_params
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	return NULL;
}

axis2_description_operation_t 
		*axis2_description_service_ops_get_operation_with_qname
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_qname_t *operation_name)
{
	return NULL;	
}	

axis2_description_operation_t 
		*axis2_description_service_ops_get_operation_with_name
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t* name)
{
	return NULL;
}

axis2_hash_t *axis2_description_service_ops_get_operations
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	return NULL;
}
	
axis2_status_t axis2_description_operation_ops_set_parent
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		,axis2_description_servicegroup_t *servicegroup_desc)
{
	return NULL;
}

axis2_description_servicegroup_t *axis2_description_service_ops_get_parent
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	return NULL;
}
