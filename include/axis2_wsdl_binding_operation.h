#ifndef AXIS2_WSDL_BINDING_OPERATION_H
#define AXIS2_WSDL_BINDING_OPERATION_H

/**
 * @file axis2_wsdl_binding_operation.h
 * @brief axis2 wsdl binding_operation interface. 
 */
#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>
#include <axis2_linked_list.h>
#include <axis2_qname.h>
#include <axis2_wsdl_extensible_component.h>
#include <axis2_wsdl_binding_msg_ref.h>
#include <axis2_wsdl_operation.h>
#include <axis2_wsdl_binding_fault.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_binding_msg_ref;
struct axis2_wsdl_binding_fault;
struct axis2_wsdl_extensible_component;	
typedef struct axis2_wsdl_binding_operation_ops axis2_wsdl_binding_operation_ops_t;
typedef struct axis2_wsdl_binding_operation axis2_wsdl_binding_operation_t;
	
/** @defgroup axis2_wsdl_binding_operation Wsdl Binding Operation
  * @ingroup axis2_wsdl
  * @{
  */

/** 
 * @brief Wsdl Binding Operation operations struct
 * Encapsulator struct for operations of axis2_wsdl_binding_operation
 */
struct axis2_wsdl_binding_operation_ops
{
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_wsdl_binding_operation_t *wsdl_binding_operation, 
                                        axis2_env_t **env);
    
    /**
     * Method getInput
     *
     * @return message reference
     */
    struct axis2_wsdl_binding_msg_ref * (AXIS2_CALL *
    get_input) (axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env);
    
    
    /**
     * Method setInput
     *
     * @param input
     */
    axis2_status_t (AXIS2_CALL *
    set_input) (axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            struct axis2_wsdl_binding_msg_ref *input);
    
    /**
     * Method getOperation
     *
     * @return operation
     */
    void *(AXIS2_CALL *
    get_operation) (axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env);
    
    /**
     * Method setOperation
     *
     * @param operation
     */
    axis2_status_t (AXIS2_CALL *
    set_operation)(axis2_wsdl_binding_operation_t *binding_operation,
                                                axis2_env_t **env,
                                                void *operation);
    
    /**
     * Method getOutput
     *
     * @return message reference
     */
    struct axis2_wsdl_binding_msg_ref * (AXIS2_CALL *
    get_output)(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env);
    
    /**
     * Method setOutput
     *
     * @param output
     */
    axis2_status_t (AXIS2_CALL *
    set_output) (axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            struct axis2_wsdl_binding_msg_ref *output);
    
    /**
     * Method getName
     *
     * @return QName
     */
    axis2_qname_t *(AXIS2_CALL *
    get_qname) (axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env);
    
    /**
     * Method setName
     *
     * @param name
     */
    axis2_status_t (AXIS2_CALL *
    set_qname) (axis2_wsdl_binding_operation_t *binding_operation,
                                                axis2_env_t **env,
                                                axis2_qname_t *qname);
    
    /**
     * Add the InFault to the Components InFaults
     *
     * @param inFault
     */
    axis2_status_t (AXIS2_CALL *
    add_infault) (axis2_wsdl_binding_operation_t *binding_operation,
                                                axis2_env_t **env,
                                                struct axis2_wsdl_binding_fault *infault);
    
    /**
     * Add the OutFault to the Component OutFaults
     *
     * @param outFault
     */
    axis2_status_t (AXIS2_CALL *
    add_outfault) (axis2_wsdl_binding_operation_t *binding_operation,
                                                axis2_env_t **env,
                                                struct axis2_wsdl_binding_fault *outfault);
    
    axis2_linked_list_t * (AXIS2_CALL *
    get_infaults) (axis2_wsdl_binding_operation_t *binding_operation,
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_infaults) (axis2_wsdl_binding_operation_t *binding_operation,
                                                axis2_env_t **env,
                                                axis2_linked_list_t *infaults);
    
    axis2_linked_list_t * (AXIS2_CALL *
    get_outfaults) (axis2_wsdl_binding_operation_t *binding_operation,
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_outfaults) (axis2_wsdl_binding_operation_t *binding_operation,
                                                axis2_env_t **env,
                                                axis2_linked_list_t *outfaults);
};

/**
 * @brief Wsdl Binding Operation struct
 * Axis2 Wsdl Binding Operation
 * Binding Operation is used to hold wsdl_binding_operation properties
 */
struct axis2_wsdl_binding_operation
{
	axis2_wsdl_binding_operation_ops_t *ops;
    struct axis2_wsdl_extensible_component *extensible_component;
};

/** create Wsdl Binding Operation struct
 * @return pointer to newly created wsdl wsdl_binding_operation
 */
AXIS2_DECLARE(axis2_wsdl_binding_operation_t *) 
axis2_wsdl_binding_operation_create (axis2_env_t **env);

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_BINDING_OPERATION_FREE(wsdl_binding_operation, env) \
		((wsdl_binding_operation->ops)->free (wsdl_binding_operation, env))

#define AXIS2_WSDL_BINDING_OPERATION_SET_INPUT(wsdl_binding_operation, env, \
		inut) \
		((wsdl_binding_operation->ops)->set_input(wsdl_binding_operation , env, \
            input)
	
#define AXIS2_WSDL_BINDING_OPERATION_GET_OPERATION(wsdl_binding_operation, env) \
		((wsdl_binding_operation->ops)->get_operation(wsdl_binding_operation, env))

#define AXIS2_WSDL_BINDING_OPERATION_SET_OPERATION(wsdl_binding_operation, env, \
		key, operation) \
		((wsdl_binding_operation->ops)->set_operation(wsdl_binding_operation, env,\
            key, operation))
		
#define AXIS2_WSDL_BINDING_OPERATION_GET_OUTPUT(wsdl_binding_operation, env) \
		((wsdl_binding_operation->ops)->get_output(wsdl_binding_operation, env))
        
#define AXIS2_WSDL_BINDING_OPERATION_SET_OUTPUT(wsdl_binding_operation, env, output) \
		((wsdl_binding_operation->ops)->set_output(wsdl_binding_operation, env, output))

#define AXIS2_WSDL_BINDING_OPERATION_GET_QNAME(wsdl_binding_operation, env) \
		((wsdl_binding_operation->ops)->get_qname(wsdl_binding_operation, env))
        
#define AXIS2_WSDL_BINDING_OPERATION_SET_QNAME(wsdl_binding_operation, env, qname) \
		((wsdl_binding_operation->ops)->set_qname(wsdl_binding_operation, env, qname))

#define AXIS2_WSDL_BINDING_OPERATION_ADD_INFAULT(wsdl_binding_operation, env, infault) \
		((wsdl_binding_operation->ops)->add_infault(wsdl_binding_operation, env, infault))
        
#define AXIS2_WSDL_BINDING_OPERATION_ADD_OUTFAULT(wsdl_binding_operation, env, outfault) \
		((wsdl_binding_operation->ops)->add_outfault(wsdl_binding_operation, env, outfault))

#define AXIS2_WSDL_BINDING_OPERATION_GET_INFAULTS(wsdl_binding_operation, env) \
		((wsdl_binding_operation->ops)->get_infaults(wsdl_binding_operation, env))
        
#define AXIS2_WSDL_BINDING_OPERATION_SET_INFAULTS(wsdl_binding_operation, env, infaults) \
		((wsdl_binding_operation->ops)->set_infaults(wsdl_binding_operation, env, infaults))        

#define AXIS2_WSDL_BINDING_OPERATION_GET_OUTFAULTS(wsdl_binding_operation, env) \
		((wsdl_binding_operation->ops)->get_outfaults(wsdl_binding_operation, env))
        
#define AXIS2_WSDL_BINDING_OPERATION_SET_OUTFAULTS(wsdl_binding_operation, env, outfaults) \
		((wsdl_binding_operation->ops)->set_outfaults(wsdl_binding_operation, env, outfaults))
     
/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif /*AXIS2_WSDL_BINDING_OPERATION_H*/
