#include "util_class_loader_test.h"
#include <axis2_url.h>
#include <axis2_utils.h>
#include <axis2_string.h>
#include <stdio.h>

void Testaxis2_url_parse_string(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = NULL;
    axis2_url_t *url = NULL;
    axis2_char_t *url_str = NULL;

    printf("******************************************\n");
    printf("testing axis2_url_parse_string\n");
    printf("******************************************\n");
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);
   
    url_str = AXIS2_STRDUP("file://home/damitha/temp/checkout.sh", env);
    url = axis2_url_parse_string(env, url_str);
   
    actual = AXIS2_URL_TO_EXTERNAL_FORM(url, env);
    printf("actual:%s\n", actual);
    
    CuAssertStrEquals(tc, expected, actual);
}
