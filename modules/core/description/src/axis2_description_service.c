#include <axis2_description_service.h>

/**
  * @struct axis2_description_service
  * @brief DESCRIPTION service struct
  * This holds the information about service
  */
struct axis2_service_description_s
{
	axis2_description_param_include_t *param_include;
	axis2_servicegroup_description_t *parent;
	axis2_hash_t *wasaction_opeartionmap;    

};

axis2_description_operation_t 
	*axis2_description_service_get_operation_with_qname(
	axis2_description_service_t *service_desc, axis2_qname_t *operation_name)
{
	return NULL;	
}	

void axis2_description_service_add_operation(axis2_description_service_t
	*service_desc, axis2_description_operation_t *operation_desc)
{

}

void axis2_description_service_add_parameter(axis2_description_service_t
	*operation_desc, axis2_parameter_t *param)
{

}

axis2_description_param_t *axis2_description_service_get_parameter(
	axis2_description_service_t *service_desc, char *name)
{
	return NULL;
}

axis2_description_param_t *axis2_description_service_get_params(
	axis2_description_service_t *service_desc)
{
	return NULL;
}

axis2_description_operation_t 
	*axis2_description_service_get_operation_with_char(
	axis2_description_service_t *service_desc, char* name)
{
	return NULL;
}

axis2_hash_t *axis2_description_service_get_operations(
	axis2_description_service_t *service_desc)
{
	return NULL;
}
	
void setParent(axis2_description_service_t *service_desc
	,axis2_description_servicegroup_t *servicegroup_desc)
{
	
}

axis2_description_servicegroup_t *axis2_description_service_getParent
(axis2_description_service_t *service_desc)
{
	return NULL;
}
