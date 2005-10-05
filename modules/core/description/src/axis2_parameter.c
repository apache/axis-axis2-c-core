#include <axis2_parameter.h>

axis2_parameter_t *axis2_parameter_create ()
{
	axis2_parameter_t *parameter = (axis2_parameter_t *) malloc 
		(sizeof(axis2_parameter_t));
}	
	
axis2_parameter_t * axis2_parameter_create_with_name_value (const char *name, char *value)
{
	
	axis2_parameter_t *parameter = (axis2_parameter_t *) malloc 
	(sizeof(axis2_parameter_t));
	if(parameter)
	{
		parameter->name = strdup (name);
		parameter->value = strdup (value);
		parameter->param_element = NULL;
		parameter->type = AXIS2_PARAMETER_TEXT;
		parameter->is_locked = FALSE;
		return parameter;
	}
			
	return NULL;
}

void axis2_parameter_free (axis2_parameter_t * param)
{
	if(param)
		free(param);
}

void axis2_parameter_set_name (axis2_parameter_t * param, char *name)
{
	if(param)
	{
		param->name = strdup(name);
	}		
}

char *axis2_parameter_get_name (axis2_parameter_t * param)
{
	if(param)
		return param->name;
	return NULL;
}

void axis2_parameter_set_value (axis2_parameter_t * param, void *value)
{
	if(param)
		param->value = value;
}	

void *axis2_parameter_get_value (axis2_parameter_t * param)
{
	if(param)
		param->value;
	return NULL;
}

int axis2_parameter_is_locked (axis2_parameter_t * param)
{
	if(param)
		return param->is_locked;
	return FALSE;
}

void axis2_parameter_set_locked (axis2_parameter_t * param,
                                  int is_locked)
{
	if(param)
		param->is_locked = is_locked;
}

void axis2_parameter_set_param_element (axis2_parameter_t * param,
                                         axis2_om_element_t * om_element)
{
	if(param)
		param->param_element = om_element;
	
}

axis2_parameter_t *axis2_parameter_get_param_element (axis2_parameter_t *
                                                        param)
{
	if(param)
		return (axis2_parameter_t *) param->param_element;
	return NULL;
}

void axis2_parameter_set_type (axis2_parameter_t * param, int type)
{
	if(param)
		param->type = type;
}

int axis2_parameter_get_type (axis2_parameter_t * param)
{
	if(param)
		return param->type;
	return AXIS2_ERROR_NULL_MEMORY_ACCESS;
}
