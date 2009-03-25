/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at

 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platforms/axutil_platform_auto_sense.h>
#include <axutil_log_default.h>
#include <axutil_file_handler.h>
#include <axutil_thread.h>
#include <signal.h>

typedef struct axutil_log_impl axutil_log_impl_t;

static axis2_status_t
axutil_log_impl_rotate(
    axutil_log_t *log);

static void AXIS2_CALL axutil_log_impl_write(
    axutil_log_t *log,
    const axis2_char_t *buffer,
    axutil_log_levels_t level,
    const axis2_char_t *file,
    const int line);

AXIS2_EXTERN void AXIS2_CALL axutil_log_impl_write_to_file(
    axutil_log_t *log,
    axutil_thread_mutex_t *mutex,
    axutil_log_levels_t level,
    const axis2_char_t *file,
    const int line,
    const axis2_char_t *value);

static void AXIS2_CALL axutil_log_impl_free(
    axutil_allocator_t *allocator,
    axutil_log_t *log);

struct axutil_log_impl
{
    axutil_log_t log;
    void *stream;
    axis2_char_t *file_name;
    axutil_thread_mutex_t *mutex;
};

#define AXUTIL_INTF_TO_IMPL(log) ((axutil_log_impl_t*)(log))

static const axutil_log_ops_t axutil_log_ops_var = {
    axutil_log_impl_free,
    axutil_log_impl_write
};

static void AXIS2_CALL
axutil_log_impl_free(
    axutil_allocator_t *allocator,
    axutil_log_t *log)
{
    axutil_log_impl_t *log_impl = NULL;

    if (log)
    {
        log_impl = AXUTIL_INTF_TO_IMPL(log);

        if (log_impl->mutex)
        {
            axutil_thread_mutex_destroy(log_impl->mutex);
        }
        if (log_impl->stream)
        {
            axutil_file_handler_close(log_impl->stream);
        }
        if (log_impl->file_name)
        {
            AXIS2_FREE(allocator, log_impl->file_name);
        }
        AXIS2_FREE(allocator, log_impl);
    }
}

AXIS2_EXTERN axutil_log_t *AXIS2_CALL
axutil_log_create(
    axutil_allocator_t *allocator,
    axutil_log_ops_t *ops,
    const axis2_char_t *stream_name)
{
    axutil_log_impl_t *log_impl;
    axis2_char_t *path_home;
    axis2_char_t log_file_name[AXUTIL_LOG_FILE_NAME_SIZE];
    axis2_char_t log_dir[AXUTIL_LOG_FILE_NAME_SIZE];
    axis2_char_t tmp_filename[AXUTIL_LOG_FILE_NAME_SIZE];

    if (!allocator)
        return NULL;

    log_impl = (axutil_log_impl_t *) AXIS2_MALLOC(allocator,
                   sizeof(axutil_log_impl_t));

    if (!log_impl)
        return NULL;

    log_impl->mutex =
        axutil_thread_mutex_create(allocator, AXIS2_THREAD_MUTEX_DEFAULT);

    if (!log_impl->mutex)
    {
        fprintf(stderr, "cannot create log mutex \n");
        return NULL;
    }

#ifndef WIN32
    signal(SIGXFSZ, SIG_IGN);
#endif

    /* default log file is axis2.log */
    if (stream_name)
        AXIS2_SNPRINTF(tmp_filename, AXUTIL_LOG_FILE_NAME_SIZE, "%s", stream_name);
    else
        AXIS2_SNPRINTF(tmp_filename, AXUTIL_LOG_FILE_NAME_SIZE, "%s", "axis2.log");

    /* we write all logs to AXIS2C_HOME/logs if it is set otherwise
     * to the working dir
     */
    if (stream_name && !(axutil_rindex(stream_name, AXIS2_PATH_SEP_CHAR)))
    {
        path_home = AXIS2_GETENV("AXIS2C_HOME");
        if (path_home)
        {
            AXIS2_SNPRINTF(log_dir, AXUTIL_LOG_FILE_NAME_SIZE, "%s%c%s", 
                path_home, AXIS2_PATH_SEP_CHAR, "logs");
            if (AXIS2_SUCCESS ==
                axutil_file_handler_access(log_dir, AXIS2_F_OK))
            {
                AXIS2_SNPRINTF(log_file_name, AXUTIL_LOG_FILE_NAME_SIZE, 
                    "%s%c%s", log_dir, AXIS2_PATH_SEP_CHAR, tmp_filename);
            }
            else
            {
                fprintf(stderr, "log folder %s does not exist - log file %s "\
                    "is written to . dir\n", log_dir, tmp_filename);
                AXIS2_SNPRINTF(log_file_name, AXUTIL_LOG_FILE_NAME_SIZE, "%s", 
                    tmp_filename);
            }
        }
        else
        {
            fprintf(stderr,
                "AXIS2C_HOME is not set - log is written to . dir\n");
            AXIS2_SNPRINTF(log_file_name, AXUTIL_LOG_FILE_NAME_SIZE, "%s", 
                tmp_filename);
        }
    }
    else
    {
        AXIS2_SNPRINTF(log_file_name, AXUTIL_LOG_FILE_NAME_SIZE, "%s", 
            tmp_filename);
    }
    log_impl->file_name = AXIS2_MALLOC(allocator, AXUTIL_LOG_FILE_NAME_SIZE);
    log_impl->log.size = AXUTIL_LOG_FILE_SIZE;
    sprintf(log_impl->file_name, "%s", log_file_name);

    axutil_thread_mutex_lock(log_impl->mutex);

    log_impl->stream = axutil_file_handler_open(log_file_name, "a+");
    axutil_log_impl_rotate((axutil_log_t *) log_impl);

    axutil_thread_mutex_unlock(log_impl->mutex);

    if (!log_impl->stream)
        log_impl->stream = stderr;

    /* by default, log is enabled */
    log_impl->log.enabled = 1;
    log_impl->log.level = AXIS2_LOG_LEVEL_DEBUG;

    if (ops)
    {
        log_impl->log.ops = ops;
    }
    else
    {
        log_impl->log.ops = &axutil_log_ops_var;
    }

    return &(log_impl->log);
}

static void AXIS2_CALL
axutil_log_impl_write(
    axutil_log_t *log,
    const axis2_char_t *buffer,
    axutil_log_levels_t level,
    const axis2_char_t *file,
    const int line)
{
    if (log && log->enabled && buffer)
    {
        axutil_log_impl_t *l = AXUTIL_INTF_TO_IMPL(log);
        if (!l->mutex)
            fprintf(stderr, "Log mutex is not found\n");
        if (!l->stream)
            fprintf(stderr, "Stream is not found\n");
        if(level <= log->level || level == AXIS2_LOG_LEVEL_CRITICAL)
        {
            axutil_log_impl_write_to_file(log, l->mutex, level, file, line, buffer);
        }
    }
#ifndef AXIS2_NO_LOG_FILE
    else if (buffer)
        fprintf(stderr, "please check your log and buffer");
#endif
    else
        fprintf(stderr, "please check your log and buffer");
}

AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_write_to_file(
    axutil_log_t *log,
    axutil_thread_mutex_t *mutex,
    axutil_log_levels_t level,
    const axis2_char_t *file,
    const int line,
    const axis2_char_t *value)
{
    const char *level_str = "";
    axutil_log_impl_t *log_impl = AXUTIL_INTF_TO_IMPL(log);
    FILE *fd = NULL;

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
    case AXIS2_LOG_LEVEL_USER:
        break;
    }
    axutil_thread_mutex_lock(mutex);

    axutil_log_impl_rotate(log);
    fd = log_impl->stream;
   
    if (fd)
    {
        if (file)
            fprintf(fd, "[%s] %s%s(%d) %s\n", axutil_log_impl_get_time_str(),
                level_str, file, line, value);
        else
            fprintf(fd, "[%s] %s %s\n", axutil_log_impl_get_time_str(), level_str,
                value);
        fflush(fd);
    }
    axutil_thread_mutex_unlock(mutex);
}

static axis2_status_t
axutil_log_impl_rotate(
    axutil_log_t *log)
{
    long size = -1;
    FILE *old_log_fd = NULL;
    axis2_char_t old_log_file_name[AXUTIL_LOG_FILE_NAME_SIZE];
    axutil_log_impl_t *log_impl = AXUTIL_INTF_TO_IMPL(log);
    if(log_impl->file_name)
        size = axutil_file_handler_size(log_impl->file_name);
  
    if(size >= log->size)
    {
        AXIS2_SNPRINTF(old_log_file_name, AXUTIL_LOG_FILE_NAME_SIZE, "%s%s", 
            log_impl->file_name, ".old");
        axutil_file_handler_close(log_impl->stream);
        old_log_fd = axutil_file_handler_open(old_log_file_name, "w+");
        log_impl->stream = axutil_file_handler_open(log_impl->file_name, "r");
        if(old_log_fd && log_impl->stream)
        {
            axutil_file_handler_copy(log_impl->stream, old_log_fd);
            axutil_file_handler_close(old_log_fd);
            axutil_file_handler_close(log_impl->stream);
            old_log_fd = NULL;
            log_impl->stream = NULL;
        }
        if(old_log_fd)
        {
            axutil_file_handler_close(old_log_fd);
        }
        if(log_impl->stream)
        {
            axutil_file_handler_close(log_impl->stream);
        }
        log_impl->stream = axutil_file_handler_open(log_impl->file_name, "w+");
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_log_user(
    axutil_log_t *log,
    const axis2_char_t *file,
    const int line,
    const axis2_char_t *format,
    ...)
{
    if (log && log->ops && log->ops->write &&
        format && log->enabled)
    {
        if(AXIS2_LOG_LEVEL_DEBUG <= log->level)
        {
            char value[AXIS2_LEN_VALUE + 1];
            va_list ap;
            va_start(ap, format);
            AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
            va_end(ap);
            log->ops->write(log, value, AXIS2_LOG_LEVEL_DEBUG, file, line);
        }
    }
#ifndef AXIS2_NO_LOG_FILE
    else
        fprintf(stderr, "please check your log and buffer");
#endif
}

AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_log_debug(
    axutil_log_t *log,
    const axis2_char_t *file,
    const int line,
    const axis2_char_t *format,
    ...)
{
    if (log && log->ops && log->ops->write &&
        format && log->enabled)
    {
        if(AXIS2_LOG_LEVEL_DEBUG <= log->level && log->level != AXIS2_LOG_LEVEL_USER)
        {
            char value[AXIS2_LEN_VALUE + 1];
            va_list ap;
            va_start(ap, format);
            AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
            va_end(ap);
            log->ops->write(log, value, AXIS2_LOG_LEVEL_DEBUG, file, line);
        }
    }
#ifndef AXIS2_NO_LOG_FILE
    else
        fprintf(stderr, "please check your log and buffer");
#endif
}

AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_log_info(
    axutil_log_t *log,
    const axis2_char_t *format,
    ...)
{
    if (log && log->ops && log->ops->write &&
        format && log->enabled)
    {
        if(AXIS2_LOG_LEVEL_INFO <= log->level && log->level != AXIS2_LOG_LEVEL_USER)
        {
            char value[AXIS2_LEN_VALUE + 1];
            va_list ap;
            va_start(ap, format);
            AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
            va_end(ap);
            log->ops->write(log, value, AXIS2_LOG_LEVEL_INFO, NULL, -1);
        }
    }
#ifndef AXIS2_NO_LOG_FILE
    else
        fprintf(stderr, "please check your log and buffer");
#endif
}

AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_log_warning(
    axutil_log_t *log,
    const axis2_char_t *file,
    const int line,
    const axis2_char_t *format,
    ...)
{
    if (log && log->ops && log->ops->write &&
        format && log->enabled)
    {
        if(AXIS2_LOG_LEVEL_WARNING <= log->level && log->level != AXIS2_LOG_LEVEL_USER)
        {
            char value[AXIS2_LEN_VALUE + 1];
            va_list ap;
            va_start(ap, format);
            AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
            va_end(ap);
            log->ops->write(log, value, AXIS2_LOG_LEVEL_WARNING, file, line);
        }
    }
#ifndef AXIS2_NO_LOG_FILE
    else
        fprintf(stderr, "please check your log and buffer");
#endif
}

AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_log_error(
    axutil_log_t *log,
    const axis2_char_t *file,
    const int line,
    const axis2_char_t *format,
    ...)
{
    if (log && log->ops && log->ops->write &&
        format && log->enabled)
    {
        char value[AXIS2_LEN_VALUE + 1];
        va_list ap;
        va_start(ap, format);
        AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
        va_end(ap);
        log->ops->write(log, value, AXIS2_LOG_LEVEL_ERROR, file, line);
    }
#ifndef AXIS2_NO_LOG_FILE
    else
        fprintf(stderr, "please check your log and buffer");
#endif
}

AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_log_critical(
    axutil_log_t *log,
    const axis2_char_t *file,
    const int line,
    const axis2_char_t *format,
    ...)
{
    if (log && log->ops && log->ops->write &&
        format && log->enabled)
    {
        char value[AXIS2_LEN_VALUE + 1];
        va_list ap;
        va_start(ap, format);
        AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
        va_end(ap);
        log->ops->write(log, value, AXIS2_LOG_LEVEL_CRITICAL, file, line);
    }
#ifndef AXIS2_NO_LOG_FILE
    else
        fprintf(stderr, "please check your log and buffer");
#endif
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_log_impl_get_time_str(
    void)
{
    time_t tp;
    char *time_str;
    tp = time(&tp);
    time_str = ctime(&tp);
    if (!time_str)
    {
        return NULL;
    }
    if ('\n' == time_str[strlen(time_str) - 1])
    {
        time_str[strlen(time_str) - 1] = '\0';
    }
    return time_str;
}

AXIS2_EXTERN axutil_log_t *AXIS2_CALL
axutil_log_create_default(
    axutil_allocator_t *allocator)
{
    axutil_log_impl_t *log_impl;

    if (!allocator)
        return NULL;

    log_impl = (axutil_log_impl_t *) AXIS2_MALLOC(allocator,
                   sizeof(axutil_log_impl_t));

    if (!log_impl)
        return NULL;

    log_impl->mutex =
        axutil_thread_mutex_create(allocator, AXIS2_THREAD_MUTEX_DEFAULT);

    if (!log_impl->mutex)
    {
        fprintf(stderr, "cannot create log mutex \n");
        return NULL;
    }

    axutil_thread_mutex_lock(log_impl->mutex);
    log_impl->file_name = NULL;
    log_impl->log.size = AXUTIL_LOG_FILE_SIZE;
    log_impl->stream = stderr;
    axutil_thread_mutex_unlock(log_impl->mutex);
    /* by default, log is enabled */
    log_impl->log.enabled = 1;
    log_impl->log.level = AXIS2_LOG_LEVEL_DEBUG;

    log_impl->log.ops = &axutil_log_ops_var;

    return &(log_impl->log);
}

#ifdef AXIS2_TRACE
AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_log_trace(
    axutil_log_t *log,
    const axis2_char_t *file,
    const int line,
    const axis2_char_t *format,
    ...)
{
    if (log && log->ops && log->ops->write &&
        format && log->enabled)
        {
            if(AXIS2_LOG_LEVEL_TRACE <= log->level && log->level != AXIS2_LOG_LEVEL_USER)
            {
                char value[AXIS2_LEN_VALUE + 1];
                va_list ap;
                va_start(ap, format);
                AXIS2_VSNPRINTF(value, AXIS2_LEN_VALUE, format, ap);
                va_end(ap);
                log->ops->write(log, value, AXIS2_LOG_LEVEL_TRACE, file, line);
            }
        }
#ifndef AXIS2_NO_LOG_FILE
    else
        fprintf(stderr, "please check your log and buffer");
#endif
}

#else
AXIS2_EXTERN void AXIS2_CALL
axutil_log_impl_log_trace(
    axutil_log_t *log,
    const axis2_char_t *filename,
    const int linenumber,
    const axis2_char_t *format,
    ...)
{
}
#endif

AXIS2_EXTERN void AXIS2_CALL
axutil_log_free(
    axutil_allocator_t *allocator,
    struct axutil_log *log)
{
    log->ops->free(allocator, log);
}

AXIS2_EXTERN void AXIS2_CALL
axutil_log_write(
    axutil_log_t *log,
    const axis2_char_t *buffer,
    axutil_log_levels_t level,
    const axis2_char_t *file,
    const int line)
{
    log->ops->write(log, buffer, level, file, line);
}

