#include <axis2.h>

apr_hash_t *axis2_apr_hash_make(apr_pool_t *om_pool)
{
	apr_status_t status;

	if (!om_pool)
	{
		status = apr_pool_create (&om_pool, NULL);
		// error handling should go hear
	}
		
	return apr_hash_make (om_pool);
}
