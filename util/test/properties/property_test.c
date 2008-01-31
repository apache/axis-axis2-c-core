#include <stdio.h>
#include <axutil_env.h>
#include "../util/create_env.h"
#include <axutil_properties.h>

axutil_hash_t* all_properties = NULL;
axis2_char_t* cur = NULL;
axis2_char_t* input_filename = "test.doc";
axutil_env_t *env = NULL;
axutil_properties_t * properties = NULL;
axis2_status_t  store_properties ;
axis2_status_t  load_properties ;
axis2_char_t * key = "key";
axis2_char_t * value = "value";

/** @brief test properties
  * read file and give the output
  */
test_properties()
{ 
    FILE *input = fopen("input.doc","rb");
    FILE *output = fopen("output.doc","rb");
    env = create_environment();
    if(!env)
    {
    printf("Environment is not cteated\n");
    }
    properties = axutil_properties_create(env);
    if(!properties)
    {
    printf("Properties are not created\n");
    }
    if(properties)
    {
    printf("Properties are created\n");
    }
    cur = (axis2_char_t*)axutil_properties_read(input,env);
    if(!cur)
    {
    printf("Can't read properties\n");
    }
    axutil_properties_read_next(cur);
    axutil_properties_set_property(properties,env, key, value);
    store_properties = axutil_properties_store(properties,env,output);
    if(!store_properties)
    {
    printf("Can not store the properties\n");
    }
    if(store_properties)
    {
    printf("Stored the properties\n");
    }
    load_properties = axutil_properties_load(properties,env,input_filename);   
    if(!load_properties)
    {
    printf("Properties not loaded\n");
    }
    if(load_properties)
    {
    printf("Properties are loaded\n");
    }
    all_properties = axutil_properties_get_all(properties,env);
    if(!all_properties)
    {
    printf("Can't call properties_get_all\n");
    }
    if(all_properties)
    {
    printf((char*)cur);
    printf("The test is success\n");
    }
    return AXIS2_SUCCESS;
}

int main()
{
    int status = AXIS2_SUCCESS;

    env = create_environment();

    status = test_properties();

    if(status == AXIS2_FAILURE)
    {
        printf(" The test is failed\n");
    }
    axutil_env_free(env);
    return 0;
}









