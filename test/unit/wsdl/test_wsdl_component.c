#include "test_wsdl_component.h"
#include <axis2_string.h>
#include <axis2_module_desc.h>
#include <axis2_hash.h>
#include <axis2_array_list.h>

axis2_array_list_t *
get_module_list(axis2_env_t **env)
{
    axis2_array_list_t *module_list = NULL;
    axis2_module_desc_t *module_desc = NULL;
    
    module_list = axis2_array_list_create(env, 0);
    if(!module_list)
    {
        return NULL;
    }
    module_desc = axis2_module_desc_create(env);
    AXIS2_ARRAY_LIST_ADD(module_list, env, module_desc);
    module_desc = NULL;
    module_desc = axis2_module_desc_create(env);
    AXIS2_ARRAY_LIST_ADD(module_list, env, module_desc);
    module_desc = NULL;
    module_desc = axis2_module_desc_create(env);
    AXIS2_ARRAY_LIST_ADD(module_list, env, module_desc);
    module_desc = NULL;

    return module_list;
}

axis2_hash_t *
get_module_list_map(axis2_env_t **env)
{
    axis2_hash_t *module_list_map = NULL;
    axis2_array_list_t *module_list = NULL;
    
    module_list_map = axis2_hash_make(env);
    if(!module_list_map)
    {
        return NULL;
    }
    module_list = get_module_list(env);
    axis2_hash_set(module_list_map, AXIS2_MODULEREF_KEY, 
        AXIS2_HASH_KEY_STRING, module_list);
    module_list = NULL;

    module_list = get_module_list(env);
    axis2_hash_set(module_list_map, AXIS2_MODULEREF_KEY, 
        AXIS2_HASH_KEY_STRING, module_list);
    module_list = NULL;
    
    module_list = get_module_list(env);
    axis2_hash_set(module_list_map, AXIS2_MODULEREF_KEY, 
        AXIS2_HASH_KEY_STRING, module_list);
    module_list = NULL;

    return module_list_map;
}

void Testaxis2_wsdl_component_set_component_properties(CuTest *tc)
{
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_status_t actual = AXIS2_FAILURE;
    axis2_hash_t *module_list_map = NULL;
    axis2_wsdl_component_t *wsdl_comp = NULL;
    
    printf("**************************************************************\n");
    printf("testing axis2_wsdl_component_set_component_properties  method \n");
    printf("**************************************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create(allocator);

    wsdl_comp = axis2_wsdl_component_create(&env);
    CuAssertPtrNotNull(tc, wsdl_comp);
    module_list_map = get_module_list_map(&env);
    CuAssertPtrNotNull(tc, module_list_map);
    actual = AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTIES(wsdl_comp, &env, module_list_map);
    /* again create a module_list_map and assign to see whether it works correctly */
    module_list_map = NULL;
    module_list_map = get_module_list_map(&env);
    CuAssertPtrNotNull(tc, module_list_map);
    actual = AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTIES(wsdl_comp, &env, module_list_map);
    /*AXIS2_WSDL_COMPONENT_FREE(wsdl_comp, &env);*/
   
    CuAssertIntEquals(tc, expected, actual);
}


