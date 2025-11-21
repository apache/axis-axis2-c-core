/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "wsdl2c_native.h"
#include <string.h>

/* Context creation and destruction */
AXIS2_EXTERN wsdl2c_context_t* AXIS2_CALL
wsdl2c_context_create(const axutil_env_t *env)
{
    wsdl2c_context_t *context = NULL;

    AXIS2_PARAM_CHECK(env->error, env, NULL);

    context = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_context_t));
    if (!context) {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset(context, 0, sizeof(wsdl2c_context_t));
    context->env = env;

    /* Initialize options with defaults */
    context->options = AXIS2_MALLOC(env->allocator, sizeof(wsdl2c_options_t));
    if (!context->options) {
        AXIS2_FREE(env->allocator, context);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset(context->options, 0, sizeof(wsdl2c_options_t));

    /* Set default values */
    context->options->language = axutil_strdup(env, "c");
    context->options->databinding = axutil_strdup(env, "adb");
    context->options->output_dir = axutil_strdup(env, ".");
    context->options->server_side = AXIS2_FALSE;
    context->options->service_descriptor = AXIS2_FALSE;
    context->options->generate_test = AXIS2_FALSE;
    context->options->unwrap = AXIS2_FALSE;
    context->options->generate_all = AXIS2_FALSE;
    context->options->async_only = AXIS2_FALSE;
    context->options->sync_only = AXIS2_FALSE;

    return context;
}

AXIS2_EXTERN void AXIS2_CALL
wsdl2c_context_free(wsdl2c_context_t *context, const axutil_env_t *env)
{
    if (!context) {
        return;
    }

    if (context->options) {
        if (context->options->wsdl_uri) {
            AXIS2_FREE(env->allocator, context->options->wsdl_uri);
        }
        if (context->options->output_dir) {
            AXIS2_FREE(env->allocator, context->options->output_dir);
        }
        if (context->options->package_name) {
            AXIS2_FREE(env->allocator, context->options->package_name);
        }
        if (context->options->language) {
            AXIS2_FREE(env->allocator, context->options->language);
        }
        if (context->options->databinding) {
            AXIS2_FREE(env->allocator, context->options->databinding);
        }
        AXIS2_FREE(env->allocator, context->options);
    }

    if (context->wsdl) {
        /* TODO: Free WSDL structure */
        AXIS2_FREE(env->allocator, context->wsdl);
    }

    if (context->doc) {
        xmlFreeDoc(context->doc);
    }

    AXIS2_FREE(env->allocator, context);
}

/* Print usage information */
AXIS2_EXTERN void AXIS2_CALL
wsdl2c_print_usage(void)
{
    printf("Usage: wsdl2c-native [options] -uri <url or path>\n");
    printf("\nwhere [options] include:\n");
    printf("  -o <path>                Specify a directory path for the generated code.\n");
    printf("  -a                       Generate async style code only (Default: off).\n");
    printf("  -s                       Generate sync style code only (Default: off). Takes precedence over -a.\n");
    printf("  -p <pkg1>                Specify a custom package name for the generated code.\n");
    printf("  -l <language>            Valid languages are c (Default: c).\n");
    printf("  -t                       Generate a test case for the generated code.\n");
    printf("  -ss                      Generate server side code (i.e. skeletons) (Default: off).\n");
    printf("  -sd                      Generate service descriptor (i.e. services.xml). (Default: off). Valid with -ss.\n");
    printf("  -d <databinding>         Valid databinding(s) are adb, none (Default: adb).\n");
    printf("  -g                       Generates all the classes. It generates clases for both client side and server side.\n");
    printf("  -u                       Unpacks the databinding classes / Unwrap parameters.\n");
    printf("  -uri <url or path>       A url or path to a WSDL (Required).\n");
    printf("  -h, --help               Show this help message.\n");
    printf("  -v, --version            Show version information.\n");
    printf("\nThis is the native C implementation of WSDL2C, targeting compatibility with Axis2/Java 2.0.1-SNAPSHOT output.\n");
}

/* Parse command line options */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
wsdl2c_parse_options(wsdl2c_context_t *context, int argc, char **argv, const axutil_env_t *env)
{
    AXIS2_PARAM_CHECK(env->error, context, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, argv, AXIS2_FAILURE);

    /* Manual argument parsing to handle non-standard options like -uri, -ss, -sd */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-uri") == 0 && i + 1 < argc) {
            context->options->wsdl_uri = axutil_strdup(env, argv[i + 1]);
            i++; /* Skip the URI argument */
        } else if (strcmp(argv[i], "-ss") == 0) {
            context->options->server_side = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-sd") == 0) {
            context->options->service_descriptor = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            if (context->options->output_dir) {
                AXIS2_FREE(env->allocator, context->options->output_dir);
            }
            context->options->output_dir = axutil_strdup(env, argv[i + 1]);
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            if (context->options->databinding) {
                AXIS2_FREE(env->allocator, context->options->databinding);
            }
            context->options->databinding = axutil_strdup(env, argv[i + 1]);
            if (strcmp(argv[i + 1], "adb") != 0 && strcmp(argv[i + 1], "none") != 0) {
                fprintf(stderr, "Error: Only 'adb' and 'none' databinding are supported\n");
                return AXIS2_FAILURE;
            }
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            if (context->options->package_name) {
                AXIS2_FREE(env->allocator, context->options->package_name);
            }
            context->options->package_name = axutil_strdup(env, argv[i + 1]);
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            if (context->options->language) {
                AXIS2_FREE(env->allocator, context->options->language);
            }
            context->options->language = axutil_strdup(env, argv[i + 1]);
            if (strcmp(argv[i + 1], "c") != 0) {
                fprintf(stderr, "Error: Only 'c' language is supported by this native implementation\n");
                return AXIS2_FAILURE;
            }
            i++; /* Skip the argument */
        } else if (strcmp(argv[i], "-u") == 0) {
            context->options->unwrap = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-a") == 0) {
            context->options->async_only = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-s") == 0) {
            context->options->sync_only = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-t") == 0) {
            context->options->generate_test = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-g") == 0) {
            context->options->generate_all = AXIS2_TRUE;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            wsdl2c_print_usage();
            return AXIS2_FAILURE; /* Exit after showing help */
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("wsdl2c-native version 1.0.0\n");
            printf("Native C implementation of WSDL2C code generator\n");
            printf("Compatible with Axis2/Java 2.0.1-SNAPSHOT output format\n");
            return AXIS2_FAILURE; /* Exit after showing version */
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Unknown option: %s. Use -h for help.\n", argv[i]);
            return AXIS2_FAILURE;
        }
    }


    /* Validate required options */
    if (!context->options->wsdl_uri) {
        fprintf(stderr, "Error: WSDL URI is required. Use -uri <path_to_wsdl>\n");
        return AXIS2_FAILURE;
    }

    /* Validate option combinations */
    if (context->options->service_descriptor && !context->options->server_side) {
        fprintf(stderr, "Warning: -sd option is only valid with -ss (server side generation)\n");
        context->options->service_descriptor = AXIS2_FALSE;
    }

    if (context->options->sync_only && context->options->async_only) {
        fprintf(stderr, "Warning: -s takes precedence over -a\n");
        context->options->async_only = AXIS2_FALSE;
    }

    return AXIS2_SUCCESS;
}

/* Utility function implementations for standalone mode */

/* Wrapper functions to match Axis2 allocator signatures */
static void* standalone_malloc(void *allocator, size_t size) {
    (void)allocator; /* Unused in standalone mode */
    return malloc(size);
}

static void* standalone_realloc(void *allocator, void *ptr, size_t size) {
    (void)allocator; /* Unused in standalone mode */
    return realloc(ptr, size);
}

static void standalone_free(void *allocator, void *ptr) {
    (void)allocator; /* Unused in standalone mode */
    free(ptr);
}

AXIS2_EXTERN axutil_env_t* AXIS2_CALL
axutil_env_create_all(const char *log_file, int log_level)
{
    axutil_env_t *env = NULL;
    axutil_allocator_t *allocator = NULL;
    axutil_error_t *error = NULL;
    axutil_log_t *log = NULL;

    (void)log_file; /* Unused parameter */

    /* Simple allocator implementation */
    allocator = malloc(sizeof(axutil_allocator_t));
    if (!allocator) {
        return NULL;
    }
    allocator->malloc_fn = standalone_malloc;
    allocator->realloc_fn = standalone_realloc;
    allocator->free_fn = standalone_free;

    /* Simple error structure */
    error = malloc(sizeof(axutil_error_t));
    if (!error) {
        free(allocator);
        return NULL;
    }
    error->error_number = 0;
    error->status_code = 0;

    /* Simple log structure */
    log = malloc(sizeof(axutil_log_t));
    if (!log) {
        free(error);
        free(allocator);
        return NULL;
    }
    log->level = log_level;

    /* Environment structure */
    env = malloc(sizeof(axutil_env_t));
    if (!env) {
        free(log);
        free(error);
        free(allocator);
        return NULL;
    }
    env->allocator = allocator;
    env->error = error;
    env->log = log;

    return env;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_env_free(axutil_env_t *env)
{
    if (env) {
        if (env->log) {
            free(env->log);
        }
        if (env->error) {
            free(env->error);
        }
        if (env->allocator) {
            free(env->allocator);
        }
        free(env);
    }
}

AXIS2_EXTERN char* AXIS2_CALL
axutil_strdup(const axutil_env_t *env, const char *str)
{
    char *result = NULL;
    size_t len;

    if (!str) {
        return NULL;
    }

    len = strlen(str) + 1;
    if (env && env->allocator && env->allocator->malloc_fn) {
        result = env->allocator->malloc_fn(env->allocator, len);
    } else {
        result = malloc(len);
    }

    if (result) {
        strcpy(result, str);
    }

    return result;
}

AXIS2_EXTERN char* AXIS2_CALL
axutil_string_toupper(const char *str, const axutil_env_t *env)
{
    char *result = NULL;
    int i;

    if (!str) {
        return NULL;
    }

    /* Create a copy of the string */
    result = axutil_strdup(env, str);
    if (!result) {
        return NULL;
    }

    /* Convert to uppercase in place */
    for (i = 0; result[i]; i++) {
        if (result[i] >= 'a' && result[i] <= 'z') {
            result[i] = result[i] - 'a' + 'A';
        }
    }

    return result;
}

#ifdef STANDALONE_BUILD
/* Simple array list implementation for standalone mode */
axutil_array_list_t* axutil_array_list_create(const axutil_env_t *env, int initial_capacity)
{
    axutil_array_list_t *list = NULL;

    (void)env; /* Suppress unused parameter warning */

    if (initial_capacity <= 0) {
        initial_capacity = 10;
    }

    list = malloc(sizeof(axutil_array_list_t));
    if (!list) {
        return NULL;
    }

    list->data = malloc(sizeof(void*) * initial_capacity);
    if (!list->data) {
        free(list);
        return NULL;
    }

    list->size = 0;
    list->capacity = initial_capacity;

    return list;
}

axis2_status_t axutil_array_list_add(axutil_array_list_t *list, const axutil_env_t *env, void *element)
{
    (void)env; /* Suppress unused parameter warning */

    if (!list) {
        return AXIS2_FAILURE;
    }

    if (list->size >= list->capacity) {
        /* Expand array */
        int new_capacity = list->capacity * 2;
        void **new_data = realloc(list->data, sizeof(void*) * new_capacity);
        if (!new_data) {
            return AXIS2_FAILURE;
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }

    list->data[list->size] = element;
    list->size++;

    return AXIS2_SUCCESS;
}

void axutil_array_list_free(axutil_array_list_t *list, const axutil_env_t *env)
{
    (void)env; /* Suppress unused parameter warning */

    if (list) {
        if (list->data) {
            free(list->data);
        }
        free(list);
    }
}
#endif