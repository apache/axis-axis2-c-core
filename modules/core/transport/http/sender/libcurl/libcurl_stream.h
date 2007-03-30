#ifndef LIBCURL_STREAM_H
#define LIBCURL_STREAM_H

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axis2_stream.h>

#ifdef __cplusplus
extern "C"
{
#endif


/** brief Constructor for creating apche2 stream
  * @return axis2_stream (libcurl)
  */
	AXIS2_EXTERN axis2_stream_t * AXIS2_CALL axis2_stream_create_libcurl
	(const axutil_env_t *env, axis2_char_t *buffer, unsigned int size);

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* LIBCURL_STREAM_H */
