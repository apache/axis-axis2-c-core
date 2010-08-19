/*
* Licensed to the Apache Software Foundation (ASF) under one or more
* contributor license agreements.  See the NOTICE file distributed with
* this work for additional information regarding copyright ownership.
* The ASF licenses this file to You under the Apache License, Version 2.0
* (the "License"); you may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <axutil_env.h>
#include "../util/create_env.h"
#include <axutil_string.h>
#include <axutil_properties.h>

void printProperties(axutil_properties_t * properties, axutil_env_t *env)
{
    axutil_hash_t* all_properties = NULL;
    all_properties = axutil_properties_get_all(properties,env);
    if(all_properties)
    {
       axutil_hash_index_t *hi = NULL;
       axis2_char_t *key = NULL;
       axis2_char_t *value = NULL;
       for(hi = axutil_hash_first(all_properties, env); hi; hi = axutil_hash_next(env, hi))
        {
          axutil_hash_this(hi, (void *)&key, NULL, (void *)&value);
            if(key)
            {
                if(!value)
                {
                    value = axutil_strdup(env, "");
                }
                printf("%s=%s\n", key, value);
            }
        }
    }
}
/** @brief test properties
  * read file and give the output
  */
axis2_status_t test_properties(axutil_env_t *env)
{ 
    axutil_hash_t* all_properties = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t* input_filename = "test.doc";
    axutil_properties_t * properties = NULL;
    axis2_status_t  store_properties ;
    axis2_status_t  load_properties ;
    axis2_char_t * key = "key";
    axis2_char_t * value = "value";
    FILE *input = fopen("input.doc","rb");
    FILE *output = fopen("output.doc","wb");
	
    if (!input)
    {
        printf("Can't open input.doc\n");
        return AXIS2_FAILURE;
    }
    if (!output)
    {
        printf("Can't open output.doc\n");
        return AXIS2_FAILURE;
    }

    properties = axutil_properties_create(env);
    if(!properties)
    {
        printf("Properties are not created\n");
        axutil_properties_free(properties,env);
        return AXIS2_FAILURE;
    }
    else
    printf("The the axutil_properties_create is successfull\n");
    status = axutil_properties_set_property(properties,env, key, value);
    if (status == AXIS2_SUCCESS)
        printf("The test axutil_properties_set_property is successful\n");
    else
        printf("The test axutil_properties_set_property failed\n") ;
		
    printProperties(properties, env);
	store_properties = AXIS2_SUCCESS;
/* Not used, program aborts on Windows with MSVC (Visual Studio 2008 Express Edition)
	store_properties = axutil_properties_store(properties,env,output);
    if(!store_properties)
    {
        printf("Can not store the properties\n");
        axutil_properties_free(properties,env);
        return AXIS2_FAILURE;
    }
    else 
    printf("The test axutil_properties_store is successfull\n");
*/    
    load_properties = axutil_properties_load(properties,env,input_filename);   
    if(!load_properties)
    {
        printf("Properties can't be loaded\n");
        axutil_properties_free(properties,env);
        return AXIS2_FAILURE;
    }
    else 
    printf("The test axutil_properties_load is successfull\n");
    printProperties(properties, env);

    all_properties = axutil_properties_get_all(properties,env);
    if(!all_properties)
    {
        printf("Can't call properties_get_all\n");
        axutil_properties_free(properties,env);
        return AXIS2_FAILURE;
    }
    else
    printf("The test axutil_properties_get_all is successfull\n");
    
    axutil_properties_free(properties,env);   
 
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









