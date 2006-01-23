#include "test_wsdl_component.h"
#include <axis2_string.h>

void Testaxis2_wsdl_component_set_component_properties(CuTest *tc)
{
    axis2_char_t *expected = NULL;
    axis2_char_t *actual = NULL;
    
    printf("**************************************************************\n");
    printf("testing axis2_wsdl_component_set_component_properties  method \n");
    printf("**************************************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create(allocator);

    CuAssertStrEquals(tc, expected, actual);
}
