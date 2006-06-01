#ifndef _AXIS2_GETOPT_WINDOWS_H_
#define _AXIS2_GETOPT_WINDOWS_H_

#include <axis2_utils_defines.h>
/**
 * @file axis2_getopt_windows.h
 * @brief windows cutdown version of getopt function in unix
 */

#ifdef	__cplusplus
extern "C" {
#endif

/** @defgroup axis2_getopt_windows getopt
 * @ingroup axis2_windows
 * @{
 */

AXIS2_DECLARE_DATA  int	opterr;
AXIS2_DECLARE_DATA  int 	optopt;	
AXIS2_DECLARE_DATA  char	*optarg;

/**
  * return and log error
  * @param __optopt option
  * @param __err error code
  * @param __showerr whether or not send to stderr
  * @return ':' or '?'
  */
int _axis2_opt_error(int __optopt, int __err, int __showerr);

/**
  * cutdown version of getopt in unix
  * @param __argc no of arguments
  * @param __argv list of arguments
  * @param __shortopts options
  * @return option char if successful, -1 if over, ':' or '?' if error
  */
AXIS2_EXTERN int AXIS2_CALL 
axis2_getopt(int __argc, char * const *__argv, const char *__shortopts);

/** @} */

#ifdef	__cplusplus
}
#endif

#endif