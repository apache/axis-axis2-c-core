#include "axis2c_om_attribute.h"
#include "string.h"


om_attribute_t *create_om_attribute(const char *localname,const char *value,om_namespace_t *ns)
{
	om_attribute_t *attr = (om_attribute_t *)malloc(sizeof(om_attribute_t));
	if(!attr)
	{
		//fprintf(stderr,"error ");
		return NULL;
	}

	attr->localname = strdup(localname);
	attr->value		= strdup(value);
	attr->ns		= ns;
	return attr;
}

void om_attribute_free(om_attribute_t *attr)
{
	if(attr)
	{
		free(attr->localname);
		free(attr->value);
		// should namespace pointer be freeed ...

	
	}

}

qname_t *om_attribute_get_qname(om_attribute_t *attr)
{	qname_t* qn=NULL;
	if(attr)
	{
		if(attr->ns)
		{
			qn=create_qname(attr->localname,attr->ns->uri,attr->ns->prefix);
		}
		else
		{
			qn=create_qname(attr->localname,NULL,NULL);
		}
		return qn;
	}
	return NULL;

}





