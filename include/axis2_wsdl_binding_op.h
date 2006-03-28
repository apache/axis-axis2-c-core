#ifndef AXIS2_WSDL_BINDING_OP_H
#define AXIS2_WSDL_BINDING_OP_H

/**
 * @file axis2_wsdl_binding_op.h
 * @brief axis2 wsdl binding_op interface. 
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
#include <axis2_wsdl_op.h>
#include <axis2_wsdl_binding_fault.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_binding_msg_ref;
struct axis2_wsdl_binding_fault;
struct axis2_wsdl_extensible_component;	
typedef struct axis2_wsdl_binding_op_ops axis2_wsdl_binding_op_ops_t;
typedef struct axis2_wsdl_binding_op axis2_wsdl_binding_op_t;
	
/** @defgroup axis2_wsdl_binding_op Wsdl Binding Operation
  * @ingroup axis2_wsdl
  * @{
  */

/** 
 * @brief Wsdl Binding Operation ops struct
 * Encapsulator struct for ops of axis2_wsdl_binding_op
 */
struct axis2_wsdl_binding_op_ops
{
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_wsdl_binding_op_t *wsdl_binding_op, 
                                        axis2_env_t **env);
    
    /**
     * Method getInput
     *
     * @return message reference
     */
    struct axis2_wsdl_binding_msg_ref * (AXIS2_CALL *
    get_input) (axis2_wsdl_binding_op_t *binding_op,
                                            axis2_env_t **env);
    
    
    /**
     * Method setInput
     *
     * @param input
     */
    axis2_status_t (AXIS2_CALL *
    set_input) (axis2_wsdl_binding_op_t *binding_op,
                axis2_env_t **env,
                struct axis2_wsdl_binding_msg_ref *input);
    
    /**
     * Method getOperation
     *
     * @return op
     */
    void *(AXIS2_CALL *
    get_op) (axis2_wsdl_binding_op_t *binding_op,
                                        axis2_env_t **env);
    
    /**
     * Method setOperation
     *
     * @param op
     */
    axis2_status_t (AXIS2_CALL *
    set_op)(axis2_wsdl_binding_op_t *binding_op,
                axis2_env_t **env,
                void *op);
    
    /**
     * Method getOutput
     *
     * @return message reference
     */
    struct axis2_wsdl_binding_msg_ref * (AXIS2_CALL *
    get_output)(axis2_wsdl_binding_op_t *binding_op,
                                            axis2_env_t **env);
    
    /**
     * Method setOutput
     *
     * @param output
     */
    axis2_status_t (AXIS2_CALL *
    set_output) (axis2_wsdl_binding_op_t *binding_op,
                                            axis2_env_t **env,
                                            struct axis2_wsdl_binding_msg_ref *output);
    
    /**
     * Method getName
     *
     * @return QName
     */
    axis2_qname_t *(AXIS2_CALL *
    get_qname) (axis2_wsdl_binding_op_t *binding_op,
                                        axis2_env_t **env);
    
    /**
     * Method setName
     *
     * @param name
     */
    axis2_status_t (AXIS2_CALL *
    set_qname) (axis2_wsdl_binding_op_t *binding_op,
                                                axis2_env_t **env,
                                                axis2_qname_t *qname);
    
    /**
     * Add the InFault to the Components InFaults
     *
     * @param inFault
     */
    axis2_status_t (AXIS2_CALL *
    add_in_fault) (axis2_wsdl_binding_op_t *binding_op,
                                                axis2_env_t **env,
                                                struct axis2_wsdl_binding_fault *in_fault);
    
    /**
     * Add the OutFault to the Component OutFaults
     *
     * @param outFault
     */
    axis2_status_t (AXIS2_CALL *
    add_out_fault) (axis2_wsdl_binding_op_t *binding_op,
                                                axis2_env_t **env,
                                                struct axis2_wsdl_binding_fault *out_fault);
    
    axis2_linked_list_t * (AXIS2_CALL *
    get_in_faults) (axis2_wsdl_binding_op_t *binding_op,
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_in_faults) (axis2_wsdl_binding_op_t *binding_op,
                                                axis2_env_t **env,
                                                axis2_linked_list_t *in_faults);
    
    axis2_linked_list_t * (AXIS2_CALL *
    get_out_faults) (axis2_wsdl_binding_op_t *binding_op,
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_out_faults) (axis2_wsdl_binding_op_t *binding_op,
                                                axis2_env_t **env,
                                                axis2_linked_list_t *out_faults);
};

/**
 * @brief Wsdl Binding Operation struct
 * Axis2 Wsdl Binding Operation
 * Binding Operation is used to hold wsdl_binding_op properties
 */
struct axis2_wsdl_binding_op
{
	axis2_wsdl_binding_op_ops_t *ops;
    struct axis2_wsdl_extensible_component *extensible_component;
};

/** create Wsdl Binding Operation struct
 * @return pointer to newly created wsdl wsdl_binding_op
 */
AXIS2_DECLARE(axis2_wsdl_binding_op_t *) 
axis2_wsdl_binding_op_create (axis2_env_t **env);

/**************************** Start of function macros ************************/

#define AXIS2_WSDL_BINDING_OP_FREE(wsdl_binding_op, env) \
		((wsdl_binding_op)->ops->free (wsdl_binding_op, env))

#define AXIS2_WSDL_BINDING_OP_SET_INPUT(wsdl_binding_op, env, input) \
		((wsdl_binding_op)->ops->set_input(wsdl_binding_op , env, input))
	
#define AXIS2_WSDL_BINDING_OP_GET_OP(wsdl_binding_op, env) \
		((wsdl_binding_op)->ops->get_op(wsdl_binding_op, env))

#define AXIS2_WSDL_BINDING_OP_SET_OP(wsdl_binding_op, env, \
		op) \
		((wsdl_binding_op)->ops->set_op(wsdl_binding_op, env,\
            op))
		
#define AXIS2_WSDL_BINDING_OP_GET_OUTPUT(wsdl_binding_op, env) \
		((wsdl_binding_op)->ops->get_output(wsdl_binding_op, env))
        
#define AXIS2_WSDL_BINDING_OP_SET_OUTPUT(wsdl_binding_op, env, output) \
		((wsdl_binding_op)->ops->set_output(wsdl_binding_op, env, output))

#define AXIS2_WSDL_BINDING_OP_GET_QNAME(wsdl_binding_op, env) \
		((wsdl_binding_op)->ops->get_qname(wsdl_binding_op, env))
        
#define AXIS2_WSDL_BINDING_OP_SET_QNAME(wsdl_binding_op, env, qname) \
		((wsdl_binding_op)->ops->set_qname(wsdl_binding_op, env, qname))

#define AXIS2_WSDL_BINDING_OP_ADD_IN_FAULT(wsdl_binding_op, env, in_fault) \
		((wsdl_binding_op)->ops->add_in_fault(wsdl_binding_op, env, in_fault))
        
#define AXIS2_WSDL_BINDING_OP_ADD_OUT_FAULT(wsdl_binding_op, env, out_fault) \
		((wsdl_binding_op)->ops->add_out_fault(wsdl_binding_op, env, out_fault))

#define AXIS2_WSDL_BINDING_OP_GET_IN_FAULTS(wsdl_binding_op, env) \
		((wsdl_binding_op)->ops->get_in_faults(wsdl_binding_op, env))
        
#define AXIS2_WSDL_BINDING_OP_SET_IN_FAULTS(wsdl_binding_op, env, in_faults) \
		((wsdl_binding_op)->ops->set_in_faults(wsdl_binding_op, env, in_faults))        

#define AXIS2_WSDL_BINDING_OP_GET_OUT_FAULTS(wsdl_binding_op, env) \
		((wsdl_binding_op)->ops->get_out_faults(wsdl_binding_op, env))
        
#define AXIS2_WSDL_BINDING_OP_SET_OUT_FAULTS(wsdl_binding_op, env, out_faults) \
		((wsdl_binding_op)->ops->set_out_faults(wsdl_binding_op, env, out_faults))
     
/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif /*AXIS2_WSDL_BINDING_OP_H*/
