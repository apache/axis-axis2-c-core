/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <axis2_platform_auto_sense.h>
#include <axis2_log_default.h>
#include <axis2_file_handler.h>

typedef struct axis2_log_impl axis2_log_impl_t;

struct axis2_log_impl
{
	axis2_log_t 	log;
	void 			*stream;
};

#define AXIS2_INTF_TO_IMPL(log) ((axis2_log_impl_t*)(log))

axis2_status_t AXIS2_CALL
axis2_log_impl_free (axis2_allocator_t *allocator, axis2_log_t *log)
{
	
    if (NULL != log )
    {
		if ( NULL != log->ops)
        	AXIS2_FREE (allocator, log->ops);
		if (NULL != AXIS2_INTF_TO_IMPL(log)->stream)
			axis2_file_handler_close(AXIS2_INTF_TO_IMPL(log)->stream);

        AXIS2_FREE (allocator, AXIS2_INTF_TO_IMPL(log)); 
    }
    return AXIS2_SUCCESS;
}

/* function prototypes */
axis2_status_t AXIS2_CALL axis2_log_impl_write (axis2_log_t *log,
	   	const axis2_char_t *buffer, axis2_log_levels_t level,
	   	const axis2_char_t *file, const int line);

axis2_status_t AXIS2_CALL axis2_log_impl_write_to_file(FILE *fd,
	   	axis2_log_levels_t level, const axis2_char_t * file, const int line,
	   	const axis2_char_t * value);


AXIS2_DECLARE(axis2_log_t *)
axis2_log_create (axis2_allocator_t * allocator, axis2_log_ops_t * ops,
		axis2_char_t * stream_name)
{
    axis2_log_impl_t *log_impl;
	axis2_char_t *log_file_name = NULL;
    if (!allocator)
        return NULL;

    log_impl = (axis2_log_impl_t *) AXIS2_MALLOC (allocator, sizeof (axis2_log_impl_t));

    if (!log_impl)
        return NULL;

	if (stream_name)
	{
		log_file_name = stream_name;
	}
	else
	{
		axis2_char_t *path = AXIS2_GETENV("AXIS2C_HOME");
		axis2_char_t *filename = "/log/axis2.log";
		int len = axis2_strlen(path) + axis2_strlen(filename)+1;
		log_file_name = (axis2_char_t*)AXIS2_MALLOC(allocator,
                    len*sizeof(axis2_char_t));
    	memcpy(log_file_name, path, axis2_strlen(path)*sizeof(axis2_char_t));
    	memcpy((log_file_name + axis2_strlen(path)*sizeof(axis2_char_t)), filename,
                    axis2_strlen(filename)*sizeof(axis2_char_t));
    	log_file_name[len*sizeof(axis2_char_t) - sizeof(axis2_char_t)] = '\0';
		printf("default file name = %s\n",log_file_name);
	}
	log_impl->stream = axis2_file_handler_open(log_file_name,"w");
	
	log_impl->log.enabled = 1;

    if (ops)
        log_impl->log.ops = ops;
    else
    {
        log_impl->log.ops =
            (axis2_log_ops_t *) AXIS2_MALLOC (allocator,
                                              sizeof (axis2_log_ops_t));

        if (!log_impl->log.ops)
        {
			axis2_log_impl_free(allocator,(axis2_log_t*)log_impl);
            return NULL;
        }
        log_impl->log.ops->free = axis2_log_impl_free;
        log_impl->log.ops->write = axis2_log_impl_write;
		log_impl->log.ops->log_critical = axis2_log_impl_log_critical;
		log_impl->log.ops->log_error = axis2_log_impl_log_error;
		log_impl->log.ops->log_warning = axis2_log_impl_log_warning;
		log_impl->log.ops->log_info = axis2_log_impl_log_info;
		log_impl->log.ops->log_debug = axis2_log_impl_log_debug;

    }

    return &(log_impl->log);
}

/*TODO:remove this method*/
axis2_status_t AXIS2_CALL
axis2_log_impl_write (axis2_log_t *log, const axis2_char_t *buffer, axis2_log_levels_t level,
		const axis2_char_t *file, const int line)
{
    if (!log || !buffer)
        return -1;
    if (level <= log->level)
    {
        char *level_str = "";
        switch (level)
        {
            case AXIS2_LOG_LEVEL_CRITICAL:
                level_str = "[critical] ";
                break;
            case AXIS2_LOG_LEVEL_ERROR:
                level_str = "[error] ";
                break;
            case AXIS2_LOG_LEVEL_WARNING:
                level_str = "[warning] ";
                break;
            case AXIS2_LOG_LEVEL_INFO:
                level_str = "[info] ";
                break;
            case AXIS2_LOG_LEVEL_DEBUG:
                level_str = "[debug] ";
                break;
        }
	  fprintf(stderr,"%s %s(%d) %s\n", level_str, file, line, buffer);
    }
    
    return 0;
}


axis2_status_t AXIS2_CALL axis2_log_impl_write_to_file(FILE *fd, axis2_log_levels_t level,
	   	const axis2_char_t *file, const int line, const axis2_char_t *value)
{
	if (!fd)
		return -1;
	/**
      * print all critical and error logs irrespective of log->level setting
	  */
    char *level_str = "";
    switch (level)
    {
    	case AXIS2_LOG_LEVEL_CRITICAL:
        	level_str = "[critical] ";
            break;
        case AXIS2_LOG_LEVEL_ERROR:
            level_str = "[error] ";
            break;
        case AXIS2_LOG_LEVEL_WARNING:
            level_str = "[warning] ";
            break;
        case AXIS2_LOG_LEVEL_INFO:
            level_str = "[info] ";
            break;
        case AXIS2_LOG_LEVEL_DEBUG:
            level_str = "[debug] ";
            break;
    }
    if (file)
    	fprintf(fd,"%s %s(%d) %s\n", level_str, file, line, value);
    else
    	fprintf(fd,"%s %s\n", level_str, value);
	return 0;
}

axis2_status_t AXIS2_CALL 
axis2_log_impl_log_debug(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	
	if (!log || !format)
		return -1;

	if (!log->enabled)
		return -1;
	
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
		return -1;
	
	if (AXIS2_LOG_LEVEL_DEBUG <= log->level)
	{
		char value[AXIS2_LEN_VALUE+1];
    	va_list ap;
    	va_start(ap, format);
    	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
    	va_end(ap);
		axis2_log_impl_write_to_file(fd, AXIS2_LOG_LEVEL_DEBUG, filename, linenumber, value);
	}
	return 0;
}

axis2_status_t AXIS2_CALL 
axis2_log_impl_log_info(axis2_log_t *log, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	
	if (!log || !format)
		return -1;
	
	if (!log->enabled)
		return -1;
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
		return -1;
	
	if (AXIS2_LOG_LEVEL_INFO <= log->level)
	{
		char value[AXIS2_LEN_VALUE+1];
    	va_list ap;
    	va_start(ap, format);
    	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
    	va_end(ap);
		axis2_log_impl_write_to_file(fd, AXIS2_LOG_LEVEL_INFO, NULL, -1, value);
	}
	return 0;
}

axis2_status_t AXIS2_CALL 
axis2_log_impl_log_warning(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	
	if (!log || !format)
		return -1;
	
	if (!log->enabled)
		return -1;
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
		return -1;
	
	if (AXIS2_LOG_LEVEL_WARNING <= log->level)
	{
		char value[AXIS2_LEN_VALUE+1];
    	va_list ap;
    	va_start(ap, format);
    	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
    	va_end(ap);
		axis2_log_impl_write_to_file(fd, AXIS2_LOG_LEVEL_WARNING, filename, linenumber, value);
	}
	return 0;
}


axis2_status_t AXIS2_CALL 
axis2_log_impl_log_error(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	
	if (!log || !format)
		return -1;
	
	if (!log->enabled)
		return -1;
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
		return -1;
	
	char value[AXIS2_LEN_VALUE+1];
   	va_list ap;
   	va_start(ap, format);
  	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
   	va_end(ap);
	axis2_log_impl_write_to_file(fd, AXIS2_LOG_LEVEL_ERROR, filename, linenumber, value);
	return 0;
}
	
axis2_status_t AXIS2_CALL 
axis2_log_impl_log_critical(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	
	if (!log || !format)
		return -1;
	
	if (!log->enabled)
		return -1;
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
		return -1;
	
	char value[AXIS2_LEN_VALUE+1];
   	va_list ap;
   	va_start(ap, format);
   	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
   	va_end(ap);
	axis2_log_impl_write_to_file(fd, AXIS2_LOG_LEVEL_CRITICAL, filename, linenumber, value);
	return 0;
}

