#ifndef AXIS2_WSDL_COMPONENT_H
#define AXIS2_WSDL_COMPONENT_H

/**
 * @file axis2_wsdl_component.h
 * @brief axis2 wsdl component interface. 
 */
#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
typedef struct axis2_wsdl_component_ops_s axis2_wsdl_component_ops_t;
typedef struct axis2_wsdl_component_s axis2_wsdl_component_t;
	
/** @defgroup axis2_wsdl_component Wsdl Component
  * @ingroup axis2_wsdl
  * @{
  */

/** 
 * @brief Wsdl Component operations struct
 * Encapsulator struct for operations of axis2_wsdl_component
 */
struct axis2_wsdl_component_ops_s
{
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free) (axis2_wsdl_component_t *wsdl_component, 
                                        axis2_env_t **env);
    
	/**
     * Sets the properties of the Component if any
     * @param axis2_hash_t properties
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *set_component_properties) 
                                        (axis2_wsdl_component_t *wsdl_component, 
                                            axis2_env_t **env, 
                                            axis2_hash_t *properties);
    
    /**
     * Returns the properties that are specific to this WSDL Component
     * @return axis2_hash_t component properties
     */
    axis2_hash_t *properties (AXIS2_CALL *get_component_properties)
                                        (axis2_wsdl_component_t *wsdl_component, 
                                        axis2_env_t **env);
    
    /**
     * Will set the property keyed with the relavent key
     * @param void Key in the map
     * @param void value to be put
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *set_component_property) (
                                        axis2_wsdl_component_t *wsdl_component,
	                                    axis2_env_t **env, 
	                                    const void *key, 
                                        void *value);
    
    /**
     * Gets the component property
     * @param void key for the map search.
     * @return void value for the key
     */
    void *(AXIS2_CALL *get_component_property) 
                                        (axis2_wsdl_component_t *wsdl_component, 
                                            axis2_env_t **env,
                                            const void *key);
    
};

/**
 * @brief Wsdl Component struct
 * Axis2 Wsdl Component
 * Component is used to hold component properties
 */
struct axis2_wsdl_component_s
{
	axis2_wsdl_component_ops_t *ops;
};

/** create Wsdl Component struct
 * @return pointer to newly created wsdl component
 */
AXIS2_DECLARE(axis2_wsdl_component_t *) 
axis2_wsdl_component_create (axis2_env_t **env);

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_COMPONENT_FREE(wsdl_component, env) \
		((wsdl_component->ops)->free (wsdl_component, env))

#define AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTIES(wsdl_component, env, \
		properties) \
		((wsdl_component->ops)->set_component_properties(wsdl_component , env, \
            properties))
	
#define AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTIES(wsdl_component, env, \
		properties) \
		((wsdl_component->ops)->get_component_properties(wsdl_component, env, \
		    properties))

#define AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(wsdl_component, env, \
		key, value) \
		((wsdl_component->ops)->set_component_property(env , wsdl_component, \
            key, value))
		
#define AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(wsdl_component, env, \
		key, value) \
		((wsdl_component->ops)->get_component_property(wsdl_component, env, \
            key, value))
		
/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif /*AXIS2_WSDL_COMPONENT_H*/
