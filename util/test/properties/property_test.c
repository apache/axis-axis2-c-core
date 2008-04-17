#include <stdio.h>
#include <axutil_env.h>
#include "../util/create_env.h"
#include <axutil_properties.h>

axis2_char_t *
axutil_properties_read(
    FILE *input,
    const axutil_env_t *env);

/** @brief test properties
  * read file and give the output
  */
axis2_status_t test_properties(axutil_env_t *env)
{ 
    axutil_hash_t* all_properties = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t* cur = NULL;
    axis2_char_t* input_filename = "test.doc";
    axutil_properties_t * properties = NULL;
    axis2_status_t  store_properties ;
    axis2_status_t  load_properties ;
    axis2_char_t * key = "key";
    axis2_char_t * value = "value";
    FILE *input = fopen("input.doc","rb");
    FILE *output = fopen("output.doc","rb");
    if (!(input && output))
    {
        return AXIS2_FAILURE;
    }
    
    properties = axutil_properties_create(env);
    if(!properties)
    {
        printf("Properties are not created\n");
        axutil_property_free(properties,env);
        return AXIS2_FAILURE;
    }
    else
    printf("The the axutil_properties_create is successfull\n");
 
    cur = axutil_properties_read(input,env);
    if(!cur)
    {
        printf("Can't read properties\n");
        axutil_property_free(properties,env);
        return AXIS2_FAILURE;
    }
    else
    printf("The test axutil_properties_read is successfull\n");

    status = axutil_properties_set_property(properties,env, key, value);
    if (status == AXIS2_SUCCESS)
        printf("The test axutil_properties_set_property is successful\n");
    else
        printf("The test axutil_properties_set_property failed\n") ;

  
    store_properties = axutil_properties_store(properties,env,output);
    if(!store_properties)
    {
        printf("Can not store the properties\n");
        axutil_property_free(properties,env);
        return AXIS2_FAILURE;
    }
    else 
    printf("The test axutil_properties_store is successfull\n");
    
    load_properties = axutil_properties_load(properties,env,input_filename);   
    if(!load_properties)
    {
        printf("Properties can't be loaded\n");
        axutil_property_free(properties,env);
        return AXIS2_FAILURE;
    }
    else 
    printf("The test axutil_properties_load is successfull\n");
    
    all_properties = axutil_properties_get_all(properties,env);
    if(!all_properties)
    {
        printf("Can't call properties_get_all\n");
        axutil_property_free(properties,env);
        return AXIS2_FAILURE;
    }
    else
    printf("The test axutil_properties_get_all is successfull\n");
    
    axutil_property_free(properties,env);   
 
    return AXIS2_SUCCESS;
}

int main()
{
    axutil_env_t *env = NULL;
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_properties(env);
    
    if(status == AXIS2_FAILURE)
    {
        printf(" The test is failed\n");
    }
    
    axutil_env_free(env);
    return 0;
}









