#include <axis2_uri.h>
#include <axis2_utils.h>
#include <axis2_string.h>
#include "util_uri_test.h"
#include <stdio.h>

void Testaxis2_uri_parse_string(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = "/home/damitha/temp/checkout.sh";
    axis2_uri_t *uri = NULL;
    axis2_char_t *uri_str = NULL;

    printf("******************************************\n");
    printf("testing axis2_uri_parse_string\n");
    printf("******************************************\n");
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);

    /*uri_str = "http://myserver:80/home/damitha/temp/checkout.sh";*/
    /*uri_str = "../temp/checkout.sh";*/
    uri_str = "temp/checkout.sh";
    printf("came1\n");
    uri = axis2_uri_parse_string(env, uri_str);
    CuAssertPtrNotNull(tc, uri);
    if(uri->ops)
        printf("came11\n");
    CuAssertPtrNotNull(tc, uri->ops);
    printf("came2\n");

    actual = AXIS2_URI_TO_STRING(uri, env, AXIS2_URI_UNP_OMITSITEPART);
    printf("came3\n");
   
    printf("actual:%s\n", actual);
    
    CuAssertStrEquals(tc, expected, actual);
}

void Testaxis2_uri_parse_relative(CuTest *tc)
{
    axis2_char_t *actual = NULL;
    axis2_char_t *expected = "http://myserver/temp/checkout.sh";
    axis2_uri_t *new_uri = NULL;
    axis2_uri_t *base = NULL;
    axis2_char_t *uri_str = NULL;
    axis2_char_t *base_str = NULL;

    printf("******************************************\n");
    printf("testing axis2_uri_parse_relative\n");
    printf("******************************************\n");
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);

    base_str = "http://myserver:80/home/damitha";
    uri_str = "../../temp/checkout.sh";
    base = axis2_uri_parse_string(env, base_str);
    new_uri = axis2_uri_parse_relative(env, base, uri_str);
    actual = AXIS2_URI_TO_STRING(new_uri, env, AXIS2_URI_UNP_OMITPASSWORD);
   
    printf("actual:%s\n", actual);
    
    CuAssertStrEquals(tc, expected, actual);
}

