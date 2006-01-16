#include <axis2_msg_info_headers.h>
#include <axis2_env.h>
#include <axis2_allocator.h>

int axis2_test_msg_info_headers_create()
{
	struct axis2_end_point_ref *to = NULL;
	axis2_msg_info_headers_t *axis2_msg_info_headers = NULL;
	axis2_allocator_t *allocator = axis2_allocator_init (NULL);
	axis2_env_t *env = axis2_env_create (allocator);
	axis2_char_t *address = "test address";
	axis2_char_t *action = "test actioin";

	to = axis2_endpoint_ref_create(&env, address);

	axis2_msg_info_headers = axis2_msg_info_headers_create(&env, to, action);
	if (axis2_msg_info_headers)
	{
		printf("SUCCESS\n");
		return 0;
	}	
	else
	{
		printf("ERROR\n");
		return -1;
	}
	return 0;
}

int main()
{
	axis2_test_msg_info_headers_create();
	return 0;
}
