#ifndef AXIS2_WSDL_COMPONENT_H
#define AXIS2_WSDL_COMPONENT_H

/**
 * @file axis2_wsdl_component.h
 * @brief axis2 wsdl component interface. 
 */
#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>
#include <axis2_linked_list.h>
#include <axis2_array_list.h>
#include <axis2_wsdl_extensible_element.h>


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_extensible_element;
struct axis2_param_container;   
struct axis2_module_desc;
typedef struct axis2_wsdl_component_ops axis2_wsdl_component_ops_t;
typedef struct axis2_wsdl_component axis2_wsdl_component_t;
   
/** @defgroup axis2_wsdl_component Wsdl Component
  * @ingroup axis2_wsdl
  * @{
  */

/** 
 * @brief Wsdl Component ops struct
 * Encapsulator struct for ops of axis2_wsdl_component
 */
struct axis2_wsdl_component_ops
{
   /** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_wsdl_component_t *wsdl_component, 
           const axis2_env_t *env);
    
   /**
     * Sets the properties of the Component if any
     * @param axis2_hash_t properties
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    set_component_properties) (axis2_wsdl_component_t *wsdl_component, 
                                const axis2_env_t *env, 
                                axis2_hash_t *properties);
    

    /**
     * Returns the properties that are specific to this WSDL Component
     * @return axis2_hash_t component properties
     */
    axis2_hash_t *(AXIS2_CALL *
    get_component_properties) (axis2_wsdl_component_t *wsdl_component, 
                               const axis2_env_t *env);
    
    /**
     * Will set the property keyed with the relavent key
     * @param void Key in the map
     * @param void value to be put
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    set_component_property) (axis2_wsdl_component_t *wsdl_component,
                                const axis2_env_t *env, 
                                axis2_char_t *key, 
                                void *value);        
                                
    
    /**
     * Gets the component property
     * @param void key for the map search.
     * @return void value for the key
     */
    void *(AXIS2_CALL *
    get_component_property) (axis2_wsdl_component_t *wsdl_component, 
                                const axis2_env_t *env,
                                axis2_char_t *key);
 


    /**
     * Adds the <code>Element</code> to this Component.
     *
     * @param element
     */
    axis2_status_t (AXIS2_CALL *
    add_extensibility_element) (axis2_wsdl_component_t *wsdl_component,
                                const axis2_env_t *env,
                                void *element);
                                                        
    /**
     * Returns the Extensibility Elements of this component;
     *
     * @return List of <code>Element</code> s
     */
    axis2_linked_list_t *(AXIS2_CALL *
    get_extensibility_elements) (axis2_wsdl_component_t *wsdl_component,
                                                    const axis2_env_t *env);
                                                
};

/**
 * @brief Wsdl Component struct
 * Axis2 Wsdl Component
 * Component is used to hold component properties
 */
struct axis2_wsdl_component
{
   axis2_wsdl_component_ops_t *ops;
};

/** create Wsdl Component struct
 * @return pointer to newly created wsdl component
 */
AXIS2_EXTERN axis2_wsdl_component_t * AXIS2_CALL 
axis2_wsdl_component_create (const axis2_env_t *env);

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_COMPONENT_FREE(wsdl_component, env) \
      ((wsdl_component)->ops->free (wsdl_component, env))

#define AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTIES(wsdl_component, env, \
      properties) \
      ((wsdl_component)->ops->set_component_properties(wsdl_component , env, \
            properties))
            
#define AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTIES(wsdl_component, env) \
      ((wsdl_component)->ops->get_component_properties(wsdl_component, env))

#define AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(wsdl_component, env, \
      key, value) \
      ((wsdl_component)->ops->set_component_property(wsdl_component, env,\
            key, value))            
      
#define AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(wsdl_component, env, key) \
      ((wsdl_component)->ops->get_component_property(wsdl_component, env, key))


#define AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_component, env, element) \
      ((wsdl_component)->ops->add_extensibility_element(wsdl_component, env, element))
 
#define AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(wsdl_component, env) \
      ((wsdl_component)->ops->get_extensibility_elements(wsdl_component, env))
        
/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif /*AXIS2_WSDL_COMPONENT_H*/
