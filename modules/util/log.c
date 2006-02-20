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
#include <string.h>
#include <axis2_platform_auto_sense.h>
#include <axis2_log_default.h>
#include <axis2_file_handler.h>
#include <axis2_thread.h>


typedef struct axis2_log_impl axis2_log_impl_t;

struct axis2_log_impl
{
	axis2_log_t 			log;
	void 					*stream;
	axis2_thread_mutex_t 	*mutex;
};

#define AXIS2_INTF_TO_IMPL(log) ((axis2_log_impl_t*)(log))

axis2_status_t AXIS2_CALL
axis2_log_impl_free (axis2_allocator_t *allocator, axis2_log_t *log)
{
	axis2_log_impl_t *log_impl = NULL;
	
    if (NULL != log )
    {
        log_impl = AXIS2_INTF_TO_IMPL(log);

		if(NULL != log_impl->mutex)
        {
            axis2_thread_mutex_destroy(log_impl->mutex);
		    log_impl->mutex = NULL;   
		}    
		if (NULL != AXIS2_INTF_TO_IMPL(log)->stream)
		{
			axis2_file_handler_close(log_impl->stream);
			log_impl->stream = NULL;
        }
        if ( NULL != log->ops)
        {
        	AXIS2_FREE (allocator, log->ops);
        	log->ops = NULL;
        }
        AXIS2_FREE (allocator, log_impl); 
    }
    return AXIS2_SUCCESS;
}

/* function prototypes */
axis2_status_t AXIS2_CALL axis2_log_impl_write (axis2_log_t *log,
	   	const axis2_char_t *buffer, axis2_log_levels_t level,
	   	const axis2_char_t *file, const int line);

AXIS2_DECLARE(axis2_status_t) axis2_log_impl_write_to_file(FILE *fd,
	   	axis2_thread_mutex_t *mutex, axis2_log_levels_t level, 
		const axis2_char_t * file, const int line, const axis2_char_t * value);


AXIS2_DECLARE(axis2_log_t *)
axis2_log_create (axis2_allocator_t * allocator, axis2_log_ops_t * ops,
		axis2_char_t * stream_name)
{
    axis2_log_impl_t *log_impl;
	axis2_char_t *path_home;
	axis2_char_t log_file_name[500];
	axis2_char_t log_dir[500];
	axis2_char_t tmp_filename[100];
	
    if (!allocator)
        return NULL;

    log_impl = (axis2_log_impl_t *) AXIS2_MALLOC (allocator, 
	sizeof (axis2_log_impl_t));

    if (!log_impl)
        return NULL;
	
	log_impl->mutex = axis2_thread_mutex_create(allocator, 
				AXIS2_THREAD_MUTEX_DEFAULT);
	
	if (!log_impl->mutex)
	{
		fprintf(stderr,"cannot create log mutex \n");
		return NULL;
	}
	
	/* default log file is axis2.log */
	if (stream_name)
		AXIS2_SNPRINTF(tmp_filename,100,"%s",stream_name);
	else
		AXIS2_SNPRINTF(tmp_filename,100,"%s","axis2.log");
	
	/* we write all logs to AXIS2C_HOME/logs if it is set otherwise
	 * to the working dir
	 */
	if (NULL != (path_home = AXIS2_GETENV("AXIS2C_HOME")))
    {
		AXIS2_SNPRINTF(log_dir, 500, "%s%c%s", path_home, AXIS2_PATH_SEP_CHAR, "logs");
		if (AXIS2_SUCCESS == axis2_file_handler_access(log_dir,AXIS2_F_OK))
		{
			AXIS2_SNPRINTF(log_file_name, 500, "%s%c%s", log_dir, AXIS2_PATH_SEP_CHAR,
                tmp_filename);
		}
		else
		{
			fprintf(stderr, "log folder %s does not exist - log file %s is written to . dir\n",
				log_dir, tmp_filename);
			AXIS2_SNPRINTF(log_file_name, 500, "%s", tmp_filename);
		}
    }
    else
    {
		fprintf(stderr, "AXIS2C_HOME is not set - log is written to . dir\n");
    	AXIS2_SNPRINTF(log_file_name, 500, "%s", tmp_filename);
    }
	
	axis2_thread_mutex_lock(log_impl->mutex);
	
	log_impl->stream = axis2_file_handler_open(log_file_name,"a+");
	
	axis2_thread_mutex_unlock(log_impl->mutex);
	
	/* by default, log is enabled */
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
    }

    return &(log_impl->log);
}

/*TODO:remove this method*/
axis2_status_t AXIS2_CALL
axis2_log_impl_write (axis2_log_t *log, const axis2_char_t *buffer, 
	axis2_log_levels_t level, const axis2_char_t *file, const int line)
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
            case AXIS2_LOG_LEVEL_TRACE:
                level_str = "[...TRACE...] ";
                break;
        }
	  fprintf(stderr,"%s %s(%d) %s\n", level_str, file, line, buffer);
    }
    
    return 0;
}


AXIS2_DECLARE(axis2_status_t)
axis2_log_impl_write_to_file(FILE *fd, axis2_thread_mutex_t *mutex, 
		axis2_log_levels_t level, const axis2_char_t *file, 
		const int line, const axis2_char_t *value)
{
    char *level_str = "";
	if (!fd)
		return -1;
	/**
      * print all critical and error logs irrespective of log->level setting
	  */
    
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
        case AXIS2_LOG_LEVEL_TRACE:
            level_str = "[...TRACE...] ";
            break;
    }
	axis2_thread_mutex_lock(mutex);
    if (file)
    	fprintf(fd,"[%s] %s%s(%d) %s\n", axis2_log_impl_get_time_str(), 
			level_str, file, line, value);
    else
    	fprintf(fd,"[%s] %s %s\n", axis2_log_impl_get_time_str(), 
			level_str, value);
	fflush(fd);
	axis2_thread_mutex_unlock(mutex);
	
	return 0;
}

AXIS2_DECLARE(axis2_status_t)
axis2_log_impl_log_debug(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	axis2_thread_mutex_t *mutex = NULL;
	
	if (!log || !format)
		return -1;

	if (!log->enabled)
		return -1;
	
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
	{
		fprintf(stderr,"Stream is not found\n");
		return -1;
	}
	
	if (NULL == (mutex = AXIS2_INTF_TO_IMPL(log)->mutex))
	{
		fprintf(stderr,"Log mutex is not found\n");
		return -1;
	}
	
	if (AXIS2_LOG_LEVEL_DEBUG <= log->level)
	{
		char value[AXIS2_LEN_VALUE+1];
    	va_list ap;
    	va_start(ap, format);
    	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
    	va_end(ap);
		axis2_log_impl_write_to_file(fd, mutex, AXIS2_LOG_LEVEL_DEBUG, filename, 
						linenumber, value);
	}
	return 0;
}

AXIS2_DECLARE(axis2_status_t)
axis2_log_impl_log_info(axis2_log_t *log, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	axis2_thread_mutex_t *mutex = NULL;
	
	if (!log || !format)
		return -1;
	
	if (!log->enabled)
		return -1;
	
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
	{
		fprintf(stderr,"Stream is not found\n");
		return -1;
	}
	
	if (NULL == (mutex = AXIS2_INTF_TO_IMPL(log)->mutex))
	{
		fprintf(stderr,"Log mutex is not found\n");
		return -1;
	}
	
	if (AXIS2_LOG_LEVEL_INFO <= log->level)
	{
		char value[AXIS2_LEN_VALUE+1];
    	va_list ap;
    	va_start(ap, format);
    	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
    	va_end(ap);
		axis2_log_impl_write_to_file(fd, mutex, AXIS2_LOG_LEVEL_INFO, NULL, -1,
			value);
	}
	return 0;
}

AXIS2_DECLARE(axis2_status_t) 
axis2_log_impl_log_warning(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	axis2_thread_mutex_t *mutex = NULL;
	
	if (!log || !format)
		return -1;
	
	if (!log->enabled)
		return -1;
	
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
	{
		fprintf(stderr,"Stream is not found\n");
		return -1;
	}
	
	if (NULL == (mutex = AXIS2_INTF_TO_IMPL(log)->mutex))
	{
		fprintf(stderr,"Log mutex is not found\n");
		return -1;
	}
	
	if (AXIS2_LOG_LEVEL_WARNING <= log->level)
	{
		char value[AXIS2_LEN_VALUE+1];
    	va_list ap;
    	va_start(ap, format);
    	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
    	va_end(ap);
		axis2_log_impl_write_to_file(fd, mutex, AXIS2_LOG_LEVEL_WARNING, 
				filename, linenumber, value);
	}
	return 0;
}


AXIS2_DECLARE(axis2_status_t)
axis2_log_impl_log_error(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	axis2_thread_mutex_t *mutex = NULL;
	
	char value[AXIS2_LEN_VALUE+1];
   	va_list ap;
   	
	if (!log || !format)
		return -1;
	
	if (!log->enabled)
		return -1;
	
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
	{
		fprintf(stderr,"Stream is not found\n");
		return -1;
	}
	
	if (NULL == (mutex = AXIS2_INTF_TO_IMPL(log)->mutex))
	{
		fprintf(stderr,"Log mutex is not found\n");
		return -1;
	}
	
	
   	va_start(ap, format);
  	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
   	va_end(ap);
	axis2_log_impl_write_to_file(fd, mutex, AXIS2_LOG_LEVEL_ERROR, filename, 
						linenumber, value);
	return 0;
}
	
AXIS2_DECLARE(axis2_status_t) 
axis2_log_impl_log_critical(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	axis2_thread_mutex_t *mutex = NULL;
	
	char value[AXIS2_LEN_VALUE+1];
   	va_list ap;
	if (!log || !format)
		return -1;
	
	if (!log->enabled)
		return -1;
	
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
	{
		fprintf(stderr,"Stream is not found\n");
		return -1;
	}
	
	if (NULL == (mutex = AXIS2_INTF_TO_IMPL(log)->mutex))
	{
		fprintf(stderr,"Log mutex is not found\n");
		return -1;
	}
	
	
   	va_start(ap, format);
   	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
   	va_end(ap);
	axis2_log_impl_write_to_file(fd, mutex, AXIS2_LOG_LEVEL_CRITICAL, filename, 
						linenumber, value);
	return 0;
}

AXIS2_DECLARE(axis2_char_t *)
axis2_log_impl_get_time_str(void)
{
    time_t tp;
	char *time_str;
    tp = time(&tp);
    time_str = ctime(&tp);
    if(NULL == time_str)
    {
        return NULL;;
    }
    if('\n' == time_str[strlen(time_str)-1])
    {
       time_str[strlen(time_str)-1] = '\0';
    }
    return time_str;
}

#ifdef AXIS2_TRACE
AXIS2_DECLARE(axis2_status_t)
axis2_log_impl_log_trace(axis2_log_t *log, const axis2_char_t *filename,
	   	const int linenumber, const axis2_char_t *format,...)
{
	FILE *fd = NULL;
	axis2_thread_mutex_t *mutex = NULL;
	
	if (!log || !format)
		return -1;

	if (!log->enabled)
		return -1;
	
	if (NULL == (fd = AXIS2_INTF_TO_IMPL(log)->stream))
	{
		fprintf(stderr,"Stream is not found\n");
		return -1;
	}
	
	if (NULL == (mutex = AXIS2_INTF_TO_IMPL(log)->mutex))
	{
		fprintf(stderr,"Log mutex is not found\n");
		return -1;
	}
	
	if (AXIS2_LOG_LEVEL_TRACE <= log->level)
	{
		char value[AXIS2_LEN_VALUE+1];
    	va_list ap;
    	va_start(ap, format);
    	AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
    	va_end(ap);
		axis2_log_impl_write_to_file(fd, mutex, AXIS2_LOG_LEVEL_TRACE, filename, 
						linenumber, value);
	}
	return 0;
}
#else
AXIS2_DECLARE(axis2_status_t) axis2_log_impl_log_trace(axis2_log_t *log, 
	const axis2_char_t *filename, const int linenumber,
	const axis2_char_t *format,...) {}
#endif
