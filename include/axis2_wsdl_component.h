#ifndef AXIS2_WSDL_COMPONENT_H
#define AXIS2_WSDL_COMPONENT_H
/**
 * @file axis2_wsdl_component.h
 * @brief axis2 WSDL  component
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
/** @cond */
struct axis2_wsdl_component_ops_s;
struct axis2_wsdl_component_s;
/** @endcond */
typedef struct axis2_wsdl_component_ops_s axis2_wsdl_component_ops_t;
typedef struct axis2_wsdl_component_s axis2_wsdl_component_t;
	

/** @defgroup axis2_wsdl WSDL (Axis2 wsdl)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @defgroup axis2_wsdl_component WSDL Component
 * @ingroup axis2_wsdl 
 * @{
 */

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_COMPONENT_FREE(env, wsdl_component) \
		((wsdl_component->ops)->free (env, wsdl_component))

#define AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTIES(env, wsdl_component \
		, properties) \
		((wsdl_component->ops)->set_component_properties(env \
		, wsdl_component, properties))
	
#define AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTIES(env, wsdl_component \
		, properties) \
		((wsdl_component->ops)->get_component_properties(env \
		, wsdl_component, properties))

#define AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(env, wsdl_component \
		, key, value) \
		((wsdl_component->ops)->set_component_property(env \
		, wsdl_component, key, value))
		
#define AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(env, wsdl_component \
		, key, value) \
		((wsdl_component->ops)->get_component_property(env \
		, wsdl_component, key, value))
		
/**************************** End of function macros **************************/
/**************************** Function pointers *******************************/

/** Deallocate memory
 * @return status code
 */
typedef axis2_status_t (*axis2_wsdl_component_free_t)
		(axis2_env_t *env
		, axis2_wsdl_component_t *wsdl_component);

/**
 * Returns the properties that are specific to this WSDL Component
 * @return axis2_hash_t** component properties
 */
typedef axis2_status_t (*axis2_wsdl_component_get_component_properties_t)
		(axis2_env_t *env, axis2_wsdl_component_t *wsdl_component
		, axis2_hash_t **properties);

/**
 * Sets the properties of the Component if any
 * @param axis2_hash_t* properties
 */
typedef axis2_status_t (*axis2_wsdl_component_set_component_properties_t) 
		(axis2_env_t *env, axis2_wsdl_component_t *wsdl_component
		, axis2_hash_t *properties);

/**
 * Will set the property keyed with the relavent key
 * @param void* Key in the map
 * @param void* value to be put
 */
typedef axis2_status_t (*axis2_wsdl_component_set_component_property_t) 
	(axis2_env_t *env, axis2_wsdl_component_t *wsdl_component
	, const void *key, void *value);
	

/**
 * Gets the component property
 *
 * @param void* key for the map search.
 * @return void* value for the key
 */
typedef axis2_status_t (*axis2_wsdl_component_get_component_property_t) 
		(axis2_env_t *env, axis2_wsdl_component_t *wsdl_component
		, const void *key, void **value) ;

		
/*************************** End of function pointers *************************/

struct axis2_wsdl_component_ops_s
{
	axis2_wsdl_component_free_t free;
	axis2_wsdl_component_set_component_properties_t set_component_properties;
	axis2_wsdl_component_get_component_properties_t get_component_properties;
	axis2_wsdl_component_set_component_property_t set_component_property;
	axis2_wsdl_component_get_component_property_t get_component_property;

};

struct axis2_wsdl_component_s
{
	axis2_wsdl_component_ops_t *ops;
};

/** create a wsdl component
 * @return axis2_wsdl_component_t ** allocated component
 */
axis2_status_t axis2_wsdl_component_create (axis2_env_t *env
		, axis2_wsdl_component_t **wsdl_component);

/** @} */
#ifdef __cplusplus
}
#endif

#endif /*AXIS2_WSDL_COMPONENT_H*/
