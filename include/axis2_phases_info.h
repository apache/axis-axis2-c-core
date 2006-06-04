#ifndef AXIS2_PHASES_INFO_H
#define AXIS2_PHASES_INFO_H

/**
 * @file axis2_phases_info.h
 * @brief axis2 phases info interface. 
 */
#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_op.h>
#include <axis2_phase.h>

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct axis2_phases_info_ops axis2_phases_info_ops_t;
typedef struct axis2_phases_info axis2_phases_info_t;
   
/** @defgroup axis2_phases_info Phases Info
  * @ingroup axis2_deployment
  * @{
  */

/** 
 * @brief Phases Info ops struct
 * Encapsulator struct for ops of axis2_phases_info
 */
struct axis2_phases_info_ops
{
   /** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free) (axis2_phases_info_t *phases_info, 
                                        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_in_phases) (axis2_phases_info_t *phases_info,
                                    const axis2_env_t *env,
                                    axis2_array_list_t *in_phases);
    
    axis2_status_t (AXIS2_CALL *
    set_out_phases) (axis2_phases_info_t *phases_info,
                                    const axis2_env_t *env,
                                    axis2_array_list_t *out_phases);
    
    axis2_status_t (AXIS2_CALL *
    set_in_faultphases) (axis2_phases_info_t *phases_info,
                                    const axis2_env_t *env,
                                    axis2_array_list_t *in_faultphases);
                                    
    axis2_status_t (AXIS2_CALL *
    set_out_faultphases) (axis2_phases_info_t *phases_info,
                                            const axis2_env_t *env,
                                            axis2_array_list_t * out_faultphases);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_in_phases) (axis2_phases_info_t *phases_info,
                                    const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_out_phases) (axis2_phases_info_t *phases_info,
                                    const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_in_faultphases) (axis2_phases_info_t *phases_info,
                                            const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_out_faultphases) (axis2_phases_info_t *phases_info,
                                            const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_op_in_phases) (axis2_phases_info_t *phases_info,
                                            const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_op_out_phases) (axis2_phases_info_t *phases_info,
                                                const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_op_in_faultphases) (axis2_phases_info_t *phases_info,
                                                    const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_op_out_faultphases) (axis2_phases_info_t *phases_info,
                                                    const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_op_phases) (axis2_phases_info_t *phases_info,
                                            const axis2_env_t *env,
                                            struct axis2_op *axis2_opt);
    
                                                
};

/**
 * @brief Phases Info struct
 * Axis2 Phases Info
 */
struct axis2_phases_info
{
   axis2_phases_info_ops_t *ops;
};

/** create Phases Info struct
 * @return pointer to newly created phases info
 */
AXIS2_EXTERN axis2_phases_info_t * AXIS2_CALL 
axis2_phases_info_create (const axis2_env_t *env);

/**************************** Start of function macros ************************/

#define AXIS2_PHASES_INFO_FREE(phases_info, env) \
      ((phases_info)->ops->free (phases_info, env))

#define AXIS2_PHASES_INFO_SET_IN_PHASES(phases_info, env, in_phases) \
      ((phases_info)->ops->set_in_phases(phases_info, env, in_phases))
      
#define AXIS2_PHASES_INFO_SET_OUT_PHASES(phases_info, env, out_phases) \
      ((phases_info)->ops->set_out_phases(phases_info, env, out_phases))

#define AXIS2_PHASES_INFO_SET_IN_FAULTPHASES(phases_info, env, in_faultphases) \
      ((phases_info)->ops->set_in_faultphases(phases_info, env, in_faultphases))

#define AXIS2_PHASES_INFO_SET_OUT_FAULTPHASES(phases_info, env, out_faultphases) \
      ((phases_info)->ops->set_out_faultphases(phases_info, env, out_faultphases)) 

#define AXIS2_PHASES_INFO_GET_IN_PHASES(phases_info, env) \
      ((phases_info)->ops->get_in_phases(phases_info, env)) 

#define AXIS2_PHASES_INFO_GET_OUT_PHASES(phases_info, env) \
      ((phases_info)->ops->get_out_phases(phases_info, env)) 
        
#define AXIS2_PHASES_INFO_GET_IN_FAULTPHASES(phases_info, env) \
      ((phases_info)->ops->get_in_faultphases(phases_info, env)) 

#define AXIS2_PHASES_INFO_GET_OUT_FAULTPHASES(phases_info, env) \
      ((phases_info)->ops->get_out_faultphases(phases_info, env)) 
        
#define AXIS2_PHASES_INFO_GET_OP_IN_PHASES(phases_info, env) \
      ((phases_info)->ops->get_op_in_phases(phases_info, env)) 

#define AXIS2_PHASES_INFO_GET_OP_OUT_PHASES(phases_info, env) \
      ((phases_info)->ops->get_op_out_phases(phases_info, env)) 
                                        
#define AXIS2_PHASES_INFO_GET_OP_IN_FAULTPHASES(phases_info, env) \
      ((phases_info)->ops->get_op_in_faultphases(phases_info, env))                                         
                                        
#define AXIS2_PHASES_INFO_GET_OP_OUT_FAULTPHASES(phases_info, env) \
      ((phases_info)->ops->get_op_out_faultphases(phases_info, env))                                         
                                        
#define AXIS2_PHASES_INFO_SET_OP_PHASES(phases_info, env, axis2_opt) \
      ((phases_info)->ops->set_op_phases(phases_info, env, axis2_opt))                                         
                                           
/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif /*AXIS2_PHASES_INFO_H*/
