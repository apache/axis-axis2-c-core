#include <axis2_service_description.h>

axis2_operation_description_t 
	*axis2_service_description_get_operation_with_qname(
	axis2_service_description_t *service_desc, axis2_qname_t *operation_name)
{
	return NULL;	
}	

void axis2_service_description_add_operation(axis2_service_description_t
	*service_desc, axis2_operation_description_t *operation_desc)
{

}

void axis2_service_description_add_parameter(axis2_service_description_t
	*operation_desc, axis2_parameter_t *param)
{

}

axis2_parameter_t *axis2_service_description_get_parameter(
	axis2_service_description_t *service_desc, char *name)
{
	return NULL;
}

axis2_parameter_t *axis2_service_description_get_parameters(
	axis2_operation_description_t *service_desc)
{
	return NULL;
}

axis2_operation_description_t 
	*axis2_service_description_get_operation_with_char(
	axis2_service_description_t *service_desc, char* name)
{
	return NULL;
}

apr_hash_t *axis2_service_description_get_operations(
	axis2_service_description_t *service_desc)
{
	return NULL;
}
	
void setParent(axis2_service_description_t *service_description
	,axis2_servicegroup_description_t *servicegroup_desc)
{
	
}

axis2_servicegroup_description_t *axis2_service_description_getParent
(axis2_service_description_t *service_desc)
{
	return NULL;
}
